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

#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"

#include "app_log.h"

#include "mikroe_mlx90632.h"

#define READING_INTERVAL_MSEC         1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static void application_task(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  if (mikroe_mlx90632_init(sl_i2cspm_mikroe) == SL_STATUS_OK) {
    app_log("IrThermo 3 Click initializes successfully\n");
  } else {
    app_log("IrThermo 3 Click initializes fail\n");
  }

  mikroe_mlx90632_default_config();

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

static void application_task(void)
{
  if (mikroe_mlx90632_present() == SL_STATUS_OK) {
    app_log("Ambient temp: %.2f\n", mikroe_mlx90632_get_ambient_temperature());
    app_log("Object temp: %.2f\n", mikroe_mlx90632_get_object_temperature());
  } else {
    app_log("IrThermo 3 Click is not present on the bus\n");
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
