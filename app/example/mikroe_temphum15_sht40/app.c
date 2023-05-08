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
#include "mikroe_sht40.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
void app_timer_callback_fcn(sl_sleeptimer_timer_handle_t *handle, void *data);

sl_status_t stt;
uint32_t sht40_serial;
mikroe_sht40_measurement_data_t sht40Data;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static bool trigger_process = false;

void app_init(void)
{
  app_log("Hello World - Mikroe Temp&Hum 15 SHT40 example\r\n");

  stt = mikroe_sht40_init(sl_i2cspm_mikroe);
  app_log("mikroe_sht40_init = 0x%x\r\n", (uint16_t)stt);

  stt = mikroe_sht40_read_serial(&sht40_serial);
  app_log("mikroe_sht40_read_serial = 0x%x Serial = 0x%lx\r\n",
          (uint16_t)stt, sht40_serial);
  app_log("sl_sleeptimer_start_periodic = 0x%lx\r\n",
          sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                                1000,
                                                app_timer_callback_fcn,
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

    stt = mikroe_sht40_get_temp_and_hum(mikroe_sht40_precision_mode_high,
                                        &sht40Data);
    if (SL_STATUS_OK == stt) {
      app_log("temperature = %0.2f%cC, humidity = %0.2f %%\r\n",
              sht40Data.temperature,
              176,
              sht40Data.humidity);
    } else {
      app_log("mikroe_sht40_get_temp_and_hum error = 0x%lx\r\n", stt);
    }
  }
}

void app_timer_callback_fcn(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
