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
#include <string.h>
#include "mikroe_ssd1306_config.h"
#include "mikroe_ssd1306.h"
#include "oled_display.h"

// This oled_frame_buffer is large enough to store one full frame.
static uint8_t oled_frame_buffer[(SSD1306_DISPLAY_HEIGHT + 7) / 8
                                 * SSD1306_DISPLAY_WIDTH];

static sl_status_t driver_init(void);
static sl_status_t draw_pixel(int16_t x, int16_t y, uint16_t color);
static uint16_t get_raw_pixel(int16_t x, int16_t y);
static sl_status_t fill_screen(uint16_t color);
static sl_status_t update_display(void);

static oled_display_t oled_display_instance;
static const oled_display_driver_api_t sl_memlcd_driver_api =
{
  .init = driver_init,
  .draw_pixel = draw_pixel,
  .get_raw_pixel = get_raw_pixel,
  .fill_screen = fill_screen,
  .update_display = update_display,
  .enable_display = mikroe_ssd1306_enable_display,
  .set_invert_color = mikroe_ssd1306_set_invert_color,
  .set_normal_color = mikroe_ssd1306_set_normal_color,
  .set_contrast = mikroe_ssd1306_set_contrast,
  .scroll_right = mikroe_ssd1306_scroll_right,
  .scroll_left = mikroe_ssd1306_scroll_left,
  .scroll_diag_right = mikroe_ssd1306_scroll_diag_right,
  .scroll_diag_left = mikroe_ssd1306_scroll_diag_left,
  .stop_scroll = mikroe_ssd1306_stop_scroll,
};

/**
 *  Flag to monitor is this driver has been initialized.
 *  The oled_display_instance is only valid after initialized=true.
 */
static bool initialized = false;

sl_status_t oled_display_init(void)
{
  oled_display_instance.width = SSD1306_DISPLAY_WIDTH;
  oled_display_instance.height = SSD1306_DISPLAY_HEIGHT;
  oled_display_instance.driver = &sl_memlcd_driver_api;
  initialized = true;
  return SL_STATUS_OK;
}

static sl_status_t draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  if (color) {
    oled_frame_buffer[x + (y / 8) * SSD1306_DISPLAY_WIDTH] |= 1 << (y % 8);
  } else {
    oled_frame_buffer[x + (y / 8)
                      * SSD1306_DISPLAY_WIDTH] &= ~(1 << (y % 8));
  }
  return SL_STATUS_OK;
}

static uint16_t get_raw_pixel(int16_t x, int16_t y)
{
  return (oled_frame_buffer[x + (y / 8) * SSD1306_DISPLAY_WIDTH]
          & (1 << (y % 8))) == 0 ? 0x0000 : 0xffff;
}

static sl_status_t fill_screen(uint16_t color)
{
  uint16_t i;

  // Fill the display with the background color of the glib_context_t
  for (i = 0; i < sizeof(oled_frame_buffer); i++) {
    oled_frame_buffer[i] = color == 0 ? 0x00 : 0xFF;
  }
  return SL_STATUS_OK;
}

static sl_status_t update_display(void)
{
  return mikroe_ssd1306_draw(oled_frame_buffer);
}

const oled_display_t *oled_display_get(void)
{
  if (initialized) {
    return &oled_display_instance;
  } else {
    return NULL;
  }
}

static sl_status_t driver_init(void)
{
  return mikroe_ssd1306_default_cfg();
}
