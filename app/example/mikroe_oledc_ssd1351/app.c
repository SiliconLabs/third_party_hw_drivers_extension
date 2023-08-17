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
#include "sl_spidrv_instances.h"
#include "sl_sleeptimer.h"

#include "app_log.h"
#include "app_assert.h"

#include "mikroe_ssd1351_image.h"
#include "mikroe_ssd1351.h"
#include "glib.h"

#define APP_STATE_DISPLAY_RECTANGLE   0
#define APP_STATE_DISPLAY_LINE        1
#define APP_STATE_DISPLAY_IMAGE       2

#define TIMEOUT_MSEC                  500

static glib_context_t glib_context;
static volatile uint8_t state = APP_STATE_DISPLAY_RECTANGLE;
static volatile bool timer_is_expire = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static void app_task(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  app_log("---- Application Init ----\r\n");
  //  Click initialization.
  sc = mikroe_ssd1351_init(sl_spidrv_mikroe_handle);
  app_assert_status(sc);
  glib_init(&glib_context);
  glib_set_bg_color(&glib_context, 0xFFFF);
  glib_set_text_color(&glib_context, 0x0000);
  glib_enable_display(true);

  // demo with API of GLIB
  glib_clear(&glib_context);
  glib_fill(&glib_context, 0xFF00);
  glib_update_display();

  glib_fill(&glib_context, 0xFFFF);
  glib_update_display();

  glib_draw_string(&glib_context, "SILICON LABS", 15, 25);
  glib_draw_string(&glib_context, "DEMO", 35, 45);
  glib_draw_string(&glib_context, "OLED C CLICK", 10, 65);
  glib_update_display();

  sl_sleeptimer_start_periodic_timer(&app_timer_handle,
                                     TIMEOUT_MSEC,
                                     app_timer_cb,
                                     (void *) NULL,
                                     0,
                                     0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (timer_is_expire == false) {
    return;
  }
  timer_is_expire = false;
  app_task();
}

// Timer callback function
static void app_task(void)
{
  switch (state)
  {
    case APP_STATE_DISPLAY_RECTANGLE:
      mikroe_ssd1351_rectangle(0, 0, 96, 96, 0xF000);
      mikroe_ssd1351_rectangle(5, 5, 91, 91, 0xFF00);
      mikroe_ssd1351_rectangle(10, 10, 86, 86, 0x00F0);
      mikroe_ssd1351_rectangle(15, 15, 81, 81, 0x0F0F);
      mikroe_ssd1351_rectangle(20, 20, 76, 76, 0xF000);
      mikroe_ssd1351_rectangle(25, 25, 71, 71, 0xFF00);
      state = APP_STATE_DISPLAY_LINE;
      break;

    case APP_STATE_DISPLAY_LINE:
      mikroe_ssd1351_rectangle(25, 25, 71, 27, 0);
      mikroe_ssd1351_rectangle(25, 71, 71, 73, 0);
      mikroe_ssd1351_rectangle(25, 25, 27, 71, 0);
      mikroe_ssd1351_rectangle(68, 25, 71, 71, 0);
      state = APP_STATE_DISPLAY_IMAGE;
      break;

    case APP_STATE_DISPLAY_IMAGE:
      mikroe_ssd1351_fill_screen(0xFFFF);
      mikroe_ssd1351_image(aclogo, 0, 0);
      mikroe_ssd1351_image(mikroe_with_slogan, 0, 62);
      state = APP_STATE_DISPLAY_RECTANGLE;
      break;
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  timer_is_expire = true;
}
