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

#include "sl_sleeptimer.h"
#include "app_log.h"

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_SPI_PRESENT
#include "sl_spidrv_instances.h"
#include "mikroe_bme688_spi.h"
#endif

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_I2C_PRESENT
#include "mikroe_bme688_i2c.h"
#include "sl_i2cspm_instances.h"
#endif

#include "bme68x.h"

#define READING_INTERVAL_MSEC    2000
#define HEATER_TEMPERATURE       300
#define HEATING_DURATION         100

static bme68x_dev_t bme688;

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_SPI_PRESENT
static bme68x_spi_t bme688_spi;
#endif

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_I2C_PRESENT
static bme68x_i2c_t bme688_i2c;
#endif

static volatile bool enable_reading_data = false;
static sl_sleeptimer_timer_handle_t app_periodic_timer;

static sl_status_t app_bme688_init(void);
static sl_status_t app_bme688_get_data(void);
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  if (app_bme688_init() != SL_STATUS_OK) {
    app_log("Initialization error. Please check again ...\r\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (enable_reading_data) {
    enable_reading_data = false;

    if (app_bme688_get_data() != SL_STATUS_OK) {
      app_log("Reading error. Please check again ...\r\n");
    }
  }
}

static sl_status_t app_bme688_init(void)
{
  int8_t rslt;
  bme68x_conf_t conf;
  bme68x_heatr_conf_t heatr_conf;

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_I2C_PRESENT
  // Initialize an I2C interface for BME688
  bme688_i2c.handle = sl_i2cspm_mikroe;
  bme688_i2c.addr = BME68X_I2C_ADDR_LOW;
  bme688.intf_ptr = &bme688_i2c;
  rslt = mikroe_bme688_i2c_init(&bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }
#endif

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_SPI_PRESENT
  // Initialize an SPI interface for BME688
  bme688_spi.handle = sl_spidrv_mikroe_handle;
  bme688.intf_ptr = &bme688_spi;
  rslt = mikroe_bme688_spi_init(&bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }
#endif

  rslt = bme68x_init(&bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }

  conf.filter = BME68X_FILTER_OFF;
  conf.odr = BME68X_ODR_NONE;
  conf.os_hum = BME68X_OS_16X;
  conf.os_pres = BME68X_OS_1X;
  conf.os_temp = BME68X_OS_2X;
  rslt = bme68x_set_conf(&conf, &bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }

  heatr_conf.enable = BME68X_ENABLE;
  heatr_conf.heatr_temp = HEATER_TEMPERATURE;
  heatr_conf.heatr_dur = HEATING_DURATION;
  rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }

  // Start periodic timer used for read data from sensor.
  return sl_sleeptimer_start_periodic_timer_ms(&app_periodic_timer,
                                               READING_INTERVAL_MSEC,
                                               app_periodic_timer_cb,
                                               (void *) NULL,
                                               0,
                                               0);
}

static sl_status_t app_bme688_get_data(void)
{
  bme68x_data_t data;
  uint8_t n_fields;
  int8_t rslt;
  uint32_t delay_period;
  bme68x_conf_t conf;

  // Trigger a measurement
  rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }
  // Calculate delay period in microseconds
  delay_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme688)
                 + HEATING_DURATION * 1000;
  // Wait for the measurement to complete
  bme688.delay_us(delay_period, bme688.intf_ptr);
  rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }

  if (n_fields) {
    app_log("\r\nTemperature : %.2f" "\xB0" "C\r\n", data.temperature);
    app_log("Humidity : %.2f %%\r\n", data.humidity);
    app_log("Pressure : %.2f Pa\r\n", data.pressure);
    app_log("Gas resistance : %.2f Ohm\r\n", data.gas_resistance);
  }

  return SL_STATUS_OK;
}

static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)timer;
  (void)data;

  enable_reading_data = true;
}
