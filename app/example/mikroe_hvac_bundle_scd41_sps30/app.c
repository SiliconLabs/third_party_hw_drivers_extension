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
#include "app_log.h"
#include "app_assert.h"
#include "mikroe_scd41.h"
#include "sparkfun_sps30_i2c.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"

#define READING_INTERVAL_MSEC    5000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t stt;
  uint16_t ser_num[3];
  mikroe_scd41_feature_data_t version_data;

  app_log("Hello World - Mikroe HVAC Click Bundle example\r\n");
  app_log("-----------------------------------------\r\n");

  // SCD41 Initialization
  stt = mikroe_scd41_init(sl_i2cspm_mikroe);
  app_assert_status(stt);
  app_log("Mikroe SCD41\r\n\nInit Done\r\n");

  mikroe_scd41_send_cmd(MIKROE_SCD41_PERFORM_FACTORY_RESET);
  app_log("Perform Factory Reset Done\r\n");

  sl_sleeptimer_delay_millisecond(2000);

  stt = mikroe_scd41_get_serial_number(ser_num);
  app_assert_status(stt);
  app_log("Get Serial Number Done\r\n");
  app_log("Serial Number: %.4d-%.4d-%.4d\r\n",
          ( uint16_t ) ser_num[0],
          ( uint16_t ) ser_num[1],
          ( uint16_t ) ser_num[2]);

  stt = mikroe_scd41_get_feature_set_version(&version_data);
  app_assert_status(stt);
  app_log("Get Feature Set Version Done\r\n");
  app_log("SCD41 - Features\r\n");
  app_log("Product Type    : %d \r\n", ( uint16_t ) version_data.product_type);
  app_log("Platform Type   : %d \r\n", ( uint16_t ) version_data.platform_type);
  app_log("Product Version : %d.%d \r\n",
          ( uint16_t ) version_data.product_major_version,
          ( uint16_t ) version_data.product_minor_version);

  // Trigger measure single shot */
  mikroe_scd41_send_cmd(MIKROE_SCD41_MEASURE_SINGLE_SHOT);
  app_log("-----------------------------------------\r\n");

  app_log_iostream_set(sl_iostream_vcom_handle);
  // SPS30 Initialization
  uint32_t sc = sps30_init(sl_i2cspm_mikroe);
  app_assert_status(sc);
  app_log("Sparkfun SPS30\r\nInit Done\r\n");

  while (sps30_probe() != 0) {
    app_log("SPS30 Sensor Probing Failed\r\n");
    sl_sleeptimer_delay_millisecond(1000);
  }

  app_log("SPS30 Sensor Probing Successful\r\n");

  uint8_t fw_major;
  uint8_t fw_minor;
  stt = sps30_read_firmware_version(&fw_major, &fw_minor);
  app_assert_status(stt);
  app_log("FW Version: %u.%u\r\n", fw_major, fw_minor);

  uint8_t serial_number[SPS30_MAX_SERIAL_LEN];
  stt = sps30_get_serial((char *)serial_number);
  app_assert_status(stt);
  app_log("Serial Number: %s\r\n", serial_number);

  stt = sps30_start_measurement();
  app_assert_status(stt);
  app_log("Measurements Started\r\n");

  app_log("-----------------------------------------\r\n\n");

  // Start periodic measurement
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t stt;
  sps30_measurement_t measure;
  mikroe_scd41_measuremen_data_t hvac_data;

  if (trigger_process) {
    trigger_process = false;
    stt = mikroe_scd41_read_measurement(&hvac_data);

    if (SL_STATUS_OK == stt) {
      app_log("<----------- Measured Values ----------->\r\n");
      app_log("CO2 Concent = %dppm\r\n", hvac_data.co2_concent);
      app_log("Temperature = %.2f%cC \r\n", hvac_data.temperature, 176);
      app_log("R.Humidity = %.2f %% \r\n", hvac_data.r_humidity);

      // Continue trigger measure single shot
      mikroe_scd41_send_cmd(MIKROE_SCD41_MEASURE_SINGLE_SHOT);
    } else {
      app_log("Error, code = 0x%lx\r\n", stt);
      while (1) {}
    }

    stt = sps30_read_measurement(&measure);
    if (stt != SL_STATUS_OK) {
      app_log("Error Reading Measurement\r\n");
    } else {
      app_log("%0.2f pm1.0\r\n"
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
    app_log("-----------------------------------------\r\n\n");
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
