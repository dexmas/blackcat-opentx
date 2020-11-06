/**
 * Default descriptors for a USB Device with a single interface HID mouse
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

#include "usb_config.h"
#include "udc/udd.h"
#include "udc/udc_desc.h"
#include "class/hid/device/udi_hid.h"
#include "class/hid/device/gpd/udi_hid_gpd.h"
#include "class/cdc/device/udi_cdc.h"
#include "class/msc/device/udi_msc.h"

/**
 * USB device descriptors
 */

 /**
  * Description of Composite Device
  */
  //! USB Interfaces descriptor structure
#define	UDI_COMPOSITE_DESC_T \
	usb_iad_desc_t       udi_cdc_iad; \
	udi_cdc_comm_desc_t  udi_cdc_comm; \
	udi_cdc_data_desc_t  udi_cdc_data; \
	udi_msc_desc_t       udi_msc; \
	udi_hid_gpd_desc_t	 udi_hid_gpd;

//! USB Interfaces descriptor value for Full Speed
#define UDI_COMPOSITE_DESC_FS \
	.udi_cdc_iad   = UDI_CDC_IAD_DESC_0, \
	.udi_cdc_comm  = UDI_CDC_COMM_DESC_0, \
	.udi_cdc_data  = UDI_CDC_DATA_DESC_0_FS, \
	.udi_msc       = UDI_MSC_DESC_FS, \
	.udi_hid_gpd   = UDI_HID_GPD_DESC, \

//! USB Interfaces descriptor value for High Speed
#define UDI_COMPOSITE_DESC_HS \
	.udi_cdc_iad   = UDI_CDC_IAD_DESC_0, \
	.udi_cdc_comm  = UDI_CDC_COMM_DESC_0, \
	.udi_cdc_data  = UDI_CDC_DATA_DESC_0_HS, \
	.udi_msc       = UDI_MSC_DESC_HS, \
	.udi_hid_gpd   = UDI_HID_GPD_DESC,

//! USB Interface APIs
#define	UDI_COMPOSITE_API \
	&udi_api_cdc_comm, \
	&udi_api_cdc_data, \
	&udi_api_msc, \
	&udi_api_hid_gpd, 

//! USB Device Descriptor
__attribute__((__aligned__(4)))
UDC_DESC_STORAGE usb_dev_desc_t udc_device_desc = {
	.bLength                   = sizeof(usb_dev_desc_t),
	.bDescriptorType           = USB_DT_DEVICE,
	.bcdUSB                    = USB_V2_0,
	.bDeviceClass              = 0,
	.bDeviceSubClass           = 0,
	.bDeviceProtocol           = 0,
	.bMaxPacketSize0           = USB_DEVICE_EP_CTRL_SIZE,
	.idVendor                  = USB_DEVICE_VENDOR_ID,
	.idProduct                 = USB_DEVICE_PRODUCT_ID,
	.bcdDevice                 = (USB_DEVICE_MAJOR_VERSION << 8)
		| USB_DEVICE_MINOR_VERSION,
#ifdef USB_DEVICE_MANUFACTURE_NAME
	.iManufacturer             = 1,
#else
	.iManufacturer             = 0,  // No manufacture string
#endif
#ifdef USB_DEVICE_PRODUCT_NAME
	.iProduct                  = 2,
#else
	.iProduct                  = 0,  // No product string
#endif
#if (defined USB_DEVICE_SERIAL_NAME || defined USB_DEVICE_GET_SERIAL_NAME_POINTER)
	.iSerialNumber             = 3,
#else
	.iSerialNumber             = 0,  // No serial string
#endif
	.bNumConfigurations        = 1
};


#ifdef USB_DEVICE_HS_SUPPORT
//! USB Device Qualifier Descriptor for HS
__attribute__((__aligned__(4)))
UDC_DESC_STORAGE usb_dev_qual_desc_t udc_device_qual = {
	.bLength                   = sizeof(usb_dev_qual_desc_t),
	.bDescriptorType           = USB_DT_DEVICE_QUALIFIER,
	.bcdUSB                    = USB_V2_0,
	.bDeviceClass              = 0,
	.bDeviceSubClass           = 0,
	.bDeviceProtocol           = 0,
	.bMaxPacketSize0           = USB_DEVICE_EP_CTRL_SIZE,
	.bNumConfigurations        = 1
};
#endif

//! Structure for USB Device Configuration Descriptor
PACK(typedef struct {
	usb_conf_desc_t conf;
	UDI_COMPOSITE_DESC_T
}) udc_desc_t;

#ifdef USB_DEVICE_LPM_SUPPORT
//! USB Device Qualifier Descriptor
__attribute__((__aligned__(4)))
UDC_DESC_STORAGE usb_dev_lpm_desc_t udc_device_lpm = {
	.bos.bLength               = sizeof(usb_dev_bos_desc_t),
	.bos.bDescriptorType       = USB_DT_BOS,
	.bos.wTotalLength          = sizeof(usb_dev_bos_desc_t) + sizeof(usb_dev_capa_ext_desc_t),
	.bos.bNumDeviceCaps        = 1,
	.capa_ext.bLength          = sizeof(usb_dev_capa_ext_desc_t),
	.capa_ext.bDescriptorType  = USB_DT_DEVICE_CAPABILITY,
	.capa_ext.bDevCapabilityType = USB_DC_USB20_EXTENSION,
	.capa_ext.bmAttributes     = USB_DC_EXT_LPM,
};
#endif

//! USB Device Configuration Descriptor filled for FS
__attribute__((__aligned__(4)))
UDC_DESC_STORAGE udc_desc_t udc_desc_fs = {
	.conf.bLength			   = sizeof(usb_conf_desc_t),
	.conf.bDescriptorType	   = USB_DT_CONFIGURATION,
	.conf.wTotalLength		   = sizeof(udc_desc_t),
	.conf.bNumInterfaces	   = USB_DEVICE_NB_INTERFACE,
	.conf.bConfigurationValue  = 1,
	.conf.iConfiguration	   = 0,
	.conf.bmAttributes		   = USB_CONFIG_ATTR_MUST_SET | USB_DEVICE_ATTR,
	.conf.bMaxPower			   = USB_CONFIG_MAX_POWER(USB_DEVICE_POWER),
	UDI_COMPOSITE_DESC_FS
};

#ifdef USB_DEVICE_HS_SUPPORT
//! USB Device Configuration Descriptor filled for HS
__attribute__((__aligned__(4)))
UDC_DESC_STORAGE udc_desc_t udc_desc_hs = {
	.conf.bLength			   = sizeof(usb_conf_desc_t),
	.conf.bDescriptorType	   = USB_DT_CONFIGURATION,
	.conf.wTotalLength		   = sizeof(udc_desc_t),
	.conf.bNumInterfaces	   = USB_DEVICE_NB_INTERFACE,
	.conf.bConfigurationValue  = 1,
	.conf.iConfiguration	   = 0,
	.conf.bmAttributes		   = USB_CONFIG_ATTR_MUST_SET | USB_DEVICE_ATTR,
	.conf.bMaxPower			   = USB_CONFIG_MAX_POWER(USB_DEVICE_POWER),
	UDI_COMPOSITE_DESC_HS
};
#endif


/**
 * \name UDC structures which contains all USB Device definitions
 */
 //@{

 //! Associate an UDI for each USB interface
UDC_DESC_STORAGE udi_api_t* udi_apis[USB_DEVICE_NB_INTERFACE] = {
	UDI_COMPOSITE_API
};

//! Add UDI with USB Descriptors FS
UDC_DESC_STORAGE udc_config_speed_t   udc_config_lsfs[1] = { {
	.desc = (usb_conf_desc_t UDC_DESC_STORAGE*) & udc_desc_fs,
	.udi_apis = udi_apis,
} };

#ifdef USB_DEVICE_HS_SUPPORT
//! Add UDI with USB Descriptors HS
UDC_DESC_STORAGE udc_config_speed_t   udc_config_hs[1] = { {
	.desc = (usb_conf_desc_t UDC_DESC_STORAGE*) & udc_desc_hs,
	.udi_apis = udi_apis,
} };
#endif

//! Add all information about USB Device in global structure for UDC
UDC_DESC_STORAGE udc_config_t udc_config = {
	.confdev_lsfs = &udc_device_desc,
	.conf_lsfs = udc_config_lsfs,
#ifdef USB_DEVICE_HS_SUPPORT
	.confdev_hs = &udc_device_desc,
	.qualifier = &udc_device_qual,
	.conf_hs = udc_config_hs,
#endif
#ifdef USB_DEVICE_LPM_SUPPORT
	.conf_bos = &udc_device_lpm.bos,
#else
	.conf_bos = NULL,
#endif
};

