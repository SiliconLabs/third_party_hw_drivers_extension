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
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"

#include "app_assert.h"
#include "app_log.h"

#include "sparkfun_vcnl4040.h"

#define READING_INTERVAL_MSEC    1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
  uint16_t id;

  app_log("\r======== VCNL4040 Proximity Sensor Driver ========\n");
  sc = sparkfun_vcnl4040_init(sl_i2cspm_qwiic);
  app_assert(sc == SL_STATUS_OK, "\rVCNL4040 Init fail...\n");
  app_log("\rVCNL4040 Init done...\n");

  sparkfun_vcnl4040_get_id(&id);
  app_log("\rDevice ID: 0x%X\n", id);
  app_log("\r\t==================================\n");

  sc = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                             READING_INTERVAL_MSEC,
                                             app_timer_cb,
                                             (void *) NULL,
                                             0,
                                             0);

  if (sc != SL_STATUS_OK) {
    app_log("\r > Start Periodic Measurement Fail\n");
  } else {
    app_log("\r > Start Periodic Measurement\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t sc;
  uint16_t prox;
  uint16_t ambient;
  uint16_t white;

  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  sc = sparkfun_vcnl4040_get_proximity(&prox);
  sc |= sparkfun_vcnl4040_get_ambient(&ambient);
  sc |= sparkfun_vcnl4040_get_white(&white);

  if (sc != SL_STATUS_OK) {
    app_log("\r > Reading data failed\n");
  } else {
    app_log(
      "\r > Proximity value: % 4d\tAmbient light value: % 4d\tWhite light value: % 4d\n",
      prox,
      ambient,
      white);
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
