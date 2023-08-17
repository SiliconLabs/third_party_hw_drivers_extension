/***************************************************************************//**
 * @file  app.c
 * @brief Top level application functions for Qwiic BME280 and CCS811 driver
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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
 * Include.
 ******************************************************************************/

#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"

#include "app_log.h"

#include "sparkfun_bme280.h"
#include "sparkfun_ccs811.h"

#define READING_INTERVAL_MSEC    1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sparkfun_bme280_i2c_t bme280_init_default;

  app_log("Application Initialization.\n");

  bme280_init_default = (sparkfun_bme280_i2c_t) BME280_I2C_DEFAULT;
  sparkfun_bme280_i2c(&bme280_init_default);
  if (sparkfun_bme280_init() == SL_STATUS_OK) {
    app_log("BME280 on 0x%02X I2C address found and initialized.\n",
            BME_280_DEFAULT_I2C_ADDR);
  } else {
    app_log("BME280 on 0x%02X I2C address not found. Check cables. "
            "\n\r Try also alternative address:", BME_280_DEFAULT_I2C_ADDR);
    if (BME_280_DEFAULT_I2C_ADDR == 0x77) {
      app_log(" 0x76\n");
    } else {
      app_log(" 0x77\n");
    }
  }

  if (!sparkfun_ccs811_init(sl_i2cspm_qwiic)) {
    app_log("CCS811 on 0x%02X I2C address found and initialized.\n",
            SPARKFUN_CCS811_DEFAULT_ADDR);
  } else {
    app_log("CCS811 on 0x%02X I2C address not found. Check cables. "
            "\n\r Try also alternative address\n",
            SPARKFUN_CCS811_DEFAULT_ADDR);
    if (SPARKFUN_CCS811_DEFAULT_ADDR == 0x5B) {
      app_log(" 0x5A\n");
    } else {
      app_log(" 0x5B\n");
    }
  }

  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
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
  uint16_t eco2, tvoc;
  int32_t temp = 0;
  uint32_t hum = 0;
  uint32_t press = 0;

  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  sparkfun_bme280_ctrl_measure_set_to_work();

  if (SL_STATUS_OK == sparkfun_bme280_read_temperature(&temp)) {
    app_log("Temperature: %ld %cC\n", (temp / 100), 0XF8);
  }

  if (SL_STATUS_OK == sparkfun_bme280_read_humidity(&hum)) {
    app_log("Humidity: %ld%%\n", hum / 1000);
  }

  if (SL_STATUS_OK == sparkfun_bme280_read_pressure(&press)) {
    app_log("Presure: %ld mBar\n", press);
  }

  if (!sparkfun_ccs811_get_measurement(sl_i2cspm_qwiic, &eco2, &tvoc)) {
    app_log("CO2: %d ppm\n", eco2);
    app_log("TVOC: %d ppb\n", tvoc);
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
