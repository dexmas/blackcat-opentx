/*
 * Copyright (C) OpenTX
 *
 * Source:
 *  https://github.com/opentx/libopenui
 *
 * This file is a part of libopenui library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifndef _LIBOPEUI_GLOBALS_H_
#define _LIBOPEUI_GLOBALS_H_

#include <inttypes.h>
#include "libopenui_config.h"

extern uint16_t lcdColorTable[LCD_COLOR_COUNT];
extern const uint16_t * const fontspecsTable[FONTS_COUNT];
extern const uint8_t * fontsTable[FONTS_COUNT];

inline void lcdSetColor(uint16_t color)
{
  lcdColorTable[CUSTOM_COLOR_INDEX] = color;
}

#endif // _LIBOPEUI_GLOBALS_H_
