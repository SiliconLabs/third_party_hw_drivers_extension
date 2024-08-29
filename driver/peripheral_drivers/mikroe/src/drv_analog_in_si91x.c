/***************************************************************************//**
 * @file drv_analog_in_si91x.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - Analog IN for Si91x
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "sl_status.h"
#include "drv_analog_in.h"
#include "sl_si91x_adc.h"
#include "sl_adc_instances.h"

#define ADC_MAX_OP_VALUE      4096     // Maximum output value get from adc
#define AUXADC_DATA_TWELFTH   BIT(11)  // Reading twelfth bit of AUXADC_DATA reg

static analog_in_t *_owner = NULL;
static sl_adc_config_t drv_adc_config;
static volatile boolean_t data_sample_complete_flag = false;

static void callback_event(uint8_t channel_no, uint8_t event);
static err_t _acquire(analog_in_t *obj, bool obj_open_state);
static err_t hal_adc_read(analog_in_t *obj, uint16_t *data);

void analog_in_configure_default(analog_in_config_t *config)
{
  config->input_pin = 0xFFFFFFFF;
  config->resolution = ANALOG_IN_RESOLUTION_DEFAULT;
  config->vref_input = ANALOG_IN_VREF_EXTERNAL;
  config->vref_value = 3.3;
}

err_t analog_in_open(analog_in_t *obj, analog_in_config_t *config)
{
  sl_status_t status;
  analog_in_config_t *p_config = &obj->config;

  memcpy(p_config, config, sizeof(analog_in_config_t));

  if (_acquire(obj, true) == ACQUIRE_FAIL) {
    return ADC_ERROR;
  }

  drv_adc_config.num_of_channel_enable = 1;
  drv_adc_config.operation_mode = SL_ADC_STATIC_MODE;
  sl_adc_channel_config.channel = *(uint8_t *)obj->handle;
  sl_adc_channel_config.num_of_samples[sl_adc_channel_config.channel] = 1;

  // Initialize ADC
  status = sl_si91x_adc_init(sl_adc_channel_config,
                             drv_adc_config,
                             obj->config.vref_value);
  if (status != SL_STATUS_OK) {
    return ADC_ERROR;
  }
  // Configure ADC channel.
  status = sl_si91x_adc_set_channel_configuration(sl_adc_channel_config,
                                                  drv_adc_config);
  if (status != SL_STATUS_OK) {
    return ADC_ERROR;
  }
  // Register user callback function
  status = sl_si91x_adc_register_event_callback(callback_event);
  if (status != SL_STATUS_OK) {
    return ADC_ERROR;
  }

  return ADC_SUCCESS;
}

err_t analog_in_set_resolution(analog_in_t *obj,
                               analog_in_resolution_t resolution)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    obj->config.resolution = resolution;
    return ADC_SUCCESS;
  }

  return ADC_ERROR;
}

err_t analog_in_set_vref_input(analog_in_t *obj, analog_in_vref_t vref)
{
  (void) vref;
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return ADC_ERROR;
  }
  // TODO
  return ADC_SUCCESS;
}

err_t analog_in_set_vref_value(analog_in_t *obj, float vref_value)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return ADC_ERROR;
  }

  obj->config.vref_value = vref_value;
  if (sl_si91x_adc_init(sl_adc_channel_config,
                        drv_adc_config,
                        vref_value) != SL_STATUS_OK) {
    return ADC_ERROR;
  }

  return ADC_SUCCESS;
}

err_t analog_in_read(analog_in_t *obj, uint16_t *readDatabuf)
{
  uint16_t adc_value;

  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return ADC_ERROR;
  }

  if (hal_adc_read(obj, &adc_value) != SL_STATUS_OK) {
    return ADC_ERROR;
  }

  switch (obj->config.resolution) {
    case ANALOG_IN_RESOLUTION_6_BIT:
      *readDatabuf = adc_value >> 6;
      break;
    case ANALOG_IN_RESOLUTION_8_BIT:
      *readDatabuf = adc_value >> 4;
      break;
    case ANALOG_IN_RESOLUTION_10_BIT:
      *readDatabuf = adc_value >> 2;
      break;
    case ANALOG_IN_RESOLUTION_12_BIT:
      *readDatabuf = adc_value;
      break;
    default:
      return ADC_ERROR;
  }

  return ADC_SUCCESS;
}

err_t analog_in_read_voltage(analog_in_t *obj, float *readDatabuf)
{
  uint16_t adc_value;

  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return ADC_ERROR;
  }

  if (hal_adc_read(obj, &adc_value) != SL_STATUS_OK) {
    return ADC_ERROR;
  }

  *readDatabuf = (float)adc_value * obj->config.vref_value / ADC_MAX_OP_VALUE;
  return ADC_SUCCESS;
}

void analog_in_close(analog_in_t *obj)
{
  sl_si91x_adc_deinit(drv_adc_config);
  obj->handle = NULL;
  _owner = NULL;
}

static err_t _acquire(analog_in_t *obj, bool obj_open_state)
{
  err_t status = ACQUIRE_SUCCESS;

  if ((obj_open_state == true) && (_owner == obj)) {
    return ACQUIRE_FAIL;
  }
  if (_owner != obj) {
    _owner = obj;
  }
  return status;
}

static err_t hal_adc_read(analog_in_t *obj, uint16_t *data)
{
  (void)obj;
  uint16_t adc_value;

  // Start the ADC operation
  if (sl_si91x_adc_start(drv_adc_config) != SL_STATUS_OK) {
    return ADC_ERROR;
  }
  // Wait until ADC data acquisition done
  while (data_sample_complete_flag != true) {}
  data_sample_complete_flag = false;
  if (sl_si91x_adc_stop(drv_adc_config) != SL_STATUS_OK) {
    return ADC_ERROR;
  }
  if (sl_si91x_adc_read_data_static(sl_adc_channel_config,
                                    drv_adc_config,
                                    &adc_value) != SL_STATUS_OK) {
    return ADC_ERROR;
  }

  if (adc_value & AUXADC_DATA_TWELFTH) {
    adc_value = (int16_t)(adc_value & (ADC_MASK_VALUE));
  } else {
    adc_value = adc_value | AUXADC_DATA_TWELFTH;
  }

  *data = adc_value;
  return ADC_SUCCESS;
}

/*******************************************************************************
 * Callback event function
 * It is responsible for the event which are triggered by ADC interface
 * @param  event       : INTERNAL_DMA => Single channel data acquisition done.
 *                       ADC_STATIC_MODE_CALLBACK => Static mode adc data
 *                       acquisition done.
 ******************************************************************************/
static void callback_event(uint8_t channel_no, uint8_t event)
{
  (void) channel_no;

  switch (event) {
    case SL_INTERNAL_DMA:
      break;
    case SL_ADC_STATIC_MODE_EVENT:
      data_sample_complete_flag = true;
      break;
  }
}

// ------------------------------------------------------------------------- END
