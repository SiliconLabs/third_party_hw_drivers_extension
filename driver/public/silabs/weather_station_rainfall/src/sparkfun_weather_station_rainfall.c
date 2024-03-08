/***************************************************************************//**
 * @file sparkfun_weather_station_rainfall.c
 * @brief Sparkfun Keypad source file.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include "sparkfun_weather_station_rainfall.h"
#include "sparkfun_weather_station_rainfall_config.h"

#include <stddef.h>

#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "gpiointerrupt.h"

#include "sl_status.h"
#include "sl_sleeptimer.h"

static uint64_t sparkfun_last_rainfall_millis = 0;
static uint32_t sparkfun_rainfall_count = 0;

static void sparkfun_weatherstation_rainfall_sensor_callback(uint8_t intNo);

static void sparkfun_weatherstation_rainfall_init_CMU(void);
static void sparkfun_weatherstation_rainfall_init_GPIO(void);

/************************************************************************
 *    Init function
 *****************************************************************************/
void sparkfun_weatherstation_rainfall_init(void)
{
  sparkfun_weatherstation_rainfall_init_CMU();
  sparkfun_weatherstation_rainfall_init_GPIO();
  sparkfun_weatherstation_rainfall_reset_rainfall_count();
}

/************************************************************************
 *    Reset the count of rainfall events to zero
 *****************************************************************************/
void sparkfun_weatherstation_rainfall_reset_rainfall_count(void)
{
  sparkfun_rainfall_count = 0;
}

/************************************************************************
 *    Get the number of rainfall events since last reset
 *****************************************************************************/
sl_status_t sparkfun_weatherstation_rainfall_get_rainfall_count(uint32_t *count)
{
  if (count == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  // Return total rainfall counts
  *count = sparkfun_rainfall_count;

  return SL_STATUS_OK;
}

/************************************************************************
 *    Get Rainfall amount in mm
 *****************************************************************************/
sl_status_t sparkfun_weatherstation_rainfall_get_rainfall_amount(
  float *rainfall)
{
  if (rainfall == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  // Return total rainfall in mm
  *rainfall = sparkfun_rainfall_count * SPARKFUN_MM_PER_RAINFALL_COUNTS;

  return SL_STATUS_OK;
}

/************************************************************************
 *    Callback for rain event
 *****************************************************************************/
static void sparkfun_weatherstation_rainfall_sensor_callback(uint8_t intNo)
{
  (void) intNo;
  uint64_t millis;
  uint64_t ticks = sl_sleeptimer_get_tick_count64();
  sl_sleeptimer_tick64_to_ms(ticks, &millis);

  // Debounce by checking time since last interrupt
  if ((millis - sparkfun_last_rainfall_millis)
      < SPARKFUN_CALIBRATION_MIN_MILLIS_PER_RAINFALL) {
    // There's not been enough time since the last interrupt, so this is
    // likely just the switch bouncing
    return;
  }

  // Enough time has passed that this is probably a real signal instead of a
  // bounce, so update the time of the last interrupt to be now
  sparkfun_last_rainfall_millis = millis;

  // Increment counter
  sparkfun_rainfall_count++;
}

/************************************************************************
 *    Init Clock Management Unit
 *****************************************************************************/
static void sparkfun_weatherstation_rainfall_init_CMU(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
}

/************************************************************************
 *    Init GPIO
 *****************************************************************************/
static void sparkfun_weatherstation_rainfall_init_GPIO(void)
{
  GPIO_PinModeSet(SPARKFUN_WEATHER_STATION_RAINFALL_INT_PORT,
                  SPARKFUN_WEATHER_STATION_RAINFALL_INT_PIN,
                  gpioModeInputPullFilter,
                  1);

  GPIO_ExtIntConfig(SPARKFUN_WEATHER_STATION_RAINFALL_INT_PORT,
                    SPARKFUN_WEATHER_STATION_RAINFALL_INT_PIN,
                    SPARKFUN_WEATHER_STATION_RAINFALL_INT_PIN,
                    false,
                    true,
                    true);

  GPIOINT_Init();
  GPIOINT_CallbackRegister(SPARKFUN_WEATHER_STATION_RAINFALL_INT_PORT,
                           sparkfun_weatherstation_rainfall_sensor_callback);
}
