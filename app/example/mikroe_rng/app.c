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
#include "mikroe_rng.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"

#define READING_INTERVAL_MSEC    500

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t stt;

  app_log("Hello World - Mikroe RNG Click example\r\n");

  stt = mikroe_rng_init(sl_i2cspm_mikroe);
  mikroe_rng_default_cfg();
  app_log("mikroe_rng_init = 0x%x\r\n", (uint16_t)stt);

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

    float voltage;
    sl_status_t stt = mikroe_rng_get_voltage(&voltage);

    app_log("mikroe_rng_get_voltage = 0x%lx, voltage = %.3f(mV)\r\n",
            stt, voltage);
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
