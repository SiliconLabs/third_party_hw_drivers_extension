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
#include "adafruit_st7789.h"
#include "lv_port_disp.h"
#include "app_ui/ui.h"
#include "app_log.h"
#include "sl_sleeptimer.h"
#include "sl_spidrv_instances.h"
#include "sl_simple_button_instances.h"
#include "sl_simple_led_instances.h"

#define LVGL_TIMER_PERIOD     1
static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data);

static sl_sleeptimer_timer_handle_t lvgl_tick_timer;

static bool led0_state = false;
static bool led1_state = false;

static bool btn0_trigger = false;
static bool btn1_trigger = false;

void app_init(void)
{
  app_log("Hello World Adafruit 1.14\" TFT LCD with LVGL Demo\r\n");
  adafruit_st7789_init(sl_spidrv_st7789_handle);
  adafruit_st7789_set_rotation(adafruit_st7789_rotation_90);

  app_log("adafruit_st7789_init done\r\n");

  lv_init();
  lv_port_disp_init();

  sl_sleeptimer_start_periodic_timer_ms(&lvgl_tick_timer,
                                        LVGL_TIMER_PERIOD,
                                        lvgl_tick_timer_callback,
                                        NULL,
                                        0,
                                        0);
  app_log("lvgl init done\r\n");

  ui_init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  lv_timer_handler_run_in_period(5);

  if (btn0_trigger) {
    btn0_trigger = false;

    if (sl_button_get_state(&sl_button_btn0)) {
      led0_state = !led0_state;
      app_log("led0 state = %d\r\n", (uint8_t)led0_state);
      if (led0_state) {
        sl_led_turn_on(&sl_led_led0);
        lv_obj_add_flag(ui_imgled0off, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_imgled0on, LV_OBJ_FLAG_HIDDEN);
      } else {
        sl_led_turn_off(&sl_led_led0);
        lv_obj_add_flag(ui_imgled0on, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_imgled0off, LV_OBJ_FLAG_HIDDEN);
      }
    }
  }

  if (btn1_trigger) {
    btn1_trigger = false;

    if (sl_button_get_state(&sl_button_btn1)) {
      led1_state = !led1_state;
      app_log("led1 state = %d\r\n", (uint8_t)led1_state);
      if (led1_state) {
        sl_led_turn_on(&sl_led_led1);
        lv_obj_add_flag(ui_imgled1off, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_imgled1on, LV_OBJ_FLAG_HIDDEN);
      } else {
        sl_led_turn_off(&sl_led_led1);
        lv_obj_add_flag(ui_imgled1on, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_imgled1off, LV_OBJ_FLAG_HIDDEN);
      }
    }
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
    btn0_trigger = true;
  } else if (handle == &sl_button_btn1) {
    btn1_trigger = true;
  }
}
