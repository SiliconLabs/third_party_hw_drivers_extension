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

#include "sl_spidrv_instances.h"
#include "sl_sleeptimer.h"

#include "app_log.h"

#include "mikroe_ad8318.h"

#define READING_INTERVAL_MSEC         1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;

static void application_task(void);
static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  app_log("Application initialization.\n");

  sc = mikroe_ad8318_init(sl_spidrv_mikroe_handle);
  if (sc == SL_STATUS_OK) {
    app_log("RF Meter Click is initialized.\n");
  } else {
    app_log("RF Meter Click initialization is failed!\n");
  }

  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (app_timer_expire == false) {
    return;
  }

  app_timer_expire = false;
  application_task();
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
  if (sc == SL_STATUS_OK) {
    app_log("Signal strength: %.2f dBm\n", signal);
  } else {
    app_log("Reading signal strength is failed!\n");
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
