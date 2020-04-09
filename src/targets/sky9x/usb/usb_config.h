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

#include "board_lowlevel.h"

/**
 * USB Device Configuration
 * @{
 */

//! Device definition (mandatory)
#define  USB_DEVICE_VENDOR_ID             0x03EB
#define  USB_DEVICE_PRODUCT_ID            0x2400
#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         0
#define  USB_DEVICE_POWER                 100 // Consumption on Vbus line (mA)
#define  USB_DEVICE_ATTR                  \
	(USB_CONFIG_ATTR_REMOTE_WAKEUP|USB_CONFIG_ATTR_BUS_POWERED)
//	(USB_CONFIG_ATTR_REMOTE_WAKEUP|USB_CONFIG_ATTR_SELF_POWERED)
//	(USB_CONFIG_ATTR_REMOTE_WAKEUP|USB_CONFIG_ATTR_BUS_POWERED)
//	(USB_CONFIG_ATTR_SELF_POWERED)
//	(USB_CONFIG_ATTR_BUS_POWERED)

//! USB Device string definitions (Optional)
#define  USB_DEVICE_MANUFACTURE_NAME      "ATMEL ASF"
#define  USB_DEVICE_PRODUCT_NAME          "HID Mouse"
// #define  USB_DEVICE_SERIAL_NAME           "12...EF"

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
#define  UDC_SOF_EVENT()                  main_sof_action()
#define  UDC_SUSPEND_EVENT()              main_suspend_action()
#define  UDC_RESUME_EVENT()               main_resume_action()
//! Mandatory when USB_DEVICE_ATTR authorizes remote wakeup feature
#define  UDC_REMOTEWAKEUP_ENABLE()        main_remotewakeup_enable()
#define  UDC_REMOTEWAKEUP_DISABLE()       main_remotewakeup_disable()
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
 * USB Interface Configuration
 * @{
 */
/**
 * Configuration of HID Mouse interface
 * @{
 */
//! Interface callback definition
#define  UDI_HID_MOUSE_ENABLE_EXT()       main_mouse_enable()
#define  UDI_HID_MOUSE_DISABLE_EXT()      main_mouse_disable()
//@}
//@}


/**
 * USB Device Driver Configuration
 * @{
 */
//@}

//! Control endpoint size
#ifdef USB_DEVICE_HS_SUPPORT
#  define  USB_DEVICE_EP_CTRL_SIZE       64
#else
#  define  USB_DEVICE_EP_CTRL_SIZE       8
#endif


//! Endpoint number used by HID mouse interface
#define  UDI_HID_MOUSE_EP_IN            (1 | USB_EP_DIR_IN)

//! Interface number
#define  UDI_HID_MOUSE_IFACE_NUMBER     0


/**
 * \name UDD Configuration
 */
 //@{
#undef USB_DEVICE_MAX_EP   // undefine this definition in header file
//! 1 endpoint used by HID mouse standard interface
#define  USB_DEVICE_MAX_EP             1

#define Max(a, b)           (((a) > (b)) ?  (a) : (b))

/*! \name Bit-Field Handling
 */
 //! @{

 /*! \brief Reads the bits of a value specified by a given bit-mask.
  *
  * \param value Value to read bits from.
  * \param mask  Bit-mask indicating bits to read.
  *
  * \return Read bits.
  */
#define Rd_bits( value, mask)        ((value) & (mask))

  /*! \brief Writes the bits of a C lvalue specified by a given bit-mask.
   *
   * \param lvalue  C lvalue to write bits to.
   * \param mask    Bit-mask indicating bits to write.
   * \param bits    Bits to write.
   *
   * \return Resulting value with written bits.
   */
#define Wr_bits(lvalue, mask, bits)  ((lvalue) = ((lvalue) & ~(mask)) |\
                                                 ((bits  ) &  (mask)))

    /*! \brief Clears the bits of a C lvalue specified by a given bit-mask.
     *
     * \param lvalue  C lvalue of which to clear bits.
     * \param mask    Bit-mask indicating bits to clear.
     *
     * \return Resulting value with cleared bits.
     */
#define Clr_bits(lvalue, mask)  ((lvalue) &= ~(mask))

     /*! \brief Sets the bits of a C lvalue specified by a given bit-mask.
      *
      * \param lvalue  C lvalue of which to set bits.
      * \param mask    Bit-mask indicating bits to set.
      *
      * \return Resulting value with set bits.
      */
#define Set_bits(lvalue, mask)  ((lvalue) |=  (mask))



#endif // _CONF_USB_H_
