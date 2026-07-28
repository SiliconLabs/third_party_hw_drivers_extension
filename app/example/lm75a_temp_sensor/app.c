/***************************************************************************//**
 * @file
 * @brief LM75A temperature Sensor I2C application.
 *******************************************************************************
 * # License
 * <b>Copyright 2026 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/

#include "app.h"
#include "lm75a_temp_sensor.h"
#include "sl_sleeptimer.h"
#include <stdlib.h>

#ifdef SLI_SI917
#include "rsi_debug.h"
#include "sl_i2c_instances.h"
#define app_printf(...) DEBUGOUT(__VA_ARGS__)

/* User-configurable I2C instance and config for LM75A communication. */
#define LM75_I2C_INSTANCE            SL_I2C2
#define LM75_I2C_CONFIG              (&sl_i2c_i2c2_config)
static sl_i2c_instance_t lm75_i2c_instance = LM75_I2C_INSTANCE;
#else
#include "app_log.h"
#include "sl_i2cspm_instances.h"
#define app_printf(...) app_log(__VA_ARGS__)

/* User-configurable I2CSPM instance for LM75A communication. */
#define LM75_I2C_INSTANCE            sl_i2cspm_i2c_sensor
#define LM75_I2C_CONFIG              NULL
#endif

/***************************************************************************//**
 * Defines
 ******************************************************************************/
#define INITIAL_INTERVAL_MS          1000 // 1 second
#define LATER_INTERVAL_MS            60000 // 60 seconds
#define SWITCH_TIME_MS               30000 // 30 seconds

/***************************************************************************//**
 * Variables
 ******************************************************************************/
static sl_sleeptimer_timer_handle_t app_timer;
static volatile bool timer_flag = false;
static uint32_t elapsed_time_ms = 0;
static uint32_t current_interval_ms = INITIAL_INTERVAL_MS;

/***************************************************************************//**
 * Function Prototypes
 ******************************************************************************/
static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static void restart_timer(uint32_t interval_ms);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  void *i2c_instance = NULL;
  void *i2c_config = NULL;

#ifdef SLI_SI917
  DEBUGINIT();
  i2c_instance = &lm75_i2c_instance;
#else
  i2c_instance = LM75_I2C_INSTANCE;
#endif

  i2c_config = LM75_I2C_CONFIG;
  app_printf("\n==== LM75 Temperature Demo ====\n");

  if (lm75_init(i2c_instance, i2c_config) != SL_STATUS_OK) {
    app_printf("LM75 Init Failed!\n");
  }

  // Start with 1-second interval
  sl_sleeptimer_start_periodic_timer_ms(&app_timer,
                                        INITIAL_INTERVAL_MS,
                                        app_timer_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  int16_t temp_c;
  int16_t temp_f;

  if (timer_flag == false) {
    return;
  }

  timer_flag = false;

  // Update total elapsed time based on current timer interval.
  elapsed_time_ms += current_interval_ms;

  // Switch from 1-second interval to 1-minute interval
  // after 30 seconds have elapsed.
  if ((current_interval_ms != LATER_INTERVAL_MS)
      && (elapsed_time_ms >= SWITCH_TIME_MS)) {
    current_interval_ms = LATER_INTERVAL_MS;
    restart_timer(LATER_INTERVAL_MS);
    app_printf("Switched to 1-minute interval...\n");
  }

  // Read temperature as Celsius x 100
  if (lm75_get_temperature(&temp_c) == SL_STATUS_OK) {
    // Convert Celsius x 100 to Fahrenheit x 100
    temp_f = (int16_t)(((int32_t)temp_c * 9) / 5 + 3200);

    app_printf("Temp: %d.%02d C | %d.%02d F\n",
               temp_c / 100,
               abs(temp_c % 100),
               temp_f / 100,
               abs(temp_f % 100));
  } else {
    app_printf("Temperature Read Failed!\n");
  }
}

/***************************************************************************//**
 * Timer callback.
 *
 * @param[in] handle Sleeptimer handle.
 * @param[in] data User data passed to the timer callback.
 ******************************************************************************/
static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;

  timer_flag = true;
}

/***************************************************************************//**
 * Restart periodic application timer with the requested interval.
 *
 * @param[in] interval_ms Timer interval in milliseconds.
 ******************************************************************************/
static void restart_timer(uint32_t interval_ms)
{
  sl_sleeptimer_stop_timer(&app_timer);

  sl_sleeptimer_start_periodic_timer_ms(&app_timer,
                                        interval_ms,
                                        app_timer_cb,
                                        NULL,
                                        0,
                                        0);
}
