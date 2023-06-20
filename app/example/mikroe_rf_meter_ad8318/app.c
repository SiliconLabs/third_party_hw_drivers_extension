/***************************************************************************//**
 * @file app.c
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
#include "mikroe_ad8318.h"
#include "app_log.h"
#include "sl_spidrv_instances.h"
#include "sl_simple_timer.h"

static sl_simple_timer_t rf_meter_timer;

static void application_task(void);
static void rf_meter_simple_timer_callback(sl_simple_timer_t *timer,
                                           void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  app_log("Application initialization.\n");

  sc = mikroe_ad8318_init(sl_spidrv_mikroe_handle);
  if (!sc) {
    app_log("RF Meter Click is initialized.\n");
  } else {
    app_log("RF Meter Click initialization is failed!\n");
  }

  sl_simple_timer_start(&rf_meter_timer,
                        1000,
                        rf_meter_simple_timer_callback,
                        NULL,
                        true);
}

/***************************************************************************//**
 * Application task.
 ******************************************************************************/
static void application_task(void)
{
  sl_status_t sc;
  float signal;

  sc = mikroe_ad8318_get_signal_strength(MIKROE_AD8318_DEF_SLOPE,
                                         MIKROE_AD8318_DEF_INTERCEPT,
                                         &signal);
  if (!sc) {
    app_log("Signal strength: %.2f dBm\n", signal);
  } else {
    app_log("Reading signal strength is failed!\n");
  }
}

/***************************************************************************//**
 * Simple timer callback function.
 ******************************************************************************/
static void rf_meter_simple_timer_callback(sl_simple_timer_t *timer,
                                           void *data)
{
  (void)timer;
  (void)data;

  application_task();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
