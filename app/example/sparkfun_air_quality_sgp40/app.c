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
#include "sparkfun_sgp40.h"

#include "sl_simple_timer.h"
#include "sl_i2cspm_instances.h"

#include "app_log.h"
// Global variables for measure
static uint16_t air_quality;
static int32_t voc_index;
static sl_simple_timer_t app_sgp40_timer_handle;
static void app_sgp40_timer_callback(sl_simple_timer_t *handle, void *data);
static sl_status_t app_sgp40_init(void);

/// ***************************************************************************//**
// * Initialize application.
// ******************************************************************************/
void app_init(void)
{
  // Click initialization.
  app_log("    Application Task   \r\n");
  app_log("-----------------------\r\n");
  app_log("  Air Quality Sensor  \r\n");
  app_log("-----------------------\r\n");

  if (app_sgp40_init() != SL_STATUS_OK) {
    app_log("  SPG40 inits failed  \r\n");
    app_log("-----------------------\r\n");
  } else {
    app_log("  SPG40 inits successfully \r\n");
    app_log("-----------------------\r\n");
    app_log("  Start measure  \r\n");
    app_log("-----------------------\r\n");
  }
}

/// ***************************************************************************//**
// * App ticking function.
// ******************************************************************************/
void app_process_action(void)
{
}

static void app_sgp40_timer_callback(sl_simple_timer_t *handle, void *data)
{
  (void) handle;
  (void) data;

  sparkfun_sgp40_measure_raw(&air_quality, 50, 25);
  app_log(" RAW data   : %d  \r\n", (uint16_t)air_quality);
  app_log("-----------------------\r\n");

  sparkfun_sgp40_get_voc_index(&voc_index, 50, 25);
  app_log(" VOC Index   : %d  \r\n", (uint16_t)voc_index);
  app_log("-----------------------\r\n");
}

static sl_status_t app_sgp40_init(void)
{
  sl_status_t ret;
  uint16_t results;

  ret = sparkfun_sgp40_init(sl_i2cspm_qwiic);
  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  sparkfun_sgp40_measure_test(&results);
  if (results != SPARKFUN_SGP40_TEST_PASSED) {
    return SL_STATUS_NOT_READY;
  }
  ret = sparkfun_sgp40_heater_off();
  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }

  sparkfun_sgp40_voc_algorithm_init();

  // Start a periodic timer 200 ms to read data from the sensor
  ret = sl_simple_timer_start(&app_sgp40_timer_handle,
                              200,
                              app_sgp40_timer_callback,
                              (void *)NULL,
                              true);

  return ret;
}
