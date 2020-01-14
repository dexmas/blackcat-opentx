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

#ifndef _CHOICE_H_
#define _CHOICE_H_

#include "form.h"

enum ChoiceType {
  CHOICE_TYPE_DROPOWN,
  CHOICE_TYPE_FOLDER,
};

class ChoiceBase : public FormField {
  public:
    ChoiceBase(FormGroup * parent, const rect_t & rect, ChoiceType type = CHOICE_TYPE_DROPOWN):
      FormField(parent, rect),
      type(type)
    {
    }

    inline ChoiceType getType() const
    {
      return type;
    }

  protected:
    ChoiceType type;
};

class Choice : public ChoiceBase {
  public:
    Choice(FormGroup * parent, const rect_t & rect, const char * values, int16_t vmin, int16_t vmax, std::function<int16_t()> getValue, std::function<void(int16_t)> setValue = nullptr);

#if defined(DEBUG_WINDOWS)
    std::string getName() override
    {
      return "Choice";
    }
#endif

    void paint(BitmapBuffer * dc) override;

#if defined(HARDWARE_KEYS)
    void onEvent(event_t event) override;
#endif

#if defined(HARDWARE_TOUCH)
    bool onTouchEnd(coord_t x, coord_t y) override;
#endif

    void setSetValueHandler(std::function<void(int16_t)> handler)
    {
      setValue = std::move(handler);
    }

    void setAvailableHandler(std::function<bool(int)> handler)
    {
      isValueAvailable = std::move(handler);
    }

    void setTextHandler(std::function<std::string(int32_t)> handler)
    {
      textHandler = std::move(handler);
    }

    void setMenuTitle(const std::string value)
    {
      menuTitle = std::move(value);
    }

  protected:
    const char * values = nullptr;
    int16_t vmin = 0;
    int16_t vmax = 0;
    std::string menuTitle;
    std::function<int16_t()> getValue;
    std::function<void(int16_t)> setValue;
    std::function<bool(int)> isValueAvailable;
    std::function<std::string(int32_t)> textHandler;

    virtual void openMenu();
};

#endif // _CHOICE_H_
