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
#include "touch_screen.h"
#include "glib.h"
#if defined(SL_CATALOG_ADAFRUIT_TFT_LCD_ILI9341_DMA_PRESENT)
#include "sl_spidrv_instances.h"
#endif

#define PRESSURE_THRESH                 25

static glib_context_t g_context;
static touch_point_t point;
static uint16_t color = ILI9341_YELLOW;
static uint8_t rotation = 0;

static void draw_main_screen(void)
{
  glib_fill(&g_context, ILI9341_BLACK);
  glib_fill_rect(&g_context, 0, g_context.height - 30, 30, 30, ILI9341_YELLOW);
  glib_fill_rect(&g_context, 30, g_context.height - 30, 30, 30, ILI9341_BLUE);
  glib_fill_rect(&g_context, 60, g_context.height - 30, 30, 30, ILI9341_GREEN);
  glib_fill_rect(&g_context, 90, g_context.height - 30, 30, 30, ILI9341_RED);
  glib_fill_rect(&g_context, 120, g_context.height - 30, 30, 30, ILI9341_WHITE);
  glib_fill_rect(&g_context, 150, g_context.height - 30, 30, 30, ILI9341_PINK);
  glib_fill_rect(&g_context, 180, g_context.height - 30, 30, 30,
                 ILI9341_PURPLE);
  glib_fill_rect(&g_context, 210, g_context.height - 30, 30, 30,
                 ILI9341_ORANGE);

  glib_fill_rect(&g_context, 0, 0, 10, 10, color);
  glib_draw_string(&g_context, "CLR", g_context.width - 37, 5);
  glib_draw_rect(&g_context, g_context.width - 40, 0, 40, 25, ILI9341_WHITE);
}

void app_init(void)
{
  sl_status_t sc;

#if defined(SL_CATALOG_ADAFRUIT_TFT_LCD_ILI9341_PRESENT)
  sc = adafruit_ili9341_init();
#elif defined(SL_CATALOG_ADAFRUIT_TFT_LCD_ILI9341_DMA_PRESENT)
  sc = adafruit_ili9341_init(sl_spidrv_ili9341_handle);
#endif
  app_assert_status(sc);
  app_log("\rAdafruit ILI9341 Init done\n");

  sc = touch_screen_init();
  app_assert_status(sc);
  app_log("\rTouch Screen Init done\n");

  glib_init(&g_context);
  app_assert_status(sc);
  app_log("\rGLIB Init done\n");
  g_context.bg_color = ILI9341_BLACK;
  g_context.textsize_x = 2;
  g_context.textsize_y = 2;
  g_context.text_color = ILI9341_WHITE;
  g_context.wrap = false;

  glib_set_rotation(&g_context, rotation);

  draw_main_screen();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  touch_screen_get_point(200, &point);
  int temp;
  switch (rotation) {
    case 1:
      temp = point.x;
      point.x = point.y;
      point.y = g_context.height - temp;
      break;
    case 2:
      point.x = g_context.width - point.x;
      point.y = g_context.height - point.y;
      break;
    case 3:
      temp = point.x;
      point.x = g_context.width - point.y;
      point.y = g_context.height - temp;
      break;
    default:
      break;
  }
  if (point.z < PRESSURE_THRESH) {
    if ((point.y >= (g_context.height - 30)) && (point.x < 240)) {
      if ((point.x >= 0) && (point.x < 30)) {
        color = ILI9341_YELLOW;
        glib_fill_rect(&g_context, 0, 0, 10, 10, color);
      } else if ((point.x >= 30) && (point.x < 60)) {
        color = ILI9341_BLUE;
        glib_fill_rect(&g_context, 0, 0, 10, 10, color);
      } else if ((point.x >= 60) && (point.x < 90)) {
        color = ILI9341_GREEN;
        glib_fill_rect(&g_context, 0, 0, 10, 10, color);
      } else if ((point.x >= 90) && (point.x < 120)) {
        color = ILI9341_RED;
        glib_fill_rect(&g_context, 0, 0, 10, 10, color);
      } else if ((point.x >= 120) && (point.x < 150)) {
        color = ILI9341_WHITE;
        glib_fill_rect(&g_context, 0, 0, 10, 10, color);
      } else if ((point.x >= 150) && (point.x < 180)) {
        color = ILI9341_PINK;
        glib_fill_rect(&g_context, 0, 0, 10, 10, color);
      } else if ((point.x >= 180) && (point.x < 210)) {
        color = ILI9341_PURPLE;
        glib_fill_rect(&g_context, 0, 0, 10, 10, color);
      } else if ((point.x >= 210) && (point.x < 240)) {
        color = ILI9341_ORANGE;
        glib_fill_rect(&g_context, 0, 0, 10, 10, color);
      }
    } else if ((point.x >= (g_context.width - 40)) && (point.y < 25)) {
      draw_main_screen();
    } else {
      glib_draw_pixel(&g_context, point.x, point.y, color);
    }
  }
}
