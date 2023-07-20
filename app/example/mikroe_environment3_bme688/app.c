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
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "app_log.h"

#ifdef SL_CATALOG_MIKROE_ENVIRONMENT3_SPI_PRESENT
#include "sl_spidrv_instances.h"
#include "mikroe_bme688_spi.h"
#endif

#ifdef SL_CATALOG_MIKROE_ENVIRONMENT3_I2C_PRESENT
#include "mikroe_bme688_i2c.h"
#include "sl_i2cspm_instances.h"
#endif

#define READING_INTERVAL_MSEC    2000
static volatile bool enable_reading_data = false;
static sl_sleeptimer_timer_handle_t app_periodic_timer;

static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

void app_init(void)
{
#ifdef SL_CATALOG_MIKROE_ENVIRONMENT3_I2C_PRESENT
  mikroe_environment3_init(sl_i2cspm_mikroe);
#endif

#ifdef SL_CATALOG_MIKROE_ENVIRONMENT3_SPI_PRESENT
  if (mikroe_environment3_init(sl_spidrv_mikroe_handle) == SL_STATUS_OK) {
    app_log("Sensor init OK\n");
  }
#endif

  mikroe_environment3_enable_heater(MIKROE_ENVIRONMENT3_DISABLE);

  app_log("Sensor initialization done.\n");
  // Start periodic timer used for read data from sensor.
  sl_sleeptimer_start_periodic_timer_ms(&app_periodic_timer,
                                        READING_INTERVAL_MSEC,
                                        app_periodic_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  float temperature, pressure, humidity;
  uint32_t gas_resistance;

  if (enable_reading_data) {
    enable_reading_data = false;

    if (mikroe_environment3_get_all_data(&temperature, &humidity, &pressure,
                                         &gas_resistance) == SL_STATUS_OK) {
      app_log("Temperature : %.2f C\r\n", temperature);

      app_log("Humidity : %.2f %%\r\n", humidity);

      app_log("Pressure : %.3f mBar\r\n", pressure);
    }
  }
}

static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)timer;
  (void)data;

  enable_reading_data = true;
}
