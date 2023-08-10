/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "sl_pwm_instances.h"
#include "sl_simple_button_instances.h"
#include "app_log.h"
#include "stepper2_a4988.h"

#define APP_STEPPER_RPM (64)

static a4988_stepper_inst_t app_stepper_instance = A4988_STEPPER_INST_DEFAULT;

static bool btn_press = false;
static uint8_t *a4988_stepper_state_mgs[] = {
  (uint8_t *)"DISABLED",
  (uint8_t *)"IDLE",
  (uint8_t *)"RUNNING",
  (uint8_t *)"SLEEP",
  (uint8_t *)"ERROR",
};

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t stt = SL_STATUS_FAIL;

  app_log("Hello World Stepper2 Click demo app !!!\r\n");

  stt = a4988_stepper_init(&app_stepper_instance, &sl_pwm_mikroe);
  app_log("a4988_stepper_init = 0x%lx\r\n", stt);

  stt = a4988_stepper_set_dir(&app_stepper_instance, CW);
  app_log("a4988_stepper_set_dir clockwise = 0x%lx\r\n", stt);

  stt = a4988_stepper_set_speed(&app_stepper_instance, APP_STEPPER_RPM * 4);
  app_log("a4988_stepper_set_speed = 0x%lx\r\n", stt);

  a4988_stepper_reset(&app_stepper_instance);
  app_log("a4988_stepper_reset\r\n");

  a4988_stepper_start(&app_stepper_instance);
  app_log("a4988_stepper_start\r\n");

  app_log("a4988_stepper_get_state = %s\r\n",
          a4988_stepper_state_mgs[a4988_stepper_get_state(
                                    &app_stepper_instance)]);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static uint8_t state;
  if (btn_press) {
    btn_press = false;
    state = a4988_stepper_get_state(&app_stepper_instance);
    app_log("a4988_stepper_get_state = %s\r\n", a4988_stepper_state_mgs[state]);
  }
}

/***************************************************************************//**
 * Emergency button: if the motor is enabled then a button press disables it.
 * The next button press will enable and start it again.
 ******************************************************************************/
void sl_button_on_change(const sl_button_t *handle)
{
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
    btn_press = true;
    if (a4988_stepper_get_state(&app_stepper_instance) == DISABLED) {
      a4988_stepper_enable(&app_stepper_instance, true);
      a4988_stepper_start(&app_stepper_instance);
    } else {
      a4988_stepper_enable(&app_stepper_instance, false);
    }
  }
}
