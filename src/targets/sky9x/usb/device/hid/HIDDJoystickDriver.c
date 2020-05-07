/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/

#include "usb/common/hid/HIDDFunction.h"
#include "usb/device/hid/HIDDJoystickDriver.h"

#include "usb/device/core/USBD.h"
#include "usb/device/core/USBD_HAL.h"
#include "usb/device/core/USBDDriver.h"
#include "usb/device/core/pmc.h"

#include "sam4s16c.h"
#include "core_cm4.h"


///*------------------------------------------------------------------------------
// *         Internal types
// *------------------------------------------------------------------------------*/

/**
 * Struct for an HID Joystick report.
 */
typedef struct {
    /** Callback when report done */
    HIDDReportEventCallback fCallback;
    /** Callback arguments */
    void* pArg;

    /** Report size (ID + DATA) */
    uint16_t wMaxSize;
    /** Transfered size */
    uint16_t wTransferred;
    /** Report idle rate */
    uint8_t bIdleRate;
    /** Delay count for Idle */
    uint8_t bDelay;
    /** Report ID */
    uint8_t bID;
    /** Report data block */
    HIDDJoystickInputReport report;
} HIDDJoystickReport;

/**
 * Driver structure for an HID device implementing joystick functionalities.
 */
typedef struct {
    HIDDFunction hidDrv;
    HIDDReport   *inputReports[1];
} HIDDJoystickDriver;

/*------------------------------------------------------------------------------
 *         Internal variables
 *------------------------------------------------------------------------------*/

/** Static instance of the HID joystick device driver. */
/*static*/ HIDDJoystickDriver hiddJoystickDriver;
/** Input report */
/*static*/ HIDDJoystickReport hiddInputReport;

/** Report descriptor used by the driver. */
/*static*/ const uint8_t hiddReportDescriptor[] =
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
};

/*------------------------------------------------------------------------------
 *         Internal functions
 *------------------------------------------------------------------------------*/

/**
 * \brief Configure 48MHz Clock for USB
 */
static void _ConfigureUsbClock(void)
{
    /* Enable PLLB for USB */
    PMC->CKGR_PLLBR = CKGR_PLLBR_MULB(16-1)|CKGR_PLLBR_DIVB(2)|CKGR_PLLBR_PLLBCOUNT(0x3FU);

    while((PMC->PMC_SR & PMC_SR_LOCKB) == 0);
    /* USB Clock uses PLLB */
    PMC->PMC_USB = PMC_USB_USBDIV(1)|PMC_USB_USBS; /* PLLB */
}

/*----------------------------------------------------------------------------
 *         External variables
 *----------------------------------------------------------------------------*/

extern USBDDriverDescriptors hiddJoystickDriverDescriptors;


/*------------------------------------------------------------------------------
 *      Exported functions
 *------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *         Callbacks re-implementation
 *----------------------------------------------------------------------------*/

/**
 * Invoked after the USB driver has been initialized. By default, configures
 * the UDP/UDPHS interrupt.
 */
void USBDCallbacks_Initialized(void)
{
    NVIC_EnableIRQ(UDP_IRQn);
}

/**
 *  Invoked whenever a SETUP request is received from the host. Forwards the
 *  request to the standard handler.
 */
void USBDCallbacks_RequestReceived(const USBGenericRequest *request)
{
    HIDDJoystickDriver_RequestHandler(request);
}

/**
 * Invoked when the configuration of the device changes. Start reading
 * output reports.
 * \param cfgnum New configuration number.
 */
void USBDDriverCallbacks_ConfigurationChanged(uint8_t cfgnum)
{
    HIDDJoystickDriver_ConfigurationChangedHandler(cfgnum);
}

/**
 * Initializes the HID Joystick device driver.
 * \param pDescriptors Pointer to descriptor list for the HID Joystick.
 */
void HIDDJoystickDriver_Initialize(const USBDDriverDescriptors *pDescriptors)
{
    HIDDJoystickDriver* pJoystick = &hiddJoystickDriver;
    HIDDFunction* pHidd = &pJoystick->hidDrv;
    USBDDriver* pUsbd = USBD_GetDriver();

    /* One input report */
    pJoystick->inputReports[0] = (HIDDReport*)&hiddInputReport;
    HIDDFunction_InitializeReport(pJoystick->inputReports[0],
                                  HIDDJoystickDriver_REPORTDESCRIPTORSIZE,
                                  0,
                                  0, 0);

    /* Initialize USBD Driver instance */
    USBDDriver_Initialize(pUsbd,
                          pDescriptors,
                          0); /* Multiple interface settings not supported */
    /* Function initialize */
    HIDDFunction_Initialize(pHidd,
                            pUsbd, 0,
                            hiddReportDescriptor,
                            (HIDDReport**)(&pJoystick->inputReports), 1,
                            0, 0);
    USBD_Init();
}

/**
 * Handles configureation changed event.
 * \param cfgnum New configuration number
 */
void HIDDJoystickDriver_ConfigurationChangedHandler(uint8_t cfgnum)
{
    HIDDJoystickDriver* pJoystick = &hiddJoystickDriver;
    HIDDFunction * pHidd = &pJoystick->hidDrv;
    USBDDriver * pUsbd = pHidd->pUsbd;
    USBConfigurationDescriptor *pDesc;

    if (cfgnum > 0) {

        /* Parse endpoints for reports */
        pDesc = USBDDriver_GetCfgDescriptors(pUsbd, cfgnum);
        HIDDFunction_ParseInterface(pHidd,
                                    (USBGenericDescriptor*)pDesc,
                                    pDesc->wTotalLength);
    }
}

/**
 * Handles HID-specific SETUP request sent by the host.
 * \param request Pointer to a USBGenericRequest instance
 */
void HIDDJoystickDriver_RequestHandler(const USBGenericRequest *request)
{
    HIDDJoystickDriver* pJoystick = &hiddJoystickDriver;
    HIDDFunction * pHidd = &pJoystick->hidDrv;
    USBDDriver * pUsbd = pHidd->pUsbd;

    /* Process HID requests */
    if (USBRC_SUCCESS == HIDDFunction_RequestHandler(pHidd,
                                                     request)) {
        return;
    }
    /* Process STD requests */
    else {

        USBDDriver_RequestHandler(pUsbd, request);
    }
}

uint8_t HIDDJoystickDriver_Change( uint8_t *data )
{
	uint32_t i ;
    HIDDJoystickDriver* pJoystick = &hiddJoystickDriver;
    HIDDFunction * pHidd = &pJoystick->hidDrv;
    HIDDJoystickInputReport * pReport = &hiddInputReport.report;

    pReport->bmButtons = *data++ ;
	for ( i = 0 ; i < 8 ; i += 1 )
	{
  	    pReport->analog[i] = *data++ ;
	}

    /* Send input report through the interrupt IN endpoint */
    return USBD_Write(pHidd->bPipeIN,
                      (void*)pReport,
                      9, //sizeof(hiddInputReport),
                      0,
                      0);
}

void HIDDJoystickDriver_RemoteWakeUp(void)
{
    HIDDJoystickDriver * pJoystick = &hiddJoystickDriver;
    HIDDFunction * pHidd = &pJoystick->hidDrv;
    USBDDriver * pUsbd = pHidd->pUsbd;

    /* Remote wake-up has been enabled */
    if (USBDDriver_IsRemoteWakeUpEnabled(pUsbd)) {

        USBD_RemoteWakeUp();
    }
}

void startJoystick(void)
{
	/* Enable UPLL for USB */
	_ConfigureUsbClock();
    HIDDJoystickDriver_Initialize(&hiddJoystickDriverDescriptors);

    USBD_Connect();
}

