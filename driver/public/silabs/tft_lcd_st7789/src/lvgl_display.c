/***************************************************************************//**
 * @file lvgl_display.c
 * @brief LVGL display driver source file
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include "sl_status.h"

#include "lvgl_display.h"

// #include "lv_conf.h"

// -----------------------------------------------------------------------------
//                       Local Function
// -----------------------------------------------------------------------------
static sl_status_t lvgl_driver_init(void);

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

static lvgl_display_t lvgl_display_instance;
static const lvgl_display_driver_api_t lvgl_driver_api =
{
  .init = lvgl_driver_init,
  .flush_area_rgb565 = adafruit_st7789_flush_area_rgb565,
};

static bool initialized = false;

sl_status_t lvgl_display_init(void)
{
  lvgl_display_instance.width = adafruit_st7789_get_display_width();
  lvgl_display_instance.height = adafruit_st7789_get_display_height();
  lvgl_display_instance.driver = &lvgl_driver_api;
  initialized = true;
  return SL_STATUS_OK;
}

lvgl_display_t *lvgl_display_get(void)
{
  if (initialized) {
    return &lvgl_display_instance;
  } else {
    return NULL;
  }
}

static sl_status_t lvgl_driver_init(void)
{
  return SL_STATUS_OK;
}
