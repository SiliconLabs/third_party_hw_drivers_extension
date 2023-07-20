/***************************************************************************//**
 * @file mikroe_bme688_i2c.c
 * @brief SCL BME688 Prototypes
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

#include "mikroe_bme688_i2c.h"
#include "mikroe_bme688_config_i2c.h"
#include "environment3.h"
#include "sl_status.h"

static environment3_t environment3;
// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

sl_status_t mikroe_environment3_init(sl_i2cspm_t *i2cspm_instance)
{
  environment3_cfg_t environment3_cfg;

  if (i2cspm_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Call basic setup functions
  environment3_cfg_setup(&environment3_cfg);

  // Configure default i2csmp instance
  environment3.i2c.handle = i2cspm_instance;

  environment3_cfg.i2c_address = BME688_I2C_ADDR;

  if (ENVIRONMENT3_OK
      != environment3_init(&environment3, &environment3_cfg)) {
    return SL_STATUS_FAIL;
  } else {
    environment3_default_cfg(&environment3);
    return SL_STATUS_OK;
  }
}

sl_status_t mikroe_environment3_generic_write(uint8_t reg,
                                              uint8_t *data_in,
                                              uint8_t len)
{
  if ((data_in == NULL) || (len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (ENVIRONMENT3_OK
          == environment3_generic_write(&environment3, reg, data_in, len))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment3_generic_read(uint8_t reg,
                                             uint8_t *data_out,
                                             uint8_t len)
{
  if ((data_out == NULL) || (len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (ENVIRONMENT3_OK
          == environment3_generic_read(&environment3, reg, data_out, len))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment3_write_byte(uint8_t reg,
                                           uint8_t data_in)
{
  return (ENVIRONMENT3_OK == environment3_write_byte(&environment3, reg,
                                                     data_in))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

uint8_t mikroe_environment3_read_byte(uint8_t reg)
{
  return environment3_read_byte(&environment3, reg);
}

sl_status_t mikroe_environment3_check_id(void)
{
  return (ENVIRONMENT3_OK
          == environment3_check_id(&environment3)) ? SL_STATUS_OK
         :SL_STATUS_FAIL;
}

uint8_t mikroe_environment3_get_device_id(void)
{
  return environment3_get_device_id(&environment3);
}

sl_status_t mikroe_environment3_soft_reset(void)
{
  return (ENVIRONMENT3_OK
          == environment3_soft_reset(&environment3)) ? SL_STATUS_OK
         :SL_STATUS_FAIL;
}

int8_t mikroe_environment3_get_all_data(float *temp,
                                        float *hum,
                                        float *pres,
                                        uint32_t *gas)
{
  return environment3_get_all_data(&environment3, temp, hum, pres, gas);
}

float mikroe_environment3_get_temperature(void)
{
  return environment3_get_temperature(&environment3);
}

float mikroe_environment3_get_humidity(void)
{
  return environment3_get_humidity(&environment3);
}

float mikroe_environment3_get_pressure(void)
{
  return environment3_get_pressure(&environment3);
}

float mikroe_environment3_get_gas_resistance(void)
{
  return environment3_get_gas_resistance(&environment3);
}

int8_t mikroe_environment3_get_regs(uint8_t reg, uint8_t *data_out,
                                    uint16_t len)
{
  if ((data_out == NULL) || (len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return environment3_get_regs(&environment3, reg, data_out, len);
}

int8_t mikroe_environment3_set_regs(uint8_t *reg, uint8_t *data_in, uint8_t len)
{
  if ((data_in == NULL) || (len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return environment3_set_regs(&environment3, reg, data_in, len);
}

int8_t mikroe_environment3_enable_heater(uint8_t state)
{
  return environment3_enable_heater(&environment3, state);
}

int8_t mikroe_environment3_set_heater_conf(
  mikroe_environment3_gas_settings_t *conf)
{
  return environment3_set_heater_conf(&environment3,
                                      (environment3_gas_settings_t *)conf);
}

int8_t mikroe_environment3_set_sensor_conf(
  mikroe_environment3_tph_settings_t *conf)
{
  return environment3_set_sensor_conf(&environment3,
                                      (environment3_tph_settings_t *)conf);
}

int8_t mikroe_environment3_set_operating_mode(uint8_t op_mode)
{
  return environment3_set_operating_mode(&environment3, op_mode);
}

int8_t mikroe_environment3_get_operating_mode(void)
{
  return environment3_get_operating_mode(&environment3);
}
