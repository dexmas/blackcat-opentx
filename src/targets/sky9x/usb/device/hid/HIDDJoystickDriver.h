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

#ifndef HIDDJOYSTICKDRIVER_H
#define HIDDJOYSTICKDRIVER_H

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/

#include "usb/common/core/USBRequests.h"

#include "usb/common/hid/HIDDescriptors.h"
#include "usb/common/hid/HIDRequests.h"

#include "usb/device/core/USBDDriver.h"

/*------------------------------------------------------------------------------
 *         Definitions
 *------------------------------------------------------------------------------*/


/** Size of the report descriptor in bytes. */
#define HIDDJoystickDriver_REPORTDESCRIPTORSIZE              54

/*------------------------------------------------------------------------------
 *         Types
 *------------------------------------------------------------------------------*/

#ifdef __ICCARM__          /* IAR */
#pragma pack(1)            /* IAR */
#define __attribute__(...) /* IAR */
#endif                     /* IAR */

typedef struct 
{
    /** Configuration descriptor. */
    USBConfigurationDescriptor configuration;
    /** Interface descriptor. */
    USBInterfaceDescriptor interface;
    /** HID descriptor. */
    HIDDescriptor1 hid;
    /** Interrupt IN endpoint descriptor. */
    USBEndpointDescriptor interruptIn;

} __attribute__ ((packed)) HIDDJoystickConfigurationDescriptors;

typedef struct
{
    uint8_t bmButtons;          /**< Bitmap state of 8 buttons. */
		int8_t analog[8] ;
} __attribute__ ((packed)) HIDDJoystickInputReport; /* GCC */

#ifdef __ICCARM__          /* IAR */
#pragma pack()             /* IAR */
#endif                     /* IAR */

/*------------------------------------------------------------------------------
 *         Exported functions
 *------------------------------------------------------------------------------*/

extern void HIDDJoystickDriver_Initialize(const USBDDriverDescriptors *pDescriptors);

extern void HIDDJoystickDriver_ConfigurationChangedHandler(uint8_t cfgnum);

extern void HIDDJoystickDriver_RequestHandler(const USBGenericRequest *request);

extern void HIDDJoystickDriver_RemoteWakeUp(void);

/**@}*/

#endif /*#ifndef HIDDJOYSTICKDRIVER_H */

