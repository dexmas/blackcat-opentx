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

extern "C" {
    extern "C" void startJoystick(void);
    extern "C" uint8_t HIDDJoystickDriver_Change(uint8_t * data);
    extern "C" void USBD_Connect(void);
    extern "C" void USBD_Disconnect(void);
}

void usbJoystickUpdate()
{
    static bool usbStarted = false;
    static bool usbInited = false;

    if (!usbStarted && usbPlugged()) {
        if (!usbInited) {
            TRACE_DEBUG("usbJoystick init\n\r");
            startJoystick();
            usbInited = true;
        }

        TRACE_DEBUG("usbJoystick start\n\r");
        USBD_Connect();
        usbStarted = true;
    }

    if (usbStarted && !usbPlugged()) {
        TRACE_DEBUG("usbJoystick stop\n\r");
        USBD_Disconnect();
        usbStarted = false;
    }

    if (usbStarted) {
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
}
