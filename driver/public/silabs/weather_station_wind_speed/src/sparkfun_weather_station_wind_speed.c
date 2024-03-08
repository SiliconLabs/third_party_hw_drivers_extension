/***************************************************************************//**
 * @file sparkfun_weather_station_wind_speed.c
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
#include "sparkfun_weather_station_wind_speed.h"
#include "sparkfun_weather_station_wind_speed_config.h"

#include <stddef.h>

#include "em_pcnt.h"
#include "em_cmu.h"
#include "em_prs.h"
#include "em_emu.h"
#include "em_gpio.h"

#include "sl_status.h"
#include "sl_sleeptimer.h"

static void sparkfun_weatherstation_windspeed_measurementCallback(
  sl_sleeptimer_timer_handle_t *handle,
  void *data);

static sl_status_t sparkfun_weatherstation_windspeed_getPulse_resetCounter(
  uint32_t *count);

static void sparkfun_weatherstation_windspeed_init_CMU(void);
static void sparkfun_weatherstation_windspeed_init_GPIO(void);
static void sparkfun_weatherstation_windspeed_init_PRS(void);
static void sparkfun_weatherstation_windspeed_init_PCNT(void);

static sl_sleeptimer_timer_handle_t my_sleeptimer_handle;

static float sparkfun_windspeed = 0;

/************************************************************************
 *    Init function
 *****************************************************************************/
void sparkfun_weatherstation_windspeed_init(void)
{
  sparkfun_weatherstation_windspeed_init_CMU();
  sparkfun_weatherstation_windspeed_init_GPIO();
  sparkfun_weatherstation_windspeed_init_PRS();
  sparkfun_weatherstation_windspeed_init_PCNT();
  sl_sleeptimer_start_periodic_timer_ms(&my_sleeptimer_handle,
                                        SPARKFUN_WINDSPEED_CHECK_PERIOD_MS,
                                        sparkfun_weatherstation_windspeed_measurementCallback,
                                        (void *)NULL,
                                        0,
                                        0);
}

/************************************************************************
 *    Get the latest windspeed
 *****************************************************************************/
sl_status_t sparkfun_weatherstation_windspeed_get(float *wind_speed)
{
  if (wind_speed == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  if (sparkfun_windspeed >= 0) {
    *wind_speed = sparkfun_windspeed;
    return SL_STATUS_OK;
  } else {
    return SL_STATUS_FAIL;
  }
}

/************************************************************************
 *    Callback to update the windspeed periodically
 *****************************************************************************/
static void sparkfun_weatherstation_windspeed_measurementCallback(
  sl_sleeptimer_timer_handle_t *handle,
  void *data)
{
  (void) data;
  (void) handle;
  uint32_t count;
  sl_status_t status = SL_STATUS_FAIL;

  status = sparkfun_weatherstation_windspeed_getPulse_resetCounter(&count);

  if (status == SL_STATUS_OK) {
    // 1 pulse/s  == 2.4 km/h
    sparkfun_windspeed = 2.4 * count * 1000
                         / SPARKFUN_WINDSPEED_CHECK_PERIOD_MS;
  } else {
    // indicate the error
    sparkfun_windspeed = -1;
  }
}

/************************************************************************
 *    Get Pulse Counter value, and reset it afterwards
 *****************************************************************************/
static sl_status_t sparkfun_weatherstation_windspeed_getPulse_resetCounter(
  uint32_t *count)
{
  if (PCNT_IsEnabled(PCNT0)) {
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    *count = PCNT_CounterGet(PCNT0);
    PCNT_CounterReset(PCNT0);
    CORE_EXIT_CRITICAL();
  } else {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

/************************************************************************
 *    Init Clock Management Unit
 *****************************************************************************/
static void sparkfun_weatherstation_windspeed_init_CMU(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_PRS, true);
  CMU_ClockEnable(cmuClock_PCNT0, true);
}

/************************************************************************
 *    Init GPIO
 *****************************************************************************/
static void sparkfun_weatherstation_windspeed_init_GPIO(void)
{
  // GPIO INIT
  GPIO_PinModeSet(SPARKFUN_WEATHER_STATION_WIND_SPEED_PORT,
                  SPARKFUN_WEATHER_STATION_WIND_SPEED_PIN,
                  gpioModeInputPullFilter,
                  1);

  GPIO_ExtIntConfig(SPARKFUN_WEATHER_STATION_WIND_SPEED_PORT,
                    SPARKFUN_WEATHER_STATION_WIND_SPEED_PIN,
                    SPARKFUN_WEATHER_STATION_WIND_SPEED_PIN,
                    false,
                    false,
                    false);
}

/************************************************************************
 *    Init Peripheral Reflex System
 *****************************************************************************/
static void sparkfun_weatherstation_windspeed_init_PRS(void)
{
  // PRS INIT
  // Set up GPIO PRS
  PRS_SourceAsyncSignalSet(WINDSPEED_PCNT_PRS_CH,
                           PRS_ASYNC_CH_CTRL_SOURCESEL_GPIO,
                           SPARKFUN_WEATHER_STATION_WIND_SPEED_PIN);
}

/************************************************************************
 *    Init Pulse Counter
 *****************************************************************************/
static void sparkfun_weatherstation_windspeed_init_PCNT(void)
{
  // PCNT INIT
  PCNT_Init_TypeDef pcntInit = PCNT_INIT_DEFAULT;
  PCNT_Filter_TypeDef pcntFilterInit = PCNT_FILTER_DEFAULT;

  pcntInit.mode = pcntModeOvsSingle;      // Oversampling single mode
  pcntInit.top = 50000;
  pcntInit.s1CntDir = false;            // S1 does not affect counter direction,
                                        // using default init setting; count up
  pcntInit.s0PRS = WINDSPEED_PCNT_PRS_CH;
  pcntInit.filter = true;      // Filter GPIO to remove glitching LED behavior

  // Use max filter len for GPIO push button
  pcntFilterInit.filtLen = _PCNT_OVSCTRL_FILTLEN_MASK;

  // Enable PRS0 for PCNT0
  PCNT_PRSInputEnable(PCNT0, pcntPRSInputS0, true);

  // Filter configuration
  PCNT_FilterConfiguration(PCNT0, &pcntFilterInit, true);

  // Enable PCNT0
  PCNT_Init(PCNT0, &pcntInit);
}
