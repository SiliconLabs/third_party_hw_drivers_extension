/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "app_log.h"
#include "sl_i2cspm_instances.h"
#include "sl_spidrv_instances.h"
#include "em_iadc.h"
#include "sl_sleeptimer.h"
#include "app_assert.h"
#include "mikroe_tb9053ftg.h"

#define APP_SET_MODE_TIMER_TIMEOUT 3000
#define APP_SET_DUTY_TIMER_TIMEOUT 500

static volatile bool duty_changed = false;
static volatile uint8_t mode;

static sl_sleeptimer_timer_handle_t set_mode_timer;
static sl_sleeptimer_timer_handle_t set_duty_timer;

static void application_task(void);
static void app_set_mode_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data);
static void app_set_duty_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  app_log("Mikroe DC Motor 26 Click Driver - Example\n");
  app_log("======= Application initialization =======\n");

  sc = mikroe_tb9053ftg_init(sl_spidrv_mikroe_handle,
                             sl_i2cspm_mikroe,
                             IADC0);
  if (SL_STATUS_OK != sc) {
    app_log("DC Motor 26 Click initialized failed!\n");
    return;
  } else {
    app_log("DC Motor 26 Click initialized successfully!\n");
  }

  mikroe_tb9053ftg_default_cfg();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  application_task();
}

/***************************************************************************//**
 * Application task function.
 ******************************************************************************/
static void application_task(void)
{
  sl_status_t sc;
  bool is_running, mode_changed;
  uint16_t duty;
  float current;

  mode = MIKROE_TB9053FTG_MODE_OUTPUT_OFF;
  sc = mikroe_tb9053ftg_set_ch1_operation_mode(mode);
  app_assert_status(sc);
  app_log("\nCH1 MODE: OFF\n");
  app_log("Change to FORWARD MODE after 3 seconds ...\n");
  sc = sl_sleeptimer_start_timer_ms(&set_mode_timer,
                                    APP_SET_MODE_TIMER_TIMEOUT,
                                    app_set_mode_timer_callback,
                                    NULL,
                                    0,
                                    0);
  app_assert_status(sc);
  sc = sl_sleeptimer_is_timer_running(&set_duty_timer, &is_running);
  app_assert_status(sc);
  if (!is_running) {
    sc = sl_sleeptimer_start_periodic_timer_ms(&set_duty_timer,
                                               APP_SET_DUTY_TIMER_TIMEOUT,
                                               app_set_duty_timer_callback,
                                               NULL,
                                               0,
                                               0);
    app_assert_status(sc);
  }

  mode_changed = true;
  while (mode_changed) {
    if (MIKROE_TB9053FTG_MODE_FORWARD == mode) {
      sc = mikroe_tb9053ftg_set_ch1_operation_mode(mode);
      app_assert_status(sc);
      app_log("\nCH1 MODE: FORWARD\n");
      sc = mikroe_tb9053ftg_set_cm_sel_pin(MIKROE_TB9053FTG_PIN_LOW_LEVEL);
      app_assert_status(sc);
      while (duty < MIKROE_TB9053FTG_CONFIG56_DUTY_PERIOD_MAX) {
        if (duty_changed) {
          sc = mikroe_tb9053ftg_set_ch1_duty_period(duty);
          app_assert_status(sc);
          app_log("Duty: %d\n", duty);
          sc = mikroe_tb9053ftg_get_motor_current(&current);
          app_assert_status(sc);
          app_log("Current: %1.2f mA\n", current * 1000);
          duty += 100;
          duty_changed = false;
        }
      }
      duty = 0;
      sl_sleeptimer_restart_timer_ms(&set_mode_timer,
                                     3000,
                                     app_set_mode_timer_callback,
                                     NULL,
                                     0,
                                     0);
      mode_changed = false;
      app_log("Change to BRAKE MODE after 3 seconds ...\n");
    }
  }

  mode_changed = true;
  while (mode_changed) {
    if (MIKROE_TB9053FTG_MODE_BRAKE == mode) {
      sc = mikroe_tb9053ftg_set_ch1_operation_mode(mode);
      app_assert_status(sc);
      app_log("\nCH1 MODE: BRAKE\n");
      sl_sleeptimer_restart_timer_ms(&set_mode_timer, 3000,
                                     app_set_mode_timer_callback,
                                     NULL,
                                     0,
                                     0);
      app_log("Change to REVERSE MODE after 3 seconds ...\n");
      mode_changed = false;
    }
  }

  mode_changed = true;
  while (mode_changed) {
    if (MIKROE_TB9053FTG_MODE_REVERSE == mode) {
      sc = mikroe_tb9053ftg_set_ch1_operation_mode(mode);
      app_assert_status(sc);
      sc = mikroe_tb9053ftg_set_cm_sel_pin(MIKROE_TB9053FTG_PIN_HIGH_LEVEL);
      app_assert_status(sc);
      app_log("\nCH1 MODE: REVERSE\n");
      while (duty < MIKROE_TB9053FTG_CONFIG56_DUTY_PERIOD_MAX) {
        if (duty_changed) {
          mikroe_tb9053ftg_set_ch1_duty_period(duty);
          app_log("Duty: %d\n", duty);
          mikroe_tb9053ftg_get_motor_current(&current);
          app_log("Current: %1.2f mA\n", current * 1000);
          duty += 100;
          duty_changed = false;
        }
        sl_sleeptimer_stop_timer(&set_mode_timer);
        mode_changed = false;
      }
    }
  }
}

/***************************************************************************//**
 * App set mode timer callback.
 ******************************************************************************/
static void app_set_mode_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data)
{
  (void) timer;
  (void) data;

  switch (mode) {
    case MIKROE_TB9053FTG_MODE_OUTPUT_OFF:
      mode = MIKROE_TB9053FTG_MODE_FORWARD;
      break;
    case MIKROE_TB9053FTG_MODE_FORWARD:
      mode = MIKROE_TB9053FTG_MODE_BRAKE;
      break;
    case MIKROE_TB9053FTG_MODE_BRAKE:
      mode = MIKROE_TB9053FTG_MODE_REVERSE;
      break;
    default:
      break;
  }
}

/***************************************************************************//**
 * App set duty timer callback.
 ******************************************************************************/
static void app_set_duty_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data)
{
  (void) timer;
  (void) data;

  duty_changed = true;
}
