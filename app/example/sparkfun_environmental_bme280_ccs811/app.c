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

#include "sparkfun_bme280.h"
#include "app_log.h"
#include "sl_i2cspm_instances.h"
#include "sparkfun_ccs811.h"
#include "sl_simple_timer.h"

static sl_simple_timer_t bme280_ccs811_timer;
static void bme280_ccs811_callback(sl_simple_timer_t *timer, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_log("Application Initialization.\n");
  sparkfun_bme280_i2c_t bme280_init_default;
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

  sl_simple_timer_start(&bme280_ccs811_timer,
                        1000,
                        bme280_ccs811_callback,
                        NULL,
                        true);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

static void bme280_ccs811_callback(sl_simple_timer_t *timer, void *data)
{
  (void)timer;
  (void)data;

  sparkfun_bme280_ctrl_measure_set_to_work();

  int32_t temp = 0;
  if (SL_STATUS_OK == sparkfun_bme280_read_temperature(&temp)) {
    app_log("Temperature: %ld %cC\n", (temp / 100), 0XF8);
  }

  uint32_t hum = 0;
  if (SL_STATUS_OK == sparkfun_bme280_read_humidity(&hum)) {
    app_log("Humidity: %ld%%\n", hum / 1000);
  }

  uint32_t press = 0;
  if (SL_STATUS_OK == sparkfun_bme280_read_pressure(&press)) {
    app_log("Presure: %ld mBar\n", press);
  }

  uint16_t eco2, tvoc;
  if (!sparkfun_ccs811_get_measurement(sl_i2cspm_qwiic, &eco2, &tvoc)) {
    app_log("CO2: %d ppm\n", eco2);
    app_log("TVOC: %d ppb\n", tvoc);
  }
}
