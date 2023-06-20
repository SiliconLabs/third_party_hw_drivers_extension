/***************************************************************************//**
 * @file mikroe_a3967.h
 * @brief Mikroe Stepper driver.
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with
 * the specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#include "mikroe_a3967.h"
#include "mikroe_a3967_config.h"
#include "sl_sleeptimer.h"
#include "em_gpio.h"

// -----------------------------------------------------------------------------
//                          Static Variables Declarations
// -----------------------------------------------------------------------------
static sl_sleeptimer_timer_handle_t mikroe_a3967_timer_handle;
static uint32_t mikroe_a3967_step_frequency =
  MIKROE_A3967_STEP_FREQUENCY;
static volatile uint32_t mikroe_a3967_pulses_count;

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------
static void mikroe_a3967_run_callback(sl_sleeptimer_timer_handle_t *handle,
                                      void *data);

static void mikroe_a3967_step_callback(sl_sleeptimer_timer_handle_t *handle,
                                       void *data);

static uint32_t mikroe_a3967_step_freq_to_tick(void);

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 *  Initializes function for driver.
 ******************************************************************************/
sl_status_t mikroe_a3967_init(void)
{
  sl_status_t sc = SL_STATUS_OK;

  GPIO_PinModeSet(MIKROE_A3967_STEP_PORT,
                  MIKROE_A3967_STEP_PIN,
                  gpioModePushPull,
                  0);

  GPIO_PinModeSet(MIKROE_A3967_DIR_PORT,
                  MIKROE_A3967_DIR_PIN,
                  gpioModePushPull,
                  0);

  GPIO_PinModeSet(MIKROE_A3967_MS1_PORT,
                  MIKROE_A3967_MS1_PIN,
                  gpioModePushPull,
                  0);

  GPIO_PinModeSet(MIKROE_A3967_MS2_PORT,
                  MIKROE_A3967_MS2_PIN,
                  gpioModePushPull,
                  0);
  return sc;
}

/***************************************************************************//**
 *    Configure stepper motor mode to run in full/half and micro-steps
 ******************************************************************************/
sl_status_t mikroe_a3967_config_mode(mikroe_a3967_mode_t mode)
{
  sl_status_t sc = SL_STATUS_OK;

  if (mikroe_a3967_get_state()) {
    return SL_STATUS_INVALID_STATE;
  }

  switch (mode)
  {
    case MIKROE_A3967_FULL_STEP:
      GPIO_PinOutClear(MIKROE_A3967_MS1_PORT, MIKROE_A3967_MS1_PIN);
      GPIO_PinOutClear(MIKROE_A3967_MS2_PORT, MIKROE_A3967_MS2_PIN);
      break;

    case MIKROE_A3967_HALF_STEP:
      GPIO_PinOutSet(MIKROE_A3967_MS1_PORT, MIKROE_A3967_MS1_PIN);
      GPIO_PinOutClear(MIKROE_A3967_MS2_PORT, MIKROE_A3967_MS2_PIN);
      break;

    case MIKROE_A3967_QUARTER_STEP:
      GPIO_PinOutClear(MIKROE_A3967_MS1_PORT, MIKROE_A3967_MS1_PIN);
      GPIO_PinOutSet(MIKROE_A3967_MS2_PORT, MIKROE_A3967_MS2_PIN);
      break;

    case MIKROE_A3967_EIGHTH_STEP:
      GPIO_PinOutSet(MIKROE_A3967_MS1_PORT, MIKROE_A3967_MS1_PIN);
      GPIO_PinOutSet(MIKROE_A3967_MS2_PORT, MIKROE_A3967_MS2_PIN);
      break;
  }

  return sc;
}

/***************************************************************************//**
 * Returns with the inner state of the driver.
 ******************************************************************************/
bool mikroe_a3967_get_state(void)
{
  bool state = false;

  sl_sleeptimer_is_timer_running(&mikroe_a3967_timer_handle,
                                 &state);
  return state;
}

/***************************************************************************//**
 * Sets the direction of rotation.
 ******************************************************************************/
sl_status_t mikroe_a3967_set_direction(mikroe_a3967_direction_t dir)
{
  sl_status_t sc = SL_STATUS_OK;

  if (mikroe_a3967_get_state()) {
    return SL_STATUS_INVALID_STATE;
  }
  if (dir == MIKROE_A3967_COUNTERCLOCKWISE) {
    GPIO_PinOutSet(MIKROE_A3967_DIR_PORT, MIKROE_A3967_DIR_PIN);
  } else {
    GPIO_PinOutClear(MIKROE_A3967_DIR_PORT, MIKROE_A3967_DIR_PIN);
  }

  return sc;
}

/***************************************************************************//**
 * Sets the speed of step motor.
 ******************************************************************************/
sl_status_t mikroe_a3967_set_step_frequency(uint32_t freq)
{
  sl_status_t sc = SL_STATUS_OK;

  if (mikroe_a3967_get_state()) {
    return SL_STATUS_INVALID_STATE;
  }

  if ((freq > MIKROE_A3967_STEP_MAX_FREQUENCY)
      || (freq < MIKROE_A3967_STEP_MIN_FREQUENCY)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  mikroe_a3967_step_frequency = freq;

  return sc;
}

/***************************************************************************//**
 *    Starts the motor.
 ******************************************************************************/
sl_status_t mikroe_a3967_start(void)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t timeout;

  if (mikroe_a3967_get_state()) {
    return SL_STATUS_INVALID_STATE;
  }

  GPIO_PinOutClear(MIKROE_A3967_STEP_PORT, MIKROE_A3967_STEP_PIN);

  timeout = mikroe_a3967_step_freq_to_tick();
  sc = sl_sleeptimer_start_periodic_timer(&mikroe_a3967_timer_handle,
                                          timeout,
                                          mikroe_a3967_run_callback,
                                          NULL,
                                          0,
                                          0);

  return sc;
}

/***************************************************************************//**
 * Stops the motor.
 ******************************************************************************/
void mikroe_a3967_stop(void)
{
  sl_sleeptimer_stop_timer(&mikroe_a3967_timer_handle);
}

/***************************************************************************//**
 * Moves the motor by a given number of steps. This is a non-blocking function.
 ******************************************************************************/
sl_status_t mikroe_a3967_step(uint16_t step)
{
  sl_status_t sc = SL_STATUS_OK;
  uint16_t timeout;

  if (mikroe_a3967_get_state()) {
    sc = SL_STATUS_INVALID_STATE;
  }
  GPIO_PinOutClear(MIKROE_A3967_STEP_PORT, MIKROE_A3967_STEP_PIN);

  timeout = mikroe_a3967_step_freq_to_tick();
  mikroe_a3967_pulses_count = step;
  sc = sl_sleeptimer_start_periodic_timer(&mikroe_a3967_timer_handle,
                                          timeout,
                                          mikroe_a3967_step_callback,
                                          NULL,
                                          0,
                                          0);

  return sc;
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

static void mikroe_a3967_step_callback(sl_sleeptimer_timer_handle_t *handle,
                                       void *data)
{
  (void) handle;
  (void) data;
  mikroe_a3967_pulses_count--;

  GPIO_PinOutToggle(MIKROE_A3967_STEP_PORT, MIKROE_A3967_STEP_PIN);
  if (mikroe_a3967_pulses_count == 0) {
    sl_sleeptimer_stop_timer(&mikroe_a3967_timer_handle);
  }
}

static void mikroe_a3967_run_callback(sl_sleeptimer_timer_handle_t *handle,
                                      void *data)
{
  (void) handle;
  (void) data;

  GPIO_PinOutToggle(MIKROE_A3967_STEP_PORT, MIKROE_A3967_STEP_PIN);
}

/*******************************************************************************
 * Converts STEP Frequency in ticks.
 ******************************************************************************/
static uint32_t mikroe_a3967_step_freq_to_tick(void)
{
  uint32_t timer_frequency = sl_sleeptimer_get_timer_frequency();

  return timer_frequency / mikroe_a3967_step_frequency;
}
