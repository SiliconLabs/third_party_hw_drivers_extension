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
#include "mikroe_a3967.h"
#include "app_log.h"
#include "sl_sleeptimer.h"

// The 28BYJ-48 motor features a 1/64 reduction gear set.
#define MOTOR_GEAR_RATIO  64

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc = SL_STATUS_OK;

  app_log("Stepper Click demo application!!!\r\n");

  // specifications of the motor
  mikroe_a3967_init();
  mikroe_a3967_set_direction(MIKROE_A3967_CLOCKWISE);
  mikroe_a3967_set_step_frequency(500);
  mikroe_a3967_config_mode(MIKROE_A3967_FULL_STEP);

  app_log("A3967 Stepper Start\r\n");

  // The 28BYJ-48 motor have 64 steps per revolution.
  // So it need (64 * MOTOR_GEAR_RATIO) step to complete one revolution.
  sc = mikroe_a3967_step(16 * MOTOR_GEAR_RATIO);
  if (sc != SL_STATUS_OK) {
    app_log("error code = 0x%lx\r\n", sc);
  }

  // Need to wait until the motor stop
  while (mikroe_a3967_get_state()) {}
  mikroe_a3967_set_direction(MIKROE_A3967_COUNTERCLOCKWISE);
  sc = mikroe_a3967_step(32 * MOTOR_GEAR_RATIO);
  if (sc != SL_STATUS_OK) {
    app_log("error code = 0x%lx\r\n", sc);
  }

  // Need to wait until the motor stop
  while (mikroe_a3967_get_state()) {}
  mikroe_a3967_set_direction(MIKROE_A3967_CLOCKWISE);
  sc = mikroe_a3967_step(64 * MOTOR_GEAR_RATIO);
  if (sc != SL_STATUS_OK) {
    app_log("error code = 0x%lx\r\n", sc);
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
