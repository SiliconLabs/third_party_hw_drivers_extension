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

#include "mikroe_sht40_sgp40.h"
#include "sl_simple_timer.h"
#include "sl_i2cspm_instances.h"
#include "app_log.h"

static sl_simple_timer_t mikroe_environment2_timer;

static void app_mikroe_environment2_timer_handle(sl_simple_timer_t *timer,
                                                 void *data);

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
void app_init(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application init code here!                         //
  // This is called once during start-up.                                    //
  /////////////////////////////////////////////////////////////////////////////
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

    sl_simple_timer_start(&mikroe_environment2_timer,
                          200,
                          app_mikroe_environment2_timer_handle,
                          NULL,
                          true);
  }
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

static void app_mikroe_environment2_timer_handle(sl_simple_timer_t *timer,
                                                 void *data)
{
  (void)timer;
  (void)data;
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
