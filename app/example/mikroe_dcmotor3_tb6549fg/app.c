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

#include "mikroe_tb6549fg.h"
#include "app_log.h"
#include "sl_pwm_instances.h"
#include "sl_sleeptimer.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

uint8_t dcmotor3_direction = 1;
void app_init(void)
{
  if (mikroe_tb6549fg_init(&sl_pwm_mikroe) == SL_STATUS_OK) {
    app_log("DC Motor 3 Click initializes successfully\n");
  }
  mikroe_tb6549fg_set_duty_cycle(0.0);
  mikroe_tb6549fg_pwm_start();
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_tb6549fg_enable();
  sl_sleeptimer_delay_millisecond(1000);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static int8_t duty_cnt = 1;
  static int8_t duty_inc = 1;
  float duty = duty_cnt / 10.0;

  if (dcmotor3_direction == 1) {
    mikroe_tb6549fg_clockwise();
    app_log(">>>> CLOCKWISE \n");
    mikroe_tb6549fg_enable();
  } else {
    mikroe_tb6549fg_counter_clockwise();
    app_log("<<<< COUNTER CLOCKWISE \n");
    mikroe_tb6549fg_enable();
  }
  mikroe_tb6549fg_set_duty_cycle(duty);
  app_log("Duty: %d%%\r\n", ( uint16_t )(duty_cnt * 10));
  sl_sleeptimer_delay_millisecond(500);

  if (10 == duty_cnt) {
    duty_inc = -1;
  } else if (0 == duty_cnt) {
    duty_inc = 1;

    if (dcmotor3_direction == 1) {
      dcmotor3_direction = 0;
    } else if (dcmotor3_direction == 0) {
      dcmotor3_direction = 1;
    }
  }
  duty_cnt += duty_inc;
}
