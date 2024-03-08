/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
 * # EXPERIMENTAL QUALITY
 * This code has not been formally tested and is provided as-is.
 * It is not suitable for production environments.
 * This code will not be maintained.
 ******************************************************************************/
#include "sl_component_catalog.h"
#include "sl_sleeptimer.h"
#include "app_log.h"

#ifdef SL_CATALOG_PM_SPS30_I2C_PRESENT
#include "sl_i2cspm_instances.h"
#include "sparkfun_sps30_i2c.h"
#endif

#ifdef SL_CATALOG_PM_SPS30_UART_PRESENT
#include "sl_iostream_init_usart_instances.h"
#include "sparkfun_sps30_uart.h"
#endif

#define READING_INTERVAL_MSEC (3000)

static sl_sleeptimer_timer_handle_t app_periodic_timer;
static volatile bool enable_reading_data = false;
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  static sl_status_t  ret;
  app_log_iostream_set(sl_iostream_vcom_handle);

  /* Initialize SPS 30 */
#ifdef SL_CATALOG_PM_SPS30_I2C_PRESENT
  app_log("Hello world - SPS30 - Particulate Matter Sensor - I2C !!!\r\n");
  sps30_init(sl_i2cspm_mikroe);
#endif

#ifdef SL_CATALOG_PM_SPS30_UART_PRESENT
  app_log("Hello world - SPS30 - Particulate Matter Sensor - UART !!!\r\n");
  sps30_init(sl_iostream_sps30_handle);
#endif

  /* Busy loop for initialization */
  while (sps30_probe() != 0) {
    app_log("SPS30 sensor probing failed\r\n");
    sl_sleeptimer_delay_millisecond(1000);
  }

  app_log("SPS30 sensor probing successful\r\n");

#ifdef SL_CATALOG_PM_SPS30_I2C_PRESENT
  uint8_t fw_major;
  uint8_t fw_minor;
  ret = sps30_read_firmware_version(&fw_major, &fw_minor);
  if (ret) {
    app_log("Error reading version\r\n");
  } else {
    app_log("FW Ver: %u.%u\r\n", fw_major, fw_minor);
  }
#endif
#ifdef SL_CATALOG_PM_SPS30_UART_PRESENT
  sps30_version_information_t version;
  ret = sps30_read_version(&version);

  if (ret) {
    app_log("Error reading version\r\n");
  } else {
    app_log("FW Ver: %u.%u\r\n", version.firmware_major,
            version.firmware_minor);
    app_log("SHDLC Ver: %u.%u\r\n", version.shdlc_major, version.shdlc_minor);
  }
#endif

  uint8_t serial_number[SPS30_MAX_SERIAL_LEN];
  ret = sps30_get_serial((char *)serial_number);
  if (ret) {
    app_log("Error reading serial number\r\n");
  } else {
    app_log("Serial Number: %s\r\n", serial_number);
  }

  ret = sps30_start_measurement();
  if (ret) {
    app_log("Error starting measurement\r\n");
  } else {
    app_log("Measurements started\r\n");
  }

  // Start timer used for periodic indications.
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
  sps30_measurement_t measure;
  static sl_status_t ret;

  if (enable_reading_data) {
    enable_reading_data = false;

    ret = sps30_read_measurement(&measure);
    if (ret) {
      app_log("Error reading measurement\r\n");
    } else {
      app_log("<---------- Measured values ---------->\r\n"
              "%0.2f pm1.0\r\n"
              "%0.2f pm2.5\r\n"
              "%0.2f pm4.0\r\n"
              "%0.2f pm10.0\r\n"
              "%0.2f nc0.5\r\n"
              "%0.2f nc1.0\r\n"
              "%0.2f nc2.5\r\n"
              "%0.2f nc4.5\r\n"
              "%0.2f nc10.0\r\n"
              "%0.2f typical particle size\r\n",
              measure.mc_1p0,
              measure.mc_2p5,
              measure.mc_4p0,
              measure.mc_10p0,
              measure.nc_0p5,
              measure.nc_1p0,
              measure.nc_2p5,
              measure.nc_4p0,
              measure.nc_10p0,
              measure.typical_particle_size);
    }
  }
}

/**************************************************************************//**
 * Timer callback
 * Called periodically to reading data from SPS30.
 *****************************************************************************/
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)data;
  (void)timer;

  enable_reading_data = true;
}
