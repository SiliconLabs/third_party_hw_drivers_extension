/***************************************************************************//**
 * @file oled_display.c
 * @brief OLED Driver API interface
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
#include <stdbool.h>
#include "sl_status.h"
#include "oled_display.h"

static sl_status_t adafruit_hxd8357d_driver_init(void);
static sl_status_t adafruit_hxd8357d_set_invert_color(void);
static sl_status_t adafruit_hxd8357d_set_normal_color(void);
static sl_status_t adafruit_hxd8357d_scroll_right(uint8_t start_page_addr,
                                                  uint8_t end_page_addr);
static sl_status_t adafruit_hxd8357d_scroll_left(uint8_t start_page_addr,
                                                 uint8_t end_page_addr);
static sl_status_t adafruit_hxd8357d_scroll_diag_right(
  uint8_t start_page_addr,
  uint8_t end_page_addr);
static sl_status_t adafruit_hxd8357d_scroll_diag_left(uint8_t start_page_addr,
                                                      uint8_t end_page_addr);
static sl_status_t adafruit_hxd8357d_stop_scroll(void);
static sl_status_t adafruit_hxd8357d_set_contrast(uint8_t contrast);
static sl_status_t adafruit_hxd8357d_update_display();
static sl_status_t adafruit_hxd8357d_enable_display(bool en);

static oled_display_t oled_display_instance;
static const oled_display_driver_api_t sl_memlcd_driver_api =
{
  .init = adafruit_hxd8357d_driver_init,
  .draw_pixel = adafruit_hxd8357d_draw_pixel,
  .get_raw_pixel = NULL,
  .fill_screen = adafruit_hxd8357d_fill_screen,
  .update_display = adafruit_hxd8357d_update_display,
  .enable_display = adafruit_hxd8357d_enable_display,
  .set_invert_color = adafruit_hxd8357d_set_invert_color,
  .set_normal_color = adafruit_hxd8357d_set_normal_color,
  .set_contrast = adafruit_hxd8357d_set_contrast,
  .scroll_right = adafruit_hxd8357d_scroll_right,
  .scroll_left = adafruit_hxd8357d_scroll_left,
  .scroll_diag_right = adafruit_hxd8357d_scroll_diag_right,
  .scroll_diag_left = adafruit_hxd8357d_scroll_diag_left,
  .stop_scroll = adafruit_hxd8357d_stop_scroll,
};

static bool initialized = false;

sl_status_t oled_display_init(void)
{
  oled_display_instance.width = HXD8357D_TFTWIDTH;
  oled_display_instance.height = HXD8357D_TFTHEIGHT;
  oled_display_instance.driver = &sl_memlcd_driver_api;
  initialized = true;
  return SL_STATUS_OK;
}

const oled_display_t *oled_display_get(void)
{
  if (initialized) {
    return &oled_display_instance;
  } else {
    return NULL;
  }
}

static sl_status_t adafruit_hxd8357d_driver_init(void)
{
  return SL_STATUS_OK;
}

static sl_status_t adafruit_hxd8357d_set_invert_color(void)
{
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t adafruit_hxd8357d_set_normal_color(void)
{
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t adafruit_hxd8357d_scroll_right(uint8_t start_page_addr,
                                                  uint8_t end_page_addr)
{
  (void)start_page_addr;
  (void)end_page_addr;
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t adafruit_hxd8357d_scroll_left(uint8_t start_page_addr,
                                                 uint8_t end_page_addr)
{
  (void)start_page_addr;
  (void)end_page_addr;
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t adafruit_hxd8357d_scroll_diag_right(
  uint8_t start_page_addr,
  uint8_t end_page_addr)
{
  (void)start_page_addr;
  (void)end_page_addr;
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t adafruit_hxd8357d_scroll_diag_left(uint8_t start_page_addr,
                                                      uint8_t end_page_addr)
{
  (void)start_page_addr;
  (void)end_page_addr;
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t adafruit_hxd8357d_stop_scroll(void)
{
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t adafruit_hxd8357d_set_contrast(uint8_t contrast)
{
  (void)contrast;
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t adafruit_hxd8357d_update_display(void)
{
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t adafruit_hxd8357d_enable_display(bool en)
{
  (void)en;
  return SL_STATUS_NOT_SUPPORTED;
}
