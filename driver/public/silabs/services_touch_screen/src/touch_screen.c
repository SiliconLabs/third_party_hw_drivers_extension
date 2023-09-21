/***************************************************************************//**
 * @file adafruit_ili9341_touch.h
 * @brief Adafruit ILI9341 TFT LCD with Touchscreen Header File
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
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include "touch_screen.h"
#if CALIBRATION_PRINTF
#include "app_log.h"
#endif

// -----------------------------------------------------------------------------
//                       Macros
// -----------------------------------------------------------------------------
// Set CLK_ADC to 10 MHz
#define CLK_SRC_ADC_FREQ         20000000 // CLK_SRC_ADC
#define CLK_ADC_FREQ             10000000 // CLK_ADC - 10 MHz max in normal mode

#define NUMSAMPLES               6

#define max(a, b) (a) > (b) ? (a) : (b)
#define min(a, b) (a) < (b) ? (a) : (b)

#define calc_adc_pos(port, pin) \
  ((((port) * 16) + pin) + (unsigned int)iadcNegInputPortAPin0)
#define adc_mux_even(ref, adc, bus, index) \
  GPIO->bus ## BUSALLOC |=                 \
    GPIO_ ## bus ## BUSALLOC_ ## bus ## EVEN ## index ## _ ## adc;

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
static IADC_Init_t init = IADC_INIT_DEFAULT;
static IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
static IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;
static IADC_SingleInput_t initSingleInput = IADC_SINGLEINPUT_DEFAULT;

// -----------------------------------------------------------------------------
//                       Local Functions
// -----------------------------------------------------------------------------

/**************************************************************************//**
 * Map range function.
 *****************************************************************************/
static int32_t map_range(int32_t x,
                         int32_t in_min,
                         int32_t in_max,
                         int32_t out_min,
                         int32_t out_max)
{
  float mapped =
    (float)((int64_t)(x - in_min) * (int64_t)(out_max - out_min)
            / (in_max - in_min) + out_min);
  if (out_min <= out_max) {
    return (int32_t)max(min(mapped, (float)out_max), (float)out_min);
  }
  return (int32_t)min(max(mapped, (float)out_max), (float)out_min);
}

/**************************************************************************//**
 * Insert sort function.
 *****************************************************************************/
static void insert_sort(int32_t *array, uint8_t size)
{
  uint8_t j;
  int32_t save;

  for (uint8_t i = 1; i < size; i++) {
    save = array[i];
    for (j = i; j >= 1 && save < array[j - 1]; j--) {
      array[j] = array[j - 1];
    }
    array[j] = save;
  }
}

/***************************************************************************//**
 * @brief
 *  Allocate bus for analog port.
 *
 * @param[in] handle
 *  ADC handle.
 * @param[out] port_index
 *  Port value.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
static sl_status_t allocate_analog_bus_even0(IADC_TypeDef *handle,
                                             GPIO_Port_TypeDef port_index)
{
  GPIO->ABUSALLOC = 0;
  GPIO->BBUSALLOC = 0;
  GPIO->CDBUSALLOC = 0;
  switch (port_index) {
    case gpioPortA:
#if (IADC_COUNT == 1)
      if (handle == IADC0) {
        adc_mux_even(handle, ADC0, A, 0);
        return SL_STATUS_OK;
#elif (IADC_COUNT == 2)
  } else if (handle == IADC1) {
    adc_mux_even(handle, ADC1, A, 0);
    return ADC_SUCCESS;
#endif
      } else {
        return SL_STATUS_FAIL;
      }

    case gpioPortB:
#if (IADC_COUNT == 1)
      if (handle == IADC0) {
        adc_mux_even(handle, ADC0, B, 0);
        return SL_STATUS_OK;
#elif (IADC_COUNT == 2)
  } else if (handle == IADC1) {
    adc_mux_even(handle, ADC1, B, 0);
    return ADC_SUCCESS;
#endif
      } else {
        return SL_STATUS_FAIL;
      }

    case gpioPortC:
    case gpioPortD:
#if (IADC_COUNT == 1)
      if (handle == IADC0) {
        adc_mux_even(handle, ADC0, CD, 0);
        return SL_STATUS_OK;
#elif (IADC_COUNT == 2)
  } else if (handle == IADC1) {
    adc_mux_even(handle, ADC1, CD, 0);
    return ADC_SUCCESS;
#endif
      } else {
        return SL_STATUS_FAIL;
      }

    default:
      return SL_STATUS_FAIL;
  }
}

/***************************************************************************//**
 * @brief
 *  Pin configuration for analog reading.
 *
 * @param[in] handle
 *  ADC handle.
 * @param[in] port
 *  Port value.
 * @param[in] pin
 *  Pin value
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
static void analog_start_read(IADC_TypeDef *handle,
                              GPIO_Port_TypeDef port,
                              uint8_t pin)
{
  allocate_analog_bus_even0(handle, port);
  initSingleInput.posInput = IADC_portPinToPosInput(port, pin);
  IADC_initSingle(IADC0, &initSingle, &initSingleInput);
}

/***************************************************************************//**
 * @brief
 *  Read ADC value from specified pin.
 *
 * @param[in] handle
 *  ADC handle.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
static uint32_t analog_read(IADC_TypeDef *handle)
{
  IADC_command(handle, iadcCmdStartSingle);
  // Wait for conversion to be complete
  // while combined status bits 8 & 6 don't equal 1 and 0 respectively
  while ((handle->STATUS & (_IADC_STATUS_CONVERTING_MASK
                            | _IADC_STATUS_SINGLEFIFODV_MASK))
         != IADC_STATUS_SINGLEFIFODV) {}
  return IADC_pullSingleFifoResult(handle).data;
}

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 * Initialize Touch Screen feature for ILI9341 module.
 *****************************************************************************/
sl_status_t touch_screen_init(void)
{
  CMU_ClockEnable(cmuClock_IADC0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Use the FSRC0 as the IADC clock so it can run in EM2
  CMU_ClockSelectSet(cmuClock_IADCCLK, cmuSelect_FSRCO);

  // Set the prescaler needed for the intended IADC clock frequency
  init.srcClkPrescale = IADC_calcSrcClkPrescale(IADC0, CLK_SRC_ADC_FREQ, 0);

  init.warmup = iadcWarmupNormal;

  initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;
  initAllConfigs.configs[0].vRef = 3300;
  initAllConfigs.configs[0].osrHighSpeed = iadcCfgOsrHighSpeed2x;
  initAllConfigs.configs[0].analogGain = iadcCfgAnalogGain1x;
  initAllConfigs.configs[0].twosComplement = iadcCfgTwosCompUnipolar;

  initSingleInput.posInput = IADC_portPinToPosInput(XM_PORT,
                                                    XM_PIN);

  initSingleInput.negInput = iadcNegInputGnd;

  if (SL_STATUS_FAIL == allocate_analog_bus_even0(IADC0,
                                                  XM_PORT)) {
    return SL_STATUS_FAIL;
  }

  // Initialize IADC
  IADC_init(IADC0, &init, &initAllConfigs);

  // Initialize IADC sing
  IADC_initSingle(IADC0, &initSingle, &initSingleInput);

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Get touch points whenever a touch is detected on the screen.
 *****************************************************************************/
sl_status_t touch_screen_get_point(int rxplate, touch_point_t *ts_point)
{
  int32_t samples[NUMSAMPLES];
  int32_t ts_data;
  int i;

  // XM = 0, XP = 1, YM & YP as input
  GPIO_PinModeSet(XM_PORT,
                  XM_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(XP_PORT,
                  XP_PIN,
                  gpioModePushPull,
                  1);
  GPIO_PinModeSet(YM_PORT,
                  YM_PIN,
                  gpioModeInput,
                  0);
  GPIO_PinModeSet(YP_PORT,
                  YP_PIN,
                  gpioModeInput,
                  0);

  // Fast ARM chips need to allow voltages to settle
  sl_udelay_wait(20);

  analog_start_read(IADC0,
                    YP_PORT,
                    YP_PIN);
  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = analog_read(IADC0);
  }
  insert_sort(samples, NUMSAMPLES);

  ts_data = (4096 - samples[NUMSAMPLES / 2]);
#if CALIBRATION_PRINTF
  app_log("\rx: %ld\n", ts_data);
#endif
  ts_point->x = map_range(ts_data,
                          CALIB_X_MIN,
                          CALIB_X_MAX,
                          0,
                          TOUCHSCREEN_WIDTH - 1);

  // YM = 0, YP = 1, XM & XP as input
  GPIO_PinModeSet(YM_PORT,
                  YM_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(YP_PORT,
                  YP_PIN,
                  gpioModePushPull,
                  1);
  GPIO_PinModeSet(XM_PORT,
                  XM_PIN,
                  gpioModeInput,
                  0);
  GPIO_PinModeSet(XP_PORT,
                  XP_PIN,
                  gpioModeInput,
                  0);

  // Fast ARM chips need to allow voltages to settle
  sl_udelay_wait(20);

  analog_start_read(IADC0,
                    XM_PORT,
                    XM_PIN);
  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = analog_read(IADC0);
  }
  insert_sort(samples, NUMSAMPLES);

  ts_data = (4096 - samples[NUMSAMPLES / 2]);
#if CALIBRATION_PRINTF
  app_log("\ry: %ld\n", ts_data);
#endif
  ts_point->y = map_range(ts_data,
                          CALIB_Y_MIN,
                          CALIB_Y_MAX,
                          0,
                          TOUCHSCREEN_HEIGHT - 1);

  // XP = 0, YM = 1, XM = Hi-Z& YP as input
  GPIO_PinModeSet(XP_PORT,
                  XP_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(YM_PORT,
                  YM_PIN,
                  gpioModePushPull,
                  1);
  GPIO_PinModeSet(XM_PORT,
                  XM_PIN,
                  gpioModeInput,
                  0);
  GPIO_PinModeSet(YP_PORT,
                  YP_PIN,
                  gpioModeInput,
                  0);

  if (rxplate != 0) {
    analog_start_read(IADC0,
                      XM_PORT,
                      XM_PIN);
    int32_t z1 = analog_read(IADC0);

    analog_start_read(IADC0,
                      YP_PORT,
                      YP_PIN);
    int32_t z2 = analog_read(IADC0);

    // now read the x
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= ts_point->x;
    rtouch *= rxplate;
    rtouch /= 4096;

    ts_point->z = rtouch;
  }
  return SL_STATUS_OK;
}
