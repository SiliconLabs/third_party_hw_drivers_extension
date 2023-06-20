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
#include "mikroe_ssd1351_config.h"
#include "oled_display.h"

#define FRAME_HEADER          6
#define FRAME_BUFFER_SIZE     SSD1351_DISPLAY_HEIGHT *SSD1351_DISPLAY_WIDTH \
  *SSD1351_DISPLAY_COLOR / 8 + FRAME_HEADER

/* This oled_frame_buffer is large enough to store one full frame. */
static uint8_t oled_frame_buffer[18438];
static oled_display_t oled_display_instance;

/** Flag to monitor is this driver has been initialized. The
 *   oled_display_instance
 *  is only valid after initialized=true. */
static bool initialized = false;
// -----------------------------------------------------------------------------
// Static Function Declarations
// -----------------------------------------------------------------------------
static sl_status_t driver_init(void);
static sl_status_t draw_pixel(int16_t x, int16_t y, uint16_t color);
static uint16_t get_raw_pixel(int16_t x, int16_t y);
static sl_status_t enable_display(bool state);
static sl_status_t set_contrast(uint8_t contrast);
static sl_status_t fill_screen(uint16_t color);
static sl_status_t update_display(void);

// -----------------------------------------------------------------------------
// Public Function Definition
// -----------------------------------------------------------------------------
static const oled_display_driver_api_t sl_memlcd_driver_api =
{
  .init = driver_init,
  .draw_pixel = draw_pixel,
  .get_raw_pixel = get_raw_pixel,
  .fill_screen = fill_screen,
  .update_display = update_display,
  .enable_display = enable_display,
  .set_invert_color = NULL,
  .set_normal_color = NULL,
  .set_contrast = set_contrast,
  .scroll_right = NULL,
  .scroll_left = NULL,
  .scroll_diag_right = NULL,
  .scroll_diag_left = NULL,
  .stop_scroll = NULL,
};

sl_status_t oled_display_init(void)
{
  oled_display_instance.width = SSD1351_DISPLAY_WIDTH;
  oled_display_instance.height = SSD1351_DISPLAY_HEIGHT;
  oled_display_instance.driver = &sl_memlcd_driver_api;
  oled_frame_buffer[0] = 0x00;
  oled_frame_buffer[1] = 0x10;
  oled_frame_buffer[2] = 0x60;
  oled_frame_buffer[3] = 0x00;
  oled_frame_buffer[4] = 0x60;
  oled_frame_buffer[5] = 0x00;
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

// -----------------------------------------------------------------------------
// Static Function Definition
// -----------------------------------------------------------------------------
static sl_status_t driver_init(void)
{
  return mikroe_ssd1351_default_cfg();
}

static sl_status_t draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x > OLEDC_SCREEN_WIDTH) || (y > OLEDC_SCREEN_HEIGHT)) {
    return SL_STATUS_FAIL;
  }

  size_t index = FRAME_HEADER + 2 * (y * SSD1351_DISPLAY_WIDTH + x);

  oled_frame_buffer[index] = color >> 8;
  oled_frame_buffer[index + 1] = color & 0x00FF;

  return SL_STATUS_OK;
}

static uint16_t get_raw_pixel(int16_t x, int16_t y)
{
  uint8_t index = y * SSD1351_DISPLAY_WIDTH * 2 + x * 2 + FRAME_HEADER;
  return (uint16_t)(oled_frame_buffer[index] << 8)
         | oled_frame_buffer[index + 1];
}

static sl_status_t fill_screen(uint16_t color)
{
  oled_frame_buffer[0] = 0x00;
  oled_frame_buffer[1] = 0x10;
  oled_frame_buffer[2] = 0x60;
  oled_frame_buffer[3] = 0x00;
  oled_frame_buffer[4] = 0x60;
  oled_frame_buffer[5] = 0x00;

  /* Fill the display with the color of the glib_context_t  */
  for (size_t i = 0; i < 96 * 96; i++) {
    oled_frame_buffer[FRAME_HEADER + i * 2] = color >> 8;
    oled_frame_buffer[FRAME_HEADER + i * 2 + 1] = color & 0x00FF;
  }
  return SL_STATUS_OK;
}

static sl_status_t update_display(void)
{
  return mikroe_ssd1351_image(oled_frame_buffer, 0, 0);
}

static sl_status_t set_contrast(uint8_t contrast)
{
  return mikroe_ssd1351_set_contrast(contrast);
}

sl_status_t enable_display(bool state)
{
  return mikroe_ssd1351_enable(state);
}
