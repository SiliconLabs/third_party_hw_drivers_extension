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
#include "sl_sleeptimer.h"
#include "sl_component_catalog.h"
#if defined(SL_CATALOG_ADAFRUIT_TFT_LCD_ILI9341_DMA_PRESENT)
#include "sl_spidrv_instances.h"
#endif
#include "adafruit_ili9341.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "ui.h"

#define LVGL_TIMER_PERIOD     1
#define LVGL_LOADER_DELAY     3000

static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t main_timer;
static sl_sleeptimer_timer_handle_t lvgl_tick_timer;

static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data);
static void app_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                    void *data);

void app_init(void)
{
#if defined(SL_CATALOG_ADAFRUIT_TFT_LCD_ILI9341_PRESENT)
  adafruit_ili9341_init();
#elif defined(SL_CATALOG_ADAFRUIT_TFT_LCD_ILI9341_DMA_PRESENT)
  adafruit_ili9341_init(sl_spidrv_ili9341_handle);
#endif
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  sl_sleeptimer_start_periodic_timer_ms(&lvgl_tick_timer,
                                        LVGL_TIMER_PERIOD,
                                        lvgl_tick_timer_callback,
                                        NULL,
                                        0,
                                        0);

  ui_init();
  sl_sleeptimer_start_timer_ms(&main_timer,
                               LVGL_LOADER_DELAY,
                               app_sleeptimer_callback,
                               NULL,
                               0,
                               0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  lv_timer_handler_run_in_period(5);

  if (app_timer_expire == true) {
    app_timer_expire = false;
    lv_disp_load_scr(ui_main);
  }
}

static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data)
{
  (void)timer;
  (void)data;

  lv_tick_inc(LVGL_TIMER_PERIOD);
}

static void app_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                    void *data)
{
  (void)timer;
  (void)data;

  app_timer_expire = true;
}
