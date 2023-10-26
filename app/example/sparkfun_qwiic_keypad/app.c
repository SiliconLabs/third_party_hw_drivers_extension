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
#include "app.h"

static volatile bool new_keypad_event = false;
static sl_sleeptimer_timer_handle_t button_read_timer;

#ifdef SAPRKFUN_KEYPAD_INT_PIN_EN

/***************************************************************************//**
 * Callback for the interrupt.
 ******************************************************************************/
void app_sparkfun_buttonEvent_callback(const uint8_t pin)
{
  if (pin == SPARKFUN_KEYPAD_GPIO_INT_PIN) {
    new_keypad_event = true;
  }
}

#else //SAPRKFUN_KEYPAD_INT_PIN_EN

/***************************************************************************//**
 * Callback for the timer.
 ******************************************************************************/
static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;
  new_keypad_event = true;
}

/***************************************************************************//**
 * Initialize the sleep timer.
 ******************************************************************************/
void app_init_sleeptimer(void)
{
  sl_status_t error_code;

  error_code = sl_sleeptimer_start_periodic_timer_ms(&button_read_timer,
                                                     BUTTON_EVENT_CHECK_FREQUENCY_MS,
                                                     app_timer_callback,
                                                     (void *)NULL,
                                                     0,
                                                     0);
  if (error_code == SL_STATUS_OK) {
    printf("Sleep timer initialized successfully.\n");
  } else if (error_code == SL_STATUS_NULL_POINTER) {
    printf("Error, invslid sleeptimer handle.\n");
  } else if (error_code == SL_STATUS_INVALID_STATE) {
    printf("Error, sleeptimer already running.\n");
  } else if (error_code == SL_STATUS_INVALID_PARAMETER) {
    printf("Error, invalid sleeptimer time_out parameter.\n");
  }
}

#endif //SAPRKFUN_KEYPAD_INT_PIN_EN

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#ifdef SAPRKFUN_KEYPAD_INT_PIN_EN
  app_init_sleeptimer();
#endif //SAPRKFUN_KEYPAD_INT_PIN_EN

  app_init_keypad();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (new_keypad_event == true) {
    new_keypad_event = false;
    app_handle_new_button();
  }
}

/***************************************************************************//**
 * Initialize the keypad.
 ******************************************************************************/
void app_init_keypad(void)
{
  sl_status_t status;
  status = sparkfun_keypad_init(sl_i2cspm_qwiic, sparkfun_keypad_get_address());
  if (status == SL_STATUS_INITIALIZATION) {
    uint8_t address = 0;
    uint8_t num_dev = 0;
    printf("Keypad not found on the specified address.\n");
    status = sparkfun_keypad_scan_address(&address, &num_dev);
    if (status == SL_STATUS_NULL_POINTER) {
      printf("Keypad address scanner invallid input parameter.\n");
    } else {
      printf("Keypad address found!\n");
    }
    status = sparkfun_keypad_init(sl_i2cspm_qwiic, address);
    if (status == SL_STATUS_OK) {
      printf("I2C initialized successfully\n");
    } else {
      printf("I2C initialization failed\n");
    }
  } else if (status == SL_STATUS_NULL_POINTER) {
    printf("I2C initialization failed, null pointer error\n");
  } else if (status == SL_STATUS_INVALID_PARAMETER) {
    printf("I2C initialization error, invalid address\n");
  } else {
    printf("I2C initialized successfully\n");
  }
}

/***************************************************************************//**
 * This function handles the interfacing with the keypad.
 ******************************************************************************/
void app_handle_new_button(void)
{
  uint8_t new_btn = 0;
  sparkfun_keypad_update_fifo();
  sparkfun_keypad_read_last_button(&new_btn);
  if (new_btn > 0) {
    printf("The last pressed button: %c\n", new_btn);
  }
}
