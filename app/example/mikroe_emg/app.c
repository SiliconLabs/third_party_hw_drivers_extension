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

#include "sl_sleeptimer.h"
#include "app_log.h"
#include "mikroe_emg.h"

#define READING_INTERVAL_MSEC    5

static uint32_t time;
static volatile bool emg_timer_expire = false;
static sl_sleeptimer_timer_handle_t emg_timer;

static void emg_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                    void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t status;

  app_log("Application Initialization.\r\n");
  status = mikroe_emg_init(IADC0);
  if (status == SL_STATUS_OK) {
    app_log("EMG Click is initialized.\r\n");
  } else {
    app_log("EMG Click initialization is failed!\r\n");
    app_log(" Please, run program again... \r\n");
  }

  time = 0;

  sl_sleeptimer_start_periodic_timer_ms(&emg_timer,
                                        READING_INTERVAL_MSEC,
                                        emg_sleeptimer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t status;
  uint16_t adc_value;

  if (emg_timer_expire == false) {
    return;
  }

  emg_timer_expire = false;
  status = mikroe_emg_read_an_pin_value(&adc_value);
  if (SL_STATUS_OK == status) {
    app_log("%u, %lu\r\n", adc_value, time);
  }
  time += READING_INTERVAL_MSEC;
}

/***************************************************************************//**
 * Simple timer callback function.
 ******************************************************************************/
static void emg_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                    void *data)
{
  (void)timer;
  (void)data;

  emg_timer_expire = true;
}
