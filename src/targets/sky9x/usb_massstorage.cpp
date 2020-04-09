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

#ifndef SIMU


void usbMassStorage()
{
  static bool initialized = false;

  if (sd_card_ready()) {
    TRACE_DEBUG("usbMassStorage\n\r");

    if (sdMounted()) {
      Card_state = SD_ST_DATA;
      audioQueue.stopSD();
      logsClose();
      f_mount(nullptr, "", 0); // unmount SD
    }

    if (!initialized) {
      /* Initialize LUN */
      //MEDSdcard_Initialize(&(medias[DRV_SDMMC]), 0);

      //LUN_Init(&(luns[DRV_SDMMC]), &(medias[DRV_SDMMC]),
      //    msdBuffer, MSD_BUFFER_SIZE,
      //    0, 0, 0, 0,
      //    MSDCallbacks_Data);

      /* BOT driver initialization */
      //MSDDriver_Initialize(luns, 1);

      // VBus_Configure();
      //USBD_Connect();

      initialized = true;
    }

    /* Mass storage state machine */
    //for (uint8_t i=0; i<50; i++)
    //  MSDDriver_StateMachine();
  }
  else {
    //msdReadTotal = 0;
    //msdWriteTotal = 0;
  }
}

#endif
