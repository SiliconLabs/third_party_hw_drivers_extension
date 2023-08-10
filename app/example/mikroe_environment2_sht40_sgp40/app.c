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

#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "app_log.h"

#include "mikroe_sht40_sgp40.h"

#define READING_INTERVAL_MSEC 1000

static bool timer_is_expire = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static void app_task(void);

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
void app_init(void)
{
  if (SL_STATUS_OK != mikroe_environment2_init(sl_i2cspm_mikroe)) {
    app_log("Sensor initialized fail!.");
  } else {
    if (mikroe_environment2_sgp40_measure_test() == SL_STATUS_OK) {
      app_log("    All tests passed\r\n");
      app_log("      Successfully\r\n");
    } else {
      app_log("    One or more tests have\r\n");
      app_log("     Failed\r\n");
    }
    app_log("-----------------------\r\n");

    mikroe_environment2_sgp40_heater_off();

    mikroe_environment2_config_sensors();

    sl_sleeptimer_start_periodic_timer(&app_timer_handle,
                                       READING_INTERVAL_MSEC,
                                       app_timer_cb,
                                       (void *) NULL,
                                       0,
                                       0);
  }
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void)
{
  if (timer_is_expire == false) {
    return;
  }
  timer_is_expire = false;
  app_task();
}

static void app_task(void)
{
  uint16_t air_quality;
  float humidity;
  float temperature;
  int32_t voc_index;

  mikroe_environment2_get_temp_hum(&humidity, &temperature);
  app_log(" Humidity    : %.2f %% \r\n", humidity);
  app_log(" Temperature : %.2f C \r\n", temperature);

  mikroe_environment2_get_air_quality(&air_quality);
  app_log(" Air Quality : %d \r\n", air_quality);
  app_log("- - - - - - - - - -  - \r\n");

  mikroe_environment2_get_voc_index(&voc_index);
  app_log(" VOC Index   : %d  \r\n", ( uint16_t ) voc_index);
  app_log("-----------------------\r\n");
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  timer_is_expire = true;
}
