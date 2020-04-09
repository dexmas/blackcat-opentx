/**
 * \file
 *
 * \brief USB Device Human Interface Device (HID) mouse interface.
 *
 * Copyright (c) 2009-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
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
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _UDI_HID_MOUSE_H_
#define _UDI_HID_MOUSE_H_

#include "usb_config.h"
#include "usb_protocol.h"
#include "usb_protocol_hid.h"
#include "udc_desc.h"
#include "udi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup udi_hid_mouse_group_udc
 * @{
 */
//! Global structure which contains standard UDI API for UDC
extern UDC_DESC_STORAGE udi_api_t udi_api_hid_mouse;
//@}

/**
 * \ingroup udi_hid_mouse_group
 * \defgroup udi_hid_mouse_group_desc USB interface descriptors
 *
 * The following structures provide predefined USB interface descriptors.
 * It must be used to define the final USB descriptors.
 */
//@{

//! Interface descriptor structure for HID mouse
typedef struct {
	usb_iface_desc_t iface;
	usb_hid_descriptor_t hid;
	usb_ep_desc_t ep;
} udi_hid_mouse_desc_t;

//! Report descriptor for HID mouse
typedef struct {
	uint8_t array[25 * 2 + 2 * 1];
} udi_hid_mouse_report_desc_t;


//! By default no string associated to this interface
#ifndef UDI_HID_MOUSE_STRING_ID
#define UDI_HID_MOUSE_STRING_ID 0
#endif

//! HID mouse endpoints size
#define UDI_HID_MOUSE_EP_SIZE  8

//! Content of HID mouse interface descriptor for all speed
#define UDI_HID_MOUSE_DESC    {\
	.iface.bLength             = sizeof(usb_iface_desc_t),\
	.iface.bDescriptorType     = USB_DT_INTERFACE,\
	.iface.bInterfaceNumber    = UDI_HID_MOUSE_IFACE_NUMBER,\
	.iface.bAlternateSetting   = 0,\
	.iface.bNumEndpoints       = 1,\
	.iface.bInterfaceClass     = HID_CLASS,\
	.iface.bInterfaceSubClass  = HID_SUB_CLASS_BOOT,\
	.iface.bInterfaceProtocol  = HID_PROTOCOL_MOUSE,\
	.iface.iInterface          = UDI_HID_MOUSE_STRING_ID,\
	.hid.bLength               = sizeof(usb_hid_descriptor_t),\
	.hid.bDescriptorType       = USB_DT_HID,\
	.hid.bcdHID                = USB_HID_BDC_V1_11,\
	.hid.bCountryCode          = USB_HID_NO_COUNTRY_CODE,\
	.hid.bNumDescriptors       = USB_HID_NUM_DESC,\
	.hid.bRDescriptorType      = USB_DT_HID_REPORT,\
	.hid.wDescriptorLength     = sizeof(udi_hid_mouse_report_desc_t),\
	.ep.bLength                = sizeof(usb_ep_desc_t),\
	.ep.bDescriptorType        = USB_DT_ENDPOINT,\
	.ep.bEndpointAddress       = UDI_HID_MOUSE_EP_IN,\
	.ep.bmAttributes           = USB_EP_TYPE_INTERRUPT,\
	.ep.wMaxPacketSize         = UDI_HID_MOUSE_EP_SIZE,\
	.ep.bInterval              = 10,\
	}
//@}
//@}


/**
 * \ingroup udi_hid_group
 * \defgroup udi_hid_mouse_group USB Device Interface (UDI) for Human Interface Device (HID) Mouse Class
 *
 * Common APIs used by high level application to use this USB class.
 *
 * See \ref udi_hid_mouse_quickstart.
 * @{
 */

/**
 * \name Interfaces for mouse events
 */
//@{

/**
 * \brief Move the scroll wheel
 *
 * \param pos     Signed value to move
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_hid_mouse_moveScroll(int8_t pos);

/**
 * \brief Move the mouse pointer on Y axe
 *
 * \param pos_y   Signed value to move
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_hid_mouse_moveY(int8_t pos_y);

/**
 * \brief Move the mouse pointer on X axe
 *
 * \param pos_x   Signed value to move
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_hid_mouse_moveX(int8_t pos_x);
//@}

/**
 * \name Interfaces for buttons events
 */
//@{

//! Value to signal a button down (pressed)
#define  HID_MOUSE_BTN_DOWN      true
//! Value to signal a button up (released)
#define  HID_MOUSE_BTN_UP        false

/**
 * \brief Changes middle button state
 *
 * \param b_state    New button state
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_hid_mouse_btnmiddle(bool b_state);

/**
 * \brief Changes right button state
 *
 * \param b_state    New button state
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_hid_mouse_btnright(bool b_state);

/**
 * \brief Changes left button state
 *
 * \param b_state    New button state
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_hid_mouse_btnleft(bool b_state);
//@}

//@}

#ifdef __cplusplus
}
#endif

#endif // _UDI_HID_MOUSE_H_
