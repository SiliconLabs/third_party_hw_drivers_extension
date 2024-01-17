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

#include "mikroe_l9958.h"
#include "app_log.h"
#include "sl_spidrv_instances.h"
#include "sl_pwm_instances.h"
#include "sl_sleeptimer.h"
#include "app_assert.h"

#define APP_TIMER_TIMEOUT 500

static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static void app_timer_handle_callback(sl_sleeptimer_timer_handle_t *handle,
                                      void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  if (mikroe_l9958_init(sl_spidrv_mikroe_handle,
                        &sl_pwm_mikroe) == SL_STATUS_OK) {
    app_log("DC Motor 24 Click initializes successfully\n");
  }
  sc = mikroe_l9958_default_cfg();
  app_assert_status(sc);
  sl_sleeptimer_delay_millisecond(1000);

  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        APP_TIMER_TIMEOUT,
                                        app_timer_handle_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static int8_t duty_pct = 10;
  static int8_t duty_step = 10;

  if (app_timer_expire) {
    if (SL_STATUS_OK == mikroe_l9958_set_duty_cycle((float)duty_pct / 100)) {
      app_log("Duty cycle: %d\n", duty_pct);
    }
    if ((100 == duty_pct) || (0 == duty_pct)) {
      duty_step = -duty_step;
      if (0 == duty_pct) {
        app_log("Switch direction\n");
        mikroe_l9958_switch_direction();
      }
    }
    duty_pct += duty_step;
    app_timer_expire = false;
  }
}

static void app_timer_handle_callback(sl_sleeptimer_timer_handle_t *handle,
                                      void *data)
{
  (void) handle;
  (void) data;
  app_timer_expire = true;
}
