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
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "mikroe_pca9685.h"

#define INTERVAL_MSEC    10

static mikroe_servo_pos_and_res_t pos_res_setting =
{
  .min_position = 0,
  .max_position = 180,
  .low_resolution = 0,
  .high_resolution = 330,
};

static sl_sleeptimer_timer_handle_t servo_position_handle;
static int16_t count;
static bool counter_clockwise = true;
static volatile bool servo_position_expire = false;
static volatile bool adjust_position = false;

static void servo_position_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  app_log("Application Initialization.\n");
  sc = mikroe_pca9685_init(sl_i2cspm_mikroe);

  if (sc == SL_STATUS_OK) {
    app_log("Mikroe Servo Click is initialized.\n");
  } else {
    app_log("Mikroe Servo Click initialization failed.\n");
  }

  mikroe_pca9685_default_cfg();
  mikroe_pca9685_setting(&pos_res_setting);

  sl_sleeptimer_start_periodic_timer_ms(&servo_position_handle,
                                        INTERVAL_MSEC,
                                        servo_position_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (servo_position_expire == false) {
    return;
  }
  servo_position_expire = false;
  if (counter_clockwise) {
    app_log("----------------------------\n");
    app_log("------Counter clockwise-----\n");

    count = pos_res_setting.min_position;
    while (count <= pos_res_setting.max_position) {
      if (adjust_position) {
        mikroe_pca9685_set_position(MIKROE_SERVO_MOTOR_1, count);
        app_log("Position: %u\n", (uint16_t)count);
        count++;
        adjust_position = false;
      }
    }
  } else {
    app_log("---------------------------\n");
    app_log("---------Clockwise---------\n");

    count = pos_res_setting.max_position;
    while (count >= pos_res_setting.min_position) {
      if (adjust_position) {
        mikroe_pca9685_set_position(MIKROE_SERVO_MOTOR_1, count);
        app_log("Position: %u\n", (uint16_t)count);
        count--;
        adjust_position = false;
      }
    }
  }
  counter_clockwise = !counter_clockwise;
}

/***************************************************************************//**
 * Sleep timer callback function.
 ******************************************************************************/
static void servo_position_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;

  servo_position_expire = true;
  adjust_position = true;
}
