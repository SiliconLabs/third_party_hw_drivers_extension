/***************************************************************************//**
 * @file drv_analog_in.h
 * @brief mikroSDK 2.0 Click Peripheral Drivers - Analog IN
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

#ifndef _DRV_ANALOG_IN_H_
#define _DRV_ANALOG_IN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hal_gpio.h"
#include "drv_name.h"

typedef const void *mikroe_adc_handle_t; ///< Created ADC handle type

typedef enum
{
  ADC_SUCCESS = 0,
  ADC_ERROR = (-1)
} analog_in_err_t;

typedef enum
{
  ANALOG_IN_VREF_EXTERNAL = 0,
  ANALOG_IN_VREF_INTERNAL
} analog_in_vref_t;

typedef enum
{
  ANALOG_IN_RESOLUTION_NOT_SET = 0,
  ANALOG_IN_RESOLUTION_6_BIT,
  ANALOG_IN_RESOLUTION_8_BIT,
  ANALOG_IN_RESOLUTION_10_BIT,
  ANALOG_IN_RESOLUTION_12_BIT,
  ANALOG_IN_RESOLUTION_14_BIT,
  ANALOG_IN_RESOLUTION_16_BIT,

  ///< Default resolution
  ANALOG_IN_RESOLUTION_DEFAULT = ANALOG_IN_RESOLUTION_12_BIT
} analog_in_resolution_t;

typedef struct
{
  pin_name_t input_pin;
  analog_in_resolution_t resolution;
  analog_in_vref_t vref_input;
  float vref_value;
} analog_in_config_t;

typedef struct
{
  mikroe_adc_handle_t handle;
  analog_in_config_t config;
} analog_in_t;

void analog_in_configure_default(analog_in_config_t *config);
err_t analog_in_open(analog_in_t *obj, analog_in_config_t *config);
err_t analog_in_set_resolution(analog_in_t *obj,
                               analog_in_resolution_t resolution);
err_t analog_in_set_vref_input(analog_in_t *obj, analog_in_vref_t vref);
err_t analog_in_set_vref_value(analog_in_t *obj, float vref_value);
err_t analog_in_read(analog_in_t *obj, uint16_t *readDatabuf);
err_t analog_in_read_voltage(analog_in_t *obj, float *readDatabuf);
void analog_in_close(analog_in_t *obj);

#ifdef __cplusplus
}
#endif

#endif // _DRV_ANALOG_IN_H_
// ------------------------------------------------------------------------- END
