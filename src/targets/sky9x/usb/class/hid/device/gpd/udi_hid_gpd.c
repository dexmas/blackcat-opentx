/**
 * Copyright (c) 2009-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 */

#include "usb_config.h"
#include "usb_protocol.h"
#include "udc/udd.h"
#include "udc/udc.h"
#include "class/hid/device/udi_hid.h"
#include "udi_hid_gpd.h"
#include <string.h>

/**
 * Structures and functions required by UDC.
 */
bool udi_hid_gpd_enable(void);
void udi_hid_gpd_disable(void);
bool udi_hid_gpd_setup(void);
uint8_t udi_hid_gpd_getsetting(void);

/* Global structure which contains standard UDI interface for UDC */
UDC_DESC_STORAGE udi_api_t udi_api_hid_gpd = {
	.enable = (bool(*)(void))udi_hid_gpd_enable,
	.disable = (void (*)(void))udi_hid_gpd_disable,
	.setup = (bool(*)(void))udi_hid_gpd_setup,
	.getsetting = (uint8_t(*)(void))udi_hid_gpd_getsetting,
	.sof_notify = NULL,
};

/**
 * Internal defines and variables to manage HID gamepad
 */

/* ! Size of report for standard HID gamepad */
#define UDI_HID_GPD_REPORT_SIZE   9
//! To store current rate of HID gamepad
__attribute__((__aligned__(4))) static uint8_t udi_hid_gpd_rate;
//! To store current protocol of HID gamepad
__attribute__((__aligned__(4))) static uint8_t udi_hid_gpd_protocol;
/* ! To store report feedback from USB host */
static uint8_t udi_hid_gpd_report_set;
/* ! To signal if a valid report is ready to send */
static bool udi_hid_gpd_report_valid;
/* ! Report ready to send */
static uint8_t udi_hid_gpd_report[UDI_HID_GPD_REPORT_SIZE];
/* ! Signal if a report transfer is on going */
static bool udi_hid_gpd_report_trans_ongoing;
/* ! Buffer used to send report */
__attribute__((__aligned__(4))) 
static uint8_t udi_hid_gpd_report_trans[UDI_HID_GPD_REPORT_SIZE];

/* ! HID report descriptor for standard HID gamepad */
UDC_DESC_STORAGE udi_hid_gpd_report_desc_t udi_hid_gpd_report_desc = {
	{
        0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
		0x09, 0x05,                    //     USAGE (Game Pad)
		0xa1, 0x01,                    //     COLLECTION (Application)
		0xa1, 0x00,                    //       COLLECTION (Physical)
		0x05, 0x09,                    //         USAGE_PAGE (Button)
		0x19, 0x01,                    //         USAGE_MINIMUM (Button 1)
		0x29, 0x08,                    //         USAGE_MAXIMUM (Button 8)
		0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
		0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
		0x95, 0x08,                    //         REPORT_COUNT (8)
		0x75, 0x01,                    //         REPORT_SIZE (1)
		0x81, 0x02,                    //         INPUT (Data,Var,Abs)
		0x05, 0x01,                    //         USAGE_PAGE (Generic Desktop)
		0x09, 0x30,                    //         USAGE (X)
		0x09, 0x31,                    //         USAGE (Y)
		0x09, 0x32,                    //         USAGE (Z)
		0x09, 0x33,                    //         USAGE (Rx)
		0x09, 0x34,                    //         USAGE (Ry)
		0x09, 0x35,                    //         USAGE (Rz)
		0x09, 0x36,                    //         USAGE (Slider)
		0x09, 0x36,                    //         USAGE (Slider)
		0x15, 0x81,                    //         LOGICAL_MINIMUM (-127)
		0x25, 0x7f,                    //         LOGICAL_MAXIMUM (127)
		0x75, 0x08,                    //         REPORT_SIZE (8)
		0x95, 0x08,                    //         REPORT_COUNT (8)
		0x81, 0x02,                    //         INPUT (Data,Var,Abs)
		0xc0,                          //       END_COLLECTION
		0xc0                           //     END_COLLECTION
	}
};

/**
 * \name Internal routines
 */

/**
 * Changes gamepad report states (like LEDs)
 */
static bool udi_hid_gpd_setreport(void);

/**
 * \brief Send the report
 *
 * \return \c 1 if send on going, \c 0 if delay.
 */
static bool udi_hid_gpd_send_report(void);

/**
 * \brief Callback called when the report is sent
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finish
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param nb_sent    number of data transfered
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
static void udi_hid_gpd_report_sent(udd_ep_status_t status, uint32_t nb_sent, udd_ep_id_t ep);

/**
 * \brief Callback called to update report from USB host
 * udi_hid_gpd_report_set is updated before callback execution
 */
static void udi_hid_gpd_setreport_valid(void);

/* -------------------------------------------- */
/* ------ Interface for UDI HID level --------- */

bool udi_hid_gpd_enable(void)
{
	/* Initialize internal values */
	udi_hid_gpd_rate = 0;
	udi_hid_gpd_protocol = 0;
	udi_hid_gpd_report_trans_ongoing = false;
	memset(udi_hid_gpd_report, 0, UDI_HID_GPD_REPORT_SIZE);
	udi_hid_gpd_report_valid = false;
	return UDI_HID_GPD_ENABLE_EXT();
}

void udi_hid_gpd_disable(void)
{
	UDI_HID_GPD_DISABLE_EXT();
}

bool udi_hid_gpd_setup(void)
{
	return udi_hid_setup(&udi_hid_gpd_rate,
								&udi_hid_gpd_protocol,
								(uint8_t *) &udi_hid_gpd_report_desc,
								udi_hid_gpd_setreport);
}

uint8_t udi_hid_gpd_getsetting(void)
{
	return 0;
}

bool udi_hid_gpd_update(uint8_t* _buffer)
{

	memcpy(udi_hid_gpd_report, _buffer, UDI_HID_GPD_REPORT_SIZE);

	uint32_t flags = cpu_irq_save();

	// Valid and send report
	udi_hid_gpd_report_valid = true;
	udi_hid_gpd_send_report();

	cpu_irq_restore(flags);
	return true;
}

static bool udi_hid_gpd_setreport(void)
{
	if ((USB_HID_REPORT_TYPE_OUTPUT == (udd_g_ctrlreq.req.wValue >> 8)) &&
		(0 == (0xFF & udd_g_ctrlreq.req.wValue)) &&
		(1 == udd_g_ctrlreq.req.wLength)) {
		/* Report OUT type on report ID 0 from USB Host */
		udd_g_ctrlreq.payload = &udi_hid_gpd_report_set;
		udd_g_ctrlreq.callback = udi_hid_gpd_setreport_valid;
		udd_g_ctrlreq.payload_size = 1;
		return true;
	}

	return false;
}

//--------------------------------------------
//------ Internal routines

static bool udi_hid_gpd_send_report(void)
{
	if (udi_hid_gpd_report_trans_ongoing)
		return false;	// Transfer on going then send this one after transfer complete

	// Copy report on other array used only for transfer
	memcpy(udi_hid_gpd_report_trans, udi_hid_gpd_report, UDI_HID_GPD_REPORT_SIZE);
	udi_hid_gpd_report_valid = false;

	// Send report
	udi_hid_gpd_report_trans_ongoing =
			udd_ep_run(	UDI_HID_GPD_EP_IN,
							false,
							udi_hid_gpd_report_trans,
							UDI_HID_GPD_REPORT_SIZE,
							udi_hid_gpd_report_sent);
	return udi_hid_gpd_report_trans_ongoing;
}


static void udi_hid_gpd_report_sent(udd_ep_status_t status,
		uint32_t nb_sent, udd_ep_id_t ep)
{
	UNUSED(ep);
	UNUSED(status);
	UNUSED(nb_sent);
	// Valid report sending
	udi_hid_gpd_report_trans_ongoing = false;
	if (udi_hid_gpd_report_valid) {
		// Send new valid report
		udi_hid_gpd_send_report();
	}
}

static void udi_hid_gpd_setreport_valid(void)
{
	UDI_HID_GPD_CHANGE_LED(udi_hid_gpd_report_set);
}

/* @} */