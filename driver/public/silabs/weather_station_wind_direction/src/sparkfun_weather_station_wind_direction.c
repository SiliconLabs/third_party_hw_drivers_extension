/***************************************************************************//**
 * @file sparkfun_weather_station_wind_direction.c
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
#include "sparkfun_weather_station_wind_direction.h"
#include "sparkfun_weather_station_wind_direction_config.h"

#include <stddef.h>

#include "em_iadc.h"
#include "em_cmu.h"
#include "em_emu.h"

#include "sl_status.h"

static void sparkfun_weatherstation_winddirection_init_CMU(void);
static sl_status_t sparkfun_weatherstation_winddirection_setbusalloc(
  GPIO_Port_TypeDef port,
  uint8_t pin);
static sl_status_t sparkfun_weatherstation_winddirection_init_ADC(void);

// Raw IADC conversion result
static volatile IADC_Result_t sample;

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
volatile bool conversion_ready = false;
#endif // #if defined(SL_CATALOG_POWER_MANAGER_PRESENT)

static uint32_t direction_resistances[] =
{
  1580, 1620, 1070, 1140,
  860, 965, 745, 800,
  780, 1400, 1320, 1800,
  1710, 1910, 1850, 1970
};

/************************************************************************
 *    Init function
 *****************************************************************************/
sl_status_t sparkfun_weatherstation_winddirection_init(void)
{
  sl_status_t status = SL_STATUS_FAIL;

  sparkfun_weatherstation_winddirection_init_CMU();
  status = sparkfun_weatherstation_winddirection_init_ADC();

  return status;
}

/************************************************************************
 *    Interrupt handler for finished ADC conversion
 *****************************************************************************/
void IADC_IRQHandler(void)
{
  sample = IADC_pullSingleFifoResult(IADC0);
  IADC_clearInt(IADC0, IADC_IF_SINGLEDONE);

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  conversion_ready = true;
#endif // #if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
}

/************************************************************************
 *    Read direction of wind
 *****************************************************************************/
sl_status_t sparkfun_weatherstation_winddirection_read_direction(
  float *wind_direction_degrees)
{
  if (wind_direction_degrees == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  uint32_t distance_of_closest = UINT32_MAX;
  uint8_t id_of_closest = 0;
  uint8_t i = 0;

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  conversion_ready = false;
#endif // #if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  // Read IADC
  IADC_command(IADC0, iadcCmdStartSingle);
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  while (!conversion_ready)
  {}
#else
  // Wait for IADC interrupt with CPU stopped
  EMU_EnterEM1();
#endif // #if defined(SL_CATALOG_POWER_MANAGER_PRESENT)

  for (i = 0; i < SPARKFUN_WIND_DIRECTION_COUNT; i++) {
    uint32_t distance;
    if (direction_resistances[i] > sample.data) {
      distance = direction_resistances[i] - sample.data;
    } else {
      distance = sample.data - direction_resistances[i];
    }
    if (distance < distance_of_closest) {
      distance_of_closest = distance;
      id_of_closest = i;
    }
  }
  *wind_direction_degrees = SPARKFUN_WIND_DIRECTION_INCREMENTS * id_of_closest;

  return SL_STATUS_OK;
}

/************************************************************************
 *    Init Clock Management Unit
 *****************************************************************************/
static void sparkfun_weatherstation_winddirection_init_CMU(void)
{
  CMU_ClockEnable(cmuClock_IADC0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  CMU_ClockSelectSet(cmuClock_IADCCLK, cmuSelect_FSRCO);
}

/************************************************************************
 *    Set bus allocation for ADC based on which port and pin is selected
 *****************************************************************************/
static sl_status_t sparkfun_weatherstation_winddirection_setbusalloc(
  GPIO_Port_TypeDef port,
  uint8_t pin)
{
  switch (port) {
    case gpioPortA:
      if (pin % 2 == 0) {
        GPIO->ABUSALLOC |= GPIO_ABUSALLOC_AEVEN0_ADC0;
      } else {
        GPIO->ABUSALLOC |= GPIO_ABUSALLOC_AODD0_ADC0;
      }
      break;
    case gpioPortB:
      if (pin % 2 == 0) {
        GPIO->BBUSALLOC |= GPIO_BBUSALLOC_BEVEN0_ADC0;
      } else {
        GPIO->BBUSALLOC |= GPIO_BBUSALLOC_BODD0_ADC0;
      }
      break;
    case gpioPortC:
    case gpioPortD:
      if (pin % 2 == 0) {
        GPIO->CDBUSALLOC |= GPIO_CDBUSALLOC_CDEVEN0_ADC0;
      } else {
        GPIO->CDBUSALLOC |= GPIO_CDBUSALLOC_CDODD0_ADC0;
      }
      break;
    default:
      return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

/************************************************************************
 *    Init IADC
 *****************************************************************************/
static sl_status_t sparkfun_weatherstation_winddirection_init_ADC(void)
{
  sl_status_t status = SL_STATUS_FAIL;

  IADC_Init_t init = IADC_INIT_DEFAULT;
  IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
  IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;

  IADC_SingleInput_t singleInput = IADC_SINGLEINPUT_DEFAULT;

  init.srcClkPrescale = IADC_calcSrcClkPrescale(IADC0, CLK_SRC_ADC_FREQ, 0);

  init.warmup = iadcWarmupNormal;

  initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;
  initAllConfigs.configs[0].vRef = 3300;
  initAllConfigs.configs[0].osrHighSpeed = iadcCfgOsrHighSpeed2x;
  initAllConfigs.configs[0].analogGain = iadcCfgAnalogGain0P5x;

  initAllConfigs.configs[0].adcClkPrescale = IADC_calcAdcClkPrescale(IADC0,
                                                                     CLK_ADC_FREQ,
                                                                     0,
                                                                     iadcCfgModeNormal,
                                                                     init.
                                                                     srcClkPrescale);

  singleInput.posInput =
    IADC_portPinToPosInput(
      SPARKFUN_WEATHER_STATION_WIND_DIRECTION_GPIO_ANALOG_PORT,
      SPARKFUN_WEATHER_STATION_WIND_DIRECTION_GPIO_ANALOG_PIN);
  singleInput.negInput = iadcNegInputGnd;
  status =
    sparkfun_weatherstation_winddirection_setbusalloc(
      SPARKFUN_WEATHER_STATION_WIND_DIRECTION_GPIO_ANALOG_PORT,
      SPARKFUN_WEATHER_STATION_WIND_DIRECTION_GPIO_ANALOG_PIN);

  if (status != SL_STATUS_OK) {
    return status;
  }

  IADC_init(IADC0, &init, &initAllConfigs);
  IADC_initSingle(IADC0, &initSingle, &singleInput);

  IADC_clearInt(IADC0, _IADC_IF_MASK);
  IADC_enableInt(IADC0, IADC_IEN_SINGLEDONE);
  NVIC_ClearPendingIRQ(IADC_IRQn);
  NVIC_EnableIRQ(IADC_IRQn);

  return status;
}
