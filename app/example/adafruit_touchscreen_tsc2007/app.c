/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "app_log.h"
#include "app_assert.h"
#include "adafruit_ili9341.h"
#include "adafruit_tsc2007.h"
#include "glib.h"
#include "sl_i2cspm_instances.h"

#define PRESSURE_THRESH                 500

static glib_context_t g_context;
static bool touch_detected = false;

static void screen_print(
  int16_t glib_y_ofset,
  int32_t x, int32_t y, int32_t rtouch,
  uint16_t x_raw, uint16_t y_raw, uint16_t z1_raw, uint16_t z2_raw);
static void touch_detect_isr_callback(void);

void app_init(void)
{
  sl_status_t sc;

  sc = adafruit_ili9341_init();
  app_assert_status(sc);
  app_log("\rAdafruit ILI9341 Init done\n");

  sc = adafruit_tsc2007_init(sl_i2cspm_qwiic);
  app_assert_status(sc);
  app_log("\rTouch Screen Init done\n");

  adafruit_tsc2007_register_touch_detect_isr_callback(
    touch_detect_isr_callback);

  glib_init(&g_context);
  app_assert_status(sc);
  app_log("\rGLIB Init done\n");
  glib_set_bg_color(&g_context, ILI9341_BLACK);
  glib_set_text_color(&g_context, ILI9341_WHITE);
  glib_set_text_size(&g_context, 2, 2);
  glib_enable_wrap(&g_context, false);

  glib_fill(&g_context, ILI9341_BLACK);
  glib_draw_string(&g_context, "~TSC2007~", 65, 10);
  screen_print(40,
               0, 0, 0,
               0, 0, 0, 0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  int32_t x, y, rtouch;
  uint16_t x_raw, y_raw, z1_raw, z2_raw;

  if (touch_detected) {
    touch_detected = false;

    adafruit_tsc2007_read_touch();
    adafruit_tsc2007_get_touch(&x_raw, &y_raw, &z1_raw, &z2_raw);
    adafruit_tsc2007_get_touch_point(&x, &y, &rtouch);
    app_log("x: %ld, y: %ld, rtouch: %ld\r\n", x, y, rtouch);
    if (rtouch < PRESSURE_THRESH) {
      screen_print(40,
                   x, y, rtouch,
                   x_raw, y_raw, z1_raw, z2_raw);
    }
  }
}

static void screen_print(
  int16_t glib_y_ofset,
  int32_t x, int32_t y, int32_t rtouch,
  uint16_t x_raw, uint16_t y_raw, uint16_t z1_raw, uint16_t z2_raw)
{
  char string_buf[32];

  snprintf(string_buf, sizeof(string_buf), "X:  %3ld  ", x);
  glib_draw_string(&g_context, string_buf, 0, glib_y_ofset);
  snprintf(string_buf, sizeof(string_buf), "X_RAW:  %4d  ", x_raw);
  glib_draw_string(&g_context, string_buf, 95, glib_y_ofset);

  snprintf(string_buf, sizeof(string_buf), "Y:  %3ld", y);
  glib_draw_string(&g_context, string_buf, 0, glib_y_ofset + 20);
  snprintf(string_buf, sizeof(string_buf), "Y_RAW:  %4d", y_raw);
  glib_draw_string(&g_context, string_buf, 95, glib_y_ofset + 20);

  snprintf(string_buf, sizeof(string_buf), "RT: %3ld  ", rtouch);
  glib_draw_string(&g_context, string_buf, 0, glib_y_ofset + 40);
  snprintf(string_buf, sizeof(string_buf), "Z1_RAW: %4d  ", z1_raw);
  glib_draw_string(&g_context, string_buf, 95, glib_y_ofset + 40);
  snprintf(string_buf, sizeof(string_buf), "Z2_RAW: %4d  ", z2_raw);
  glib_draw_string(&g_context, string_buf, 95, glib_y_ofset + 60);
}

static void touch_detect_isr_callback(void)
{
  touch_detected = true;
}
