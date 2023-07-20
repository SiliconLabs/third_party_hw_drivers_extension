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
#include "mikroe_tsd10.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "sl_status.h"

static float sensor_ntu_value;
static volatile bool turbidity_timer_expire = false;
static sl_sleeptimer_timer_handle_t turbidity_timer;

static void turbidity_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                          void *data);

void app_init(void)
{
  app_log("Mikroe Turbidity - TSD10 sensor example\r\n");

  if (SL_STATUS_OK == mikroe_turbidity_init(sl_i2cspm_mikroe)) {
    app_log("Sensor initialization done.\n");
  } else {
    app_log("Sensor initialization failed.\n");
  }

  sl_sleeptimer_start_periodic_timer_ms(&turbidity_timer,
                                        500,
                                        turbidity_sleeptimer_callback,
                                        (void *) NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static sl_status_t ret;
  if (turbidity_timer_expire) {
    turbidity_timer_expire = false;

    ret = mikroe_turbidity_get_ntu(&sensor_ntu_value);
    if (SL_STATUS_OK == ret) {
      app_log("NTU value: %.2f\n", sensor_ntu_value);
    } else {
      app_log("Sensor read failed\n");
    }
  }
}

void turbidity_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                   void *data)
{
  (void) data;
  (void) timer;

  turbidity_timer_expire = true;
}
