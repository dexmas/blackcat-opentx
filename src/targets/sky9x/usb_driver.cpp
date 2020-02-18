/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"
#include "debug.h"
#include "usb_driver.h"

#ifndef SIMU

void usbMassStorage();
void startCDCSerial();

extern "C" {
    extern "C" void startJoystick(void);
    extern "C" uint8_t HIDDJoystickDriver_Change(uint8_t * data);
    extern "C" void USBD_Connect(void);
    extern "C" void USBD_Disconnect(void);
}

static bool usbDriverStarted = false;
#if defined(BOOT)
static usbMode selectedUsbMode = USB_MASS_STORAGE_MODE;
#else
static usbMode selectedUsbMode = USB_UNSELECTED_MODE;
#endif

int getSelectedUsbMode()
{
    return selectedUsbMode;
}

void setSelectedUsbMode(int mode)
{
    selectedUsbMode = usbMode(mode);
}

int usbPlugged()
{
    return (PIOC->PIO_PDSR & PIO_PC25);
}

void usbInit()
{
    // Initialize hardware
    //USB_OTG_BSP_Init(&USB_OTG_dev);
    usbDriverStarted = false;
}

void usbStart()
{
    switch (getSelectedUsbMode()) {
#if !defined(BOOT)
    case USB_JOYSTICK_MODE:
        startJoystick();
        break;
#endif
#if defined(USB_SERIAL)
    case USB_SERIAL_MODE:
        startCDCSerial();
        break;
#endif
    default:
    case USB_MASS_STORAGE_MODE:
        usbMassStorage();
        break;
    }
    usbDriverStarted = true;
}

void usbStop()
{
    usbDriverStarted = false;
    USBD_Disconnect();
    //USBD_DeInit(&USB_OTG_dev);
}

bool usbStarted()
{
    return usbDriverStarted;
}

void usbJoystickUpdate()
{
    static uint8_t HID_Buffer[9];

    HID_Buffer[0] = 0; // buttons
    for (int i = 0; i < 8; ++i) {
        if ( channelOutputs[i+8] > 0 ) {
            HID_Buffer[0] |= (1 << i);
        }
    }

    //analog values
    for (int i = 0; i < 8; ++i) {
        int16_t value = channelOutputs[i] / 8;
        if ( value > 127 ) value = 127;
        else if ( value < -127 ) value = -127;
        HID_Buffer[i+1] = static_cast<int8_t>(value);
    }

    HIDDJoystickDriver_Change(HID_Buffer);
}

void usbPluggedIn()
{
    //#TODO
}

#endif