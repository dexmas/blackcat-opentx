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

#ifndef _NUMBEREDIT_H_
#define _NUMBEREDIT_H_

#include "basenumberedit.h"
#include <string>

class NumberEdit : public BaseNumberEdit {
  public:
    NumberEdit(Window * parent, const rect_t & rect, int32_t vmin, int32_t vmax,
               std::function<int32_t()> getValue, std::function<void(int32_t)> setValue = nullptr, LcdFlags flags = 0);

#if defined(DEBUG_WINDOWS)
    std::string getName() override
    {
      return "NumberEdit(" + std::to_string(getValue()) + ")";
    }
#endif

    void paint(BitmapBuffer * dc) override;

    void enable(bool enabled)
    {
      this->enabled = enabled;
    }

    void setAvailableHandler(std::function<bool(int)> handler)
    {
      isValueAvailable = std::move(handler);
    }

    void setPrefix(std::string value)
    {
      prefix = std::move(value);
    }

    void setSuffix(std::string value)
    {
      suffix = std::move(value);
    }

    void setZeroText(std::string value)
    {
      zeroText = std::move(value);
    }

    void setDisplayHandler(std::function<void(BitmapBuffer *, LcdFlags, int32_t)> function)
    {
      displayFunction = std::move(function);
    }

    void onEvent(event_t event) override;

#if defined(SOFTWARE_KEYBOARD)
    bool onTouchEnd(coord_t x, coord_t y) override;
#endif

    void onFocusLost() override;

  protected:
    std::function<void(BitmapBuffer *, LcdFlags, int32_t)> displayFunction;
    std::string prefix;
    std::string suffix;
    std::string zeroText;
    bool enabled = true;
    std::function<bool(int)> isValueAvailable;
};

#endif // _NUMBEREDIT_H_
