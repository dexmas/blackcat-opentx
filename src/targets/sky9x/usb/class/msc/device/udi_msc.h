/**
 * USB Device Mass Storage Class (MSC) interface definitions.
 *
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

#ifndef _UDI_MSC_H_
#define _UDI_MSC_H_

#include "usb_config.h"
#include "usb_protocol.h"
#include "class/msc/usb_protocol_msc.h"
#include "udc/udd.h"
#include "udc/udc_desc.h"
#include "udc/udi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup udi_msc_group_udc
 */
//! Global structure which contains standard UDI interface for UDC
extern UDC_DESC_STORAGE udi_api_t udi_api_msc;

/**
 * \ingroup udi_msc_group
 * \defgroup udi_msc_group USB interface descriptors
 *
 * The following structures provide predefined USB interface descriptors.
 * It must be used to define the final USB descriptors.
 */

//! Interface descriptor structure for MSC
typedef struct {
	usb_iface_desc_t iface;
	usb_ep_desc_t ep_in;
	usb_ep_desc_t ep_out;
} udi_msc_desc_t;

//! By default no string associated to this interface
#ifndef UDI_MSC_STRING_ID
#define UDI_MSC_STRING_ID     0
#endif

//! MSC endpoints size for full speed
#define UDI_MSC_EPS_SIZE_FS   64
//! MSC endpoints size for high speed
#define UDI_MSC_EPS_SIZE_HS   512

//! Content of MSC interface descriptor for all speeds
#define UDI_MSC_DESC      \
   .iface.bLength             = sizeof(usb_iface_desc_t),\
   .iface.bDescriptorType     = USB_DT_INTERFACE,\
   .iface.bInterfaceNumber    = UDI_MSC_IFACE_NUMBER,\
   .iface.bAlternateSetting   = 0,\
   .iface.bNumEndpoints       = 2,\
   .iface.bInterfaceClass     = MSC_CLASS,\
   .iface.bInterfaceSubClass  = MSC_SUBCLASS_TRANSPARENT,\
   .iface.bInterfaceProtocol  = MSC_PROTOCOL_BULK,\
   .iface.iInterface          = UDI_MSC_STRING_ID,\
   .ep_in.bLength             = sizeof(usb_ep_desc_t),\
   .ep_in.bDescriptorType     = USB_DT_ENDPOINT,\
   .ep_in.bEndpointAddress    = UDI_MSC_EP_IN,\
   .ep_in.bmAttributes        = USB_EP_TYPE_BULK,\
   .ep_in.bInterval           = 0,\
   .ep_out.bLength            = sizeof(usb_ep_desc_t),\
   .ep_out.bDescriptorType    = USB_DT_ENDPOINT,\
   .ep_out.bEndpointAddress   = UDI_MSC_EP_OUT,\
   .ep_out.bmAttributes       = USB_EP_TYPE_BULK,\
   .ep_out.bInterval          = 0,

//! Content of MSC interface descriptor for full speed only
#define UDI_MSC_DESC_FS   {\
   UDI_MSC_DESC \
   .ep_in.wMaxPacketSize      = UDI_MSC_EPS_SIZE_FS,\
   .ep_out.wMaxPacketSize     = UDI_MSC_EPS_SIZE_FS,\
   }

//! Content of MSC interface descriptor for high speed only
#define UDI_MSC_DESC_HS   {\
   UDI_MSC_DESC \
   .ep_in.wMaxPacketSize      = UDI_MSC_EPS_SIZE_HS,\
   .ep_out.wMaxPacketSize     = UDI_MSC_EPS_SIZE_HS,\
   }

/**
 * \ingroup udi_group
 * \defgroup udi_msc_group USB Device Interface (UDI) for Mass Storage Class (MSC)
 *
 * Common APIs used by high level application to use this USB class.
 *
 * These routines are used by memory to transfer its data
 * to/from USB MSC endpoints.
 *
 * See \ref udi_msc_quickstart.
 */

/**
 * \brief Process the background read/write commands
 *
 * Routine called by the main loop
 */
bool udi_msc_process_trans(void);

/**
 * \brief Transfers data to/from USB MSC endpoints
 *
 *
 * \param b_read        Memory to USB, if true
 * \param block         Buffer on Internal RAM to send or fill
 * \param block_size    Buffer size to send or fill
 * \param callback      Function to call at the end of transfer.
 *                      If NULL then the routine exit when transfer is finish.
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
bool udi_msc_trans_block(bool b_read, uint8_t * block, uint32_t block_size,
		void (*callback) (udd_ep_status_t status, uint32_t n, udd_ep_id_t ep));

#ifdef __cplusplus
}
#endif

#endif // _UDI_MSC_H_
