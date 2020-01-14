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

#ifndef _KEYBOARD_NUMBER_H_
#define _KEYBOARD_NUMBER_H_

#include "keyboard_base.h"

class NumberKeyboard : public Keyboard {
  public:
    NumberKeyboard();

    ~NumberKeyboard() override;

#if defined(DEBUG_WINDOWS)
    std::string getName() override
    {
      return "NumberKeyboard";
    }
#endif

    static void show(FormField * field)
    {
      if (!_instance)
        _instance = new NumberKeyboard();
      _instance->setField(field);
    }

    void paint(BitmapBuffer * dc) override;

  protected:
    static NumberKeyboard * _instance;
};

#endif // _KEYBOARD_NUMBER_H_