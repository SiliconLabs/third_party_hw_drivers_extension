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

#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "mikroe_mic2.h"

#define READING_INTERVAL_MSEC    500

static mikroe_mic2_data_t adc_value;
static volatile bool mic2_timer_expire = false;
static sl_sleeptimer_timer_handle_t mic2_timer;

static void mic2_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  app_log("Application Initialization.\n");
  mikroe_mic2_cfg_setup();
  sc = mikroe_mic2_init(sl_i2cspm_mikroe, IADC0);
  if (!sc) {
    app_log("MIC 2 Click is initialized.\n");
  } else {
    app_log("Status code: %lx\n", sc);
    app_log("MIC 2 Click initialization is failed!\n");
  }
  mikroe_mic2_set_potentiometer(35);

  sl_sleeptimer_start_periodic_timer_ms(&mic2_timer,
                                        READING_INTERVAL_MSEC,
                                        mic2_sleeptimer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * Simple timer callback function.
 ******************************************************************************/
static void mic2_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data)
{
  (void)timer;
  (void)data;

  mic2_timer_expire = true;
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t sc;
  float db_value;

  if (mic2_timer_expire) {
    mic2_timer_expire = false;
    sc = mikroe_mic2_generic_read(&adc_value);
    if (SL_STATUS_OK == sc) {
      db_value = (adc_value + 83.2073) / 11.003;
      app_log("Sound intensity: %.2f dB.\n", db_value);
    } else {
      app_log("Reading the ADC value is failing.!\n");
    }
  }
}
