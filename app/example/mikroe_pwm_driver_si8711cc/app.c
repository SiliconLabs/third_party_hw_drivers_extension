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
#include "sl_pwm_instances.h"
#include "mikroe_si8711cc.h"
#include "sl_sleeptimer.h"

static const float DUTY_CYCLE_STEP = 0.1;
static const uint16_t FREQUENCY = 1000;
static const uint16_t TIMER_PERIOD = 2000;

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool timer_trigger = false;
static uint8_t step_count = 1;
static float duty_cycle;

static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                               void *data);

void app_init(void)
{
  sl_status_t stt;

  app_log("Hello World - PWM Driver Click !!!\r\n");

  if (mikroe_si8711cc_init(&sl_pwm_mikroe) == SL_STATUS_OK) {
    app_log("PWM Driver Click board driver init successfully\r\n");

    stt = sl_sleeptimer_restart_periodic_timer_ms(&app_timer_handle,
                                                  TIMER_PERIOD,
                                                  app_timer_callback,
                                                  NULL,
                                                  0,
                                                  0);

    app_log("sl_sleeptimer_restart_periodic_timer = 0x%lx\r\n", stt);
    duty_cycle = step_count * DUTY_CYCLE_STEP;
    mikroe_si8711cc_set_frequency(FREQUENCY);
    mikroe_si8711cc_pwm_start();

    app_log("mikroe_si8711cc_pwm_start \r\n");
  } else {
    app_log("PWM Driver Click board driver init failed," \
            " please run program again.\r\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (timer_trigger) {
    timer_trigger = false;

    mikroe_si8711cc_set_duty_cycle(duty_cycle);
    app_log("PWM Driver set duty = %d%%\r\n", (uint16_t)(duty_cycle * 100));

    step_count++;
    duty_cycle = step_count * DUTY_CYCLE_STEP;

    if (step_count >= 10) {
      step_count = 1;
      duty_cycle = 0.1;
    }
  }
}

static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;

  timer_trigger = true;
}
