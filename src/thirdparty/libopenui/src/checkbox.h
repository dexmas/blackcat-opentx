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

#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include "form.h"

class CheckBox : public FormField {
  public:
    CheckBox(Window * parent, const rect_t & rect, std::function<uint8_t()> getValue, std::function<void(uint8_t)> setValue,
             LcdFlags flags = 0) :
      FormField(parent, rect),
      _getValue(getValue),
      _setValue(setValue)
    {
    }

#if defined(DEBUG_WINDOWS)
    std::string getName() override
    {
      return "CheckBox";
    }
#endif

    const char * getLabel() const
    {
      return label.c_str();
    }

    void setLabel(std::string newLabel)
    {
      label = std::move(newLabel);
    }

    void paint(BitmapBuffer * dc) override;

#if defined(HARDWARE_KEYS)
    void onEvent(event_t event) override;
#endif

#if defined(HARDWARE_TOUCH)
    bool onTouchEnd(coord_t x, coord_t y) override;
#endif

    uint8_t getValue() const
    {
      return _getValue();
    }

  protected:
    std::string label;
    std::function<uint8_t()> _getValue;
    std::function<void(uint8_t)> _setValue;
};

#endif // _CHECKBOX_H_
