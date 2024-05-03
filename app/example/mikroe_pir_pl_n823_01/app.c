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
#include "sl_status.h"
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#include "sl_sleeptimer.h"
#include "mikroe_pl_n823_01.h"

#define APP_TIMER_INTERVAL_MS 1000

static sl_sleeptimer_timer_handle_t app_timer;
static volatile bool update = true;

static void app_timer_handler(sl_sleeptimer_timer_handle_t *timer, void *data);

/***************************************************************************//**
 * App timer handler function.
 ******************************************************************************/
static void app_timer_handler(sl_sleeptimer_timer_handle_t *timer, void *data)
{
  (void) timer;
  (void) data;

  update = true;
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
  app_log("=======================================\n");
  app_log("Silicon Labs Example - MikroE PIR Click.\n");
  app_log(" Application initialized successfully.\n");
  sc = mikroe_pl_n823_01_init(sl_i2cspm_mikroe);
  if (sc != SL_STATUS_OK) {
    app_log("MikroE PIR Click initialized failed!\n");
  }
  app_log("=======================================\n");
  sl_sleeptimer_start_periodic_timer_ms(&app_timer,
                                        APP_TIMER_INTERVAL_MS,
                                        app_timer_handler,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint16_t adc_val;
  float map_out;

  if (update) {
    adc_val = mikroe_pl_n823_01_get_adc();
    map_out = mikroe_pl_n823_01_scale_results(adc_val, 0, 3303);

    app_log("[PL-N823-01]: Voltage: %.2f mV\n", map_out);
    update = false;
  }
}
