/**
 * \file
 *
 * \brief USB Device Human Interface Device (HID) gamepad interface.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef _UDC_HID_KBD_H_
#define _UDC_HID_KBD_H_

#include "usb_config.h"
#include "usb_protocol.h"
#include "usb_protocol_hid.h"
#include "udc_desc.h"
#include "udi.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \addtogroup udi_hid_gamepad_group_udc
     * @{
     */
     /* ! Global structure which contains standard UDI API for UDC */
    extern UDC_DESC_STORAGE udi_api_t udi_api_hid_gpd;
    /* @} */

    /**
     * \ingroup udi_hid_gamepad_group
     * \defgroup udi_hid_gamepad_group_desc USB interface descriptors
     *
     * The following structures provide predefined USB interface descriptors.
     * It must be used to define the final USB descriptors.
     */
     /* @{ */

     /* ! Interface descriptor structure for HID gamepad */
    typedef struct {
        usb_iface_desc_t iface;
        usb_hid_descriptor_t hid;
        usb_ep_desc_t ep;
    } udi_hid_gpd_desc_t;

    /* ! Report descriptor for HID gamepad */
    typedef struct {
        //uint8_t array[54];
        uint8_t array[75];
    } udi_hid_gpd_report_desc_t;

    /* ! By default no string associated to this interface */
#ifndef UDI_HID_GPD_STRING_ID
#define UDI_HID_GPD_STRING_ID 0
#endif

/* ! HID gamepad endpoints size */
#define UDI_HID_GPD_EP_SIZE  8

/* ! Content of HID gamepad interface descriptor for all speed */
#define UDI_HID_GPD_DESC    { \
		.iface.bLength             = sizeof(usb_iface_desc_t), \
		.iface.bDescriptorType     = USB_DT_INTERFACE, \
		.iface.bInterfaceNumber    = UDI_HID_GPD_IFACE_NUMBER, \
		.iface.bAlternateSetting   = 0,	\
		.iface.bNumEndpoints       = 1,	\
		.iface.bInterfaceClass     = HID_CLASS,	\
		.iface.bInterfaceSubClass  = HID_SUB_CLASS_NOBOOT, \
		.iface.bInterfaceProtocol  = 0x00, \
		.iface.iInterface          = UDI_HID_GPD_STRING_ID, \
		.hid.bLength               = sizeof(usb_hid_descriptor_t), \
		.hid.bDescriptorType       = USB_DT_HID, \
		.hid.bcdHID                = USB_HID_BDC_V1_11, \
		.hid.bCountryCode          = USB_HID_NO_COUNTRY_CODE, \
		.hid.bNumDescriptors       = USB_HID_NUM_DESC, \
		.hid.bRDescriptorType      = USB_DT_HID_REPORT,	\
		.hid.wDescriptorLength     = sizeof(udi_hid_gpd_report_desc_t), \
		.ep.bLength                = sizeof(usb_ep_desc_t), \
		.ep.bDescriptorType        = USB_DT_ENDPOINT, \
		.ep.bEndpointAddress       = UDI_HID_GPD_EP_IN,	\
		.ep.bmAttributes           = USB_EP_TYPE_INTERRUPT, \
		.ep.wMaxPacketSize         = UDI_HID_GPD_EP_SIZE,	\
		.ep.bInterval              = 5,	\
}
/* @} */

/**
 * \ingroup udi_hid_group
 * \defgroup udi_hid_gamepad_group USB Device Interface (UDI) for Human
 * Interface Device (HID) gamepad Class
 *
 * Common APIs used by high level application to use this USB class.
 *
 * See \ref udi_hid_gamepad_quickstart.
 * @{
 */


    bool udi_hid_gpd_fill_report(uint8_t* _report);

    /* @} */

#ifdef __cplusplus
}
#endif

#endif /* _UDC_HID_gpd_H_ */