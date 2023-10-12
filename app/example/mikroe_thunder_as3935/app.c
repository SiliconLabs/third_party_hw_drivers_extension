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
#include "sl_spidrv_instances.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "mikroe_thunder_as3935.h"

#define READING_INTERVAL_MSEC 1000

static volatile bool flag = false;
static uint8_t storm_mode;
static uint32_t storm_energy;
static uint8_t storm_distance;
static sl_sleeptimer_timer_handle_t app_timer_handle;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle,
                         void *data);

void app_init(void)
{
  sl_status_t sc;

  app_log("---- Application Init ----\r\n");

  sc = mikroe_thunder_as3935_init(sl_spidrv_mikroe_handle);

  if (sc != SL_STATUS_OK) {
    app_log("---- Application Init Error ----");
    app_log("---- Please, run program again ----");
    for ( ; ; ) {}
  }
  mikroe_thunder_as3935_default_cfg();
  app_log("---- Application Init Done ----\r\n");
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (flag) {
    storm_mode = mikroe_thunder_as3935_check_int();

    if (THUNDER_NOISE_LEVEL_INTERR == storm_mode) {
      app_log("Noise level too high\r\n\n");
    } else if (THUNDER_DISTURBER_INTERR == storm_mode) {
      app_log("Disturber detected\r\n\n");
    } else if (THUNDER_LIGHTNING_INTERR == storm_mode) {
      mikroe_thunder_as3935_get_storm_info(&storm_energy, &storm_distance);
      app_log("Energy of the single lightning : %lu\r\n", storm_energy);
      app_log("Distance estimation : %u km\r\n\n", ( uint16_t ) storm_distance);
      // Reset configuration to prepare for the next measurement
      mikroe_thunder_as3935_default_cfg();
    }
    flag = false;
  }
}

void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  flag = true;
}
