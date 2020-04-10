/**
 * \file
 *
 * \brief USB configuration file
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "board.h"

/**
 * USB Device Configuration
 * @{
 */

//! Device definition (mandatory)
#define  USB_DEVICE_VENDOR_ID             0x03EB
#define  USB_DEVICE_PRODUCT_ID            0x6200
#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         1
#define  USB_DEVICE_POWER                 100 // Consumption on Vbus line (mA)
#define  USB_DEVICE_ATTR                  (USB_CONFIG_ATTR_REMOTE_WAKEUP|USB_CONFIG_ATTR_SELF_POWERED)


//! USB Device string definitions (Optional)
#define  USB_DEVICE_MANUFACTURE_NAME      "OpenTX"
#define  USB_DEVICE_PRODUCT_NAME          "9XR Pro"
#define  USB_DEVICE_SERIAL_NAME           "OpenTX 2.3.3"

/**
 * Device speeds support
 * @{
 */
//! To define a Low speed device
//#define  USB_DEVICE_LOW_SPEED

//! To authorize the High speed
#if (UC3A3 || UC3A4)
//#define  USB_DEVICE_HS_SUPPORT
#elif (SAM3XA || SAM3U)
//#define  USB_DEVICE_HS_SUPPORT
#elif (SAMV71 || SAMV70 || SAMS70 || SAME70)
//#define  USB_DEVICE_HS_SUPPORT
#endif
//@}

/**
 * USB Device Callbacks definitions (Optional)
 * @{
 */
#define  UDC_VBUS_EVENT(b_vbus_high)
#define  UDC_SOF_EVENT()                  
#define  UDC_SUSPEND_EVENT()              
#define  UDC_RESUME_EVENT()               
//! Mandatory when USB_DEVICE_ATTR authorizes remote wakeup feature
#define  UDC_REMOTEWAKEUP_ENABLE()        
#define  UDC_REMOTEWAKEUP_DISABLE()       
#ifdef USB_DEVICE_LPM_SUPPORT
#define  UDC_SUSPEND_LPM_EVENT()          main_suspend_lpm_action()
#define  UDC_REMOTEWAKEUP_LPM_ENABLE()    main_remotewakeup_lpm_enable()
#define  UDC_REMOTEWAKEUP_LPM_DISABLE()   main_remotewakeup_lpm_disable()
#endif
//! When a extra string descriptor must be supported
//! other than manufacturer, product and serial string
// #define  UDC_GET_EXTRA_STRING()
//@}

//@}

/**
 * USB Device Driver Configuration
 * @{
 */
 /* ! Interface callback definition */
//#TODO DeXmas
#define  UDI_HID_GPD_ENABLE_EXT()       true
#define  UDI_HID_GPD_DISABLE_EXT()      
#define  UDI_HID_GPD_CHANGE_LED(value)

/* ! Interface number */
#define  UDI_HID_GPD_IFACE_NUMBER    0

/* ! Endpoint numbers definition */
#define  UDI_HID_GPD_EP_IN           (1 | USB_EP_DIR_IN)
//@}

//! Control endpoint size
#ifdef USB_DEVICE_HS_SUPPORT
#  define  USB_DEVICE_EP_CTRL_SIZE       64
#else
#  define  USB_DEVICE_EP_CTRL_SIZE       8
#endif

//! Define two USB communication ports
#define  UDI_CDC_PORT_NB 1

//! Interface callback definition
//#TODO DeXmas
#define  UDI_CDC_ENABLE_EXT(port)         true
#define  UDI_CDC_DISABLE_EXT(port)        
#define  UDI_CDC_RX_NOTIFY(port)          
#define  UDI_CDC_TX_EMPTY_NOTIFY(port)
#define  UDI_CDC_SET_CODING_EXT(port,cfg) 
#define  UDI_CDC_SET_DTR_EXT(port,set)    
#define  UDI_CDC_SET_RTS_EXT(port,set)

//! Define it when the transfer CDC Device to Host is a low rate (<512000 bauds)
//! to reduce CDC buffers size
//#define  UDI_CDC_LOW_RATE

//! Default configuration of communication port
#define  UDI_CDC_DEFAULT_RATE             115200
#define  UDI_CDC_DEFAULT_STOPBITS         CDC_STOP_BITS_1
#define  UDI_CDC_DEFAULT_PARITY           CDC_PAR_NONE
#define  UDI_CDC_DEFAULT_DATABITS         8

/**
 * USB CDC low level configuration
 * In standalone these configurations are defined by the CDC module.
 * For composite device, these configuration must be defined here
 * @{
 */
 //! Endpoint numbers definition
#define  UDI_CDC_COMM_EP_0             (7 | USB_EP_DIR_IN)  // Notify endpoint
#define  UDI_CDC_DATA_EP_IN_0          (6 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_0         (5 | USB_EP_DIR_OUT) // RX

//! Interface numbers
#define  UDI_CDC_COMM_IFACE_NUMBER_0   0
#define  UDI_CDC_DATA_IFACE_NUMBER_0   1

/**
 * \name UDD Configuration
 */
 //@{
#undef USB_DEVICE_MAX_EP   // undefine this definition in header file
//! 1 endpoint used by HID mouse standard interface
#define  USB_DEVICE_MAX_EP             1

static inline uint32_t pmc_enable_periph_clk(uint32_t ul_id)
{
    if (ul_id < 32) {
        if ((PMC->PMC_PCSR0 & (1u << ul_id)) != (1u << ul_id)) {
            PMC->PMC_PCER0 = 1 << ul_id;
        }
    }
    else {
        ul_id -= 32;
        if ((PMC->PMC_PCSR1 & (1u << ul_id)) != (1u << ul_id)) {
            PMC->PMC_PCER1 = 1 << ul_id;
        }
    }

    return 0;
}

static inline uint32_t pmc_disable_periph_clk(uint32_t ul_id)
{
    if (ul_id < 32) {
        if ((PMC->PMC_PCSR0 & (1u << ul_id)) == (1u << ul_id)) {
            PMC->PMC_PCDR0 = 1 << ul_id;
        }
    }
    else {
        ul_id -= 32;
        if ((PMC->PMC_PCSR1 & (1u << ul_id)) == (1u << ul_id)) {
            PMC->PMC_PCDR1 = 1 << ul_id;
        }
    }
    return 0;
}

static inline void sysclk_enable_usb(void)
{
    //#TODO DeXmas: WRONG PLL SOURCE!
    //#TODO DeXmas: PLL Routine to board_lowlwvel
    PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) |
        CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN |
        CKGR_MOR_MOSCXTST(((15625 * CHIP_FREQ_SLCK_RC / 8 / 1000000) < 0x100 ? (15625 * CHIP_FREQ_SLCK_RC / 8 / 1000000) : 0xFF));
    /* Wait the Xtal to stabilize */
    while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));

    PMC->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;

    while (!(PMC->PMC_SR & PMC_SR_MOSCSELS)) {
        /* Do nothing */
    }

    /* PMC hardware will automatically make it mul+1 */
    uint32_t ctrl = CKGR_PLLAR_MULA(16 - 1) | CKGR_PLLAR_DIVA(2) | CKGR_PLLAR_PLLACOUNT(0x3fU);

    PMC->CKGR_PLLBR = CKGR_PLLBR_MULB(0);
    PMC->CKGR_PLLBR = ctrl;

    while (!(PMC->PMC_SR & PMC_SR_LOCKB));

    PMC->PMC_USB = PMC_USB_USBDIV(2 - 1) | PMC_USB_USBS;

    PMC->PMC_SCER = PMC_SCER_UDP;

    return;
}

#endif // _CONF_USB_H_
