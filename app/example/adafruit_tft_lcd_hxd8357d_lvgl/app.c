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
#if defined(SL_CATALOG_ADAFRUIT_TFT_LCD_HXD8357D_DMA_PRESENT)
#include "sl_spidrv_instances.h"
#endif
#include "adafruit_hxd8357d.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "ui.h"

#include "sl_simple_led_instances.h"
#include "sl_simple_button_instances.h"

#define LVGL_TIMER_PERIOD     1

static bool btn0_pressed = 0;
static bool btn0_released = 0;
static sl_sleeptimer_timer_handle_t lvgl_tick_timer;

static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data);

void app_init(void)
{
#if defined(SL_CATALOG_ADAFRUIT_TFT_LCD_HXD8357D_PRESENT)
  adafruit_hxd8357d_init();
#elif defined(SL_CATALOG_ADAFRUIT_TFT_LCD_HXD8357D_DMA_PRESENT)
  adafruit_hxd8357d_init(sl_spidrv_hxd8357d_handle);
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

  // init status led 0
  lv_obj_add_flag(ui_led0ONSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_led0OnLabel, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_led0OffSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_led0OffLabel, LV_OBJ_FLAG_HIDDEN);

  // init status Led 1
  lv_obj_add_flag(ui_led1ONSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_led1OnLabel, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_led1OffSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_led1OffLabel, LV_OBJ_FLAG_HIDDEN);

  // init status button 0
  lv_obj_add_flag(ui_buttonOnSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_buttonOffSymbol, LV_OBJ_FLAG_HIDDEN);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  lv_timer_handler_run_in_period(5);

  if (btn0_pressed) {
    lv_obj_add_flag(ui_buttonOffSymbol, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_buttonOnSymbol, LV_OBJ_FLAG_HIDDEN);
    btn0_pressed = false;
  }
  if (btn0_released) {
    lv_obj_add_flag(ui_buttonOnSymbol, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_buttonOffSymbol, LV_OBJ_FLAG_HIDDEN);
    btn0_released = false;
  }
}

static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data)
{
  (void)timer;
  (void)data;

  lv_tick_inc(LVGL_TIMER_PERIOD);
}

void sl_button_on_change(const sl_button_t *handle)
{
  if (handle == &sl_button_btn0) {
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
      btn0_pressed = true;
    } else {
      btn0_released = true;
    }
  }
}
