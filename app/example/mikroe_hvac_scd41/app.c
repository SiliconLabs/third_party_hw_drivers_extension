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
#include "mikroe_scd41.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"

#define READING_INTERVAL_MSEC    5000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;
static uint16_t ser_num[3];
static mikroe_scd41_measuremen_data_t hvac_data;
static mikroe_scd41_feature_data_t version_data;

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t stt;

  app_log("Hello World - Mikroe HVAC Click example\r\n");

  stt = mikroe_scd41_init(sl_i2cspm_mikroe);
  app_log("Mikroe SCD41 Init = 0x%x\r\n", (uint16_t)stt);

  mikroe_scd41_send_cmd(MIKROE_SCD41_PERFORM_FACTORY_RESET);
  app_log("Perform Factory Reset \r\n");
  app_log("-----------------------------------------\r\n");

  sl_sleeptimer_delay_millisecond(2000);

  stt = mikroe_scd41_get_serial_number(ser_num);
  app_log("mikroe_scd41_get_serial_number = 0x%x\r\n", (uint16_t)stt);
  app_log("SCD41 - Serial Number: %.4d-%.4d-%.4d\r\n",
          ( uint16_t ) ser_num[0],
          ( uint16_t ) ser_num[1],
          ( uint16_t ) ser_num[2]);
  app_log("-----------------------------------------\r\n");

  stt = mikroe_scd41_get_feature_set_version(&version_data);
  app_log("mikroe_scd41_get_feature_set_version = 0x%x\r\n", (uint16_t)stt);
  app_log("SCD41 - Features\r\n");
  app_log("Product Type    : %d \r\n", ( uint16_t ) version_data.product_type);
  app_log("Platform Type   : %d \r\n", ( uint16_t ) version_data.platform_type);
  app_log("Product Version : %d.%d \r\n",
          ( uint16_t ) version_data.product_major_version,
          ( uint16_t ) version_data.product_minor_version);
  app_log("-----------------------------------------\r\n");

  // Trigger measure single shot */
  mikroe_scd41_send_cmd(MIKROE_SCD41_MEASURE_SINGLE_SHOT);
  app_log("sl_sleeptimer_start_periodic = 0x%lx\r\n",
          sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                                READING_INTERVAL_MSEC,
                                                app_timer_callback,
                                                NULL,
                                                0,
                                                0));
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (trigger_process) {
    trigger_process = false;
    sl_status_t stt = mikroe_scd41_read_measurement(&hvac_data);

    if (SL_STATUS_OK == stt) {
      app_log("CO2 Concent = %dppm\r\n", hvac_data.co2_concent);
      app_log("Temperature = %.2f%cC \r\n", hvac_data.temperature, 176);
      app_log("R.Humidity = %.2f %% \r\n", hvac_data.r_humidity);
      app_log("-----------------------------------------\r\n");

      // Continue trigger measure single shot
      mikroe_scd41_send_cmd(MIKROE_SCD41_MEASURE_SINGLE_SHOT);
    } else {
      app_log("Error, code = 0x%lx\r\n", stt);
      while (1) {}
    }
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
