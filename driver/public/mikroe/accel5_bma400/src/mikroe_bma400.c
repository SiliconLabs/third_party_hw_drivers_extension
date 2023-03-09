/***************************************************************************//**
 * @file mikroe_bma400.c
 * @brief SCL BMA400 Source File
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

#include "accel5.h"
#include "mikroe_bma400.h"
#include "mikroe_bma400_config.h"
#include "third_party_hw_drivers_helpers.h"

static accel5_t accel5;
static accel5_cfg_t accel5_cfg;

sl_status_t mikroe_bma400_init(sl_i2cspm_t *i2cspm_instance)
{
  if (i2cspm_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  THIRD_PARTY_HW_DRV_RETCODE_INIT();

  // Configure default i2csmp instance
  accel5.i2c.handle = i2cspm_instance;

  // Call basic setup functions
  accel5_cfg_setup(&accel5_cfg);
#if defined(BMA400_IT1_PORT) && defined(BMA400_IT1_PIN)
  accel5_cfg.it1 = hal_gpio_pin_name(BMA400_IT1_PORT, BMA400_IT1_PIN);
#endif

#if defined(BMA400_IT2_PORT) && defined(BMA400_IT2_PIN)
  accel5_cfg.it2 = hal_gpio_pin_name(BMA400_IT2_PORT, BMA400_IT2_PIN);
#endif

  THIRD_PARTY_HW_DRV_RETCODE_TEST(accel5_init(&accel5, &accel5_cfg));

  if (SL_STATUS_OK != THIRD_PARTY_HW_DRV_RETCODE_VALUE) {
    return THIRD_PARTY_HW_DRV_RETCODE_VALUE;
  }

  THIRD_PARTY_HW_DRV_RETCODE_TEST(mikroe_bma400_present());

  return THIRD_PARTY_HW_DRV_RETCODE_VALUE;
}

sl_status_t mikroe_bma400_set_i2csmp_instance(sl_i2cspm_t *i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  accel5.i2c.handle = i2cspm_instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_default_cfg(uint8_t mode, uint8_t range)
{
  accel5_default_cfg(&accel5, mode, range);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Check whether a special BMA400 is present on the bus or not.
 ******************************************************************************/
sl_status_t mikroe_bma400_present(void)
{
  uint8_t chip_id;

  mikroe_bma400_read_byte(MIKROE_BMA400_REG_CHIP_ID, &chip_id);
  if (chip_id != MIKROE_BMA400_CHIP_ID) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_generic_write(uint8_t reg,
                                        uint8_t *data_buf,
                                        uint8_t len)
{
  if (NULL == data_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  accel5_generic_write(&accel5, reg, data_buf, len);

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_generic_read(uint8_t reg,
                                       uint8_t *data_buf,
                                       uint8_t len)
{
  if (NULL == data_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  accel5_generic_read(&accel5, reg, data_buf, len);

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_write_byte(uint8_t reg, uint8_t reg_data)
{
  accel5_write_byte(&accel5, reg, reg_data);

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_read_byte(uint8_t reg, uint8_t *data)
{
  if (NULL == data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *data = accel5_read_byte(&accel5, reg);

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_get_axis(uint8_t axis, int16_t *data)
{
  if (NULL == data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *data = accel5_get_axis(&accel5, axis);

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_sensor_time(uint32_t *sensor_time)
{
  if (NULL == sensor_time) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *sensor_time = accel5_sensor_time(&accel5);

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_get_temperature(float *temperature)
{
  if (NULL == temperature) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *temperature = accel5_get_temperature(&accel5);

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_read_data(uint8_t reg, uint16_t *data)
{
  *data = accel5_read_data(&accel5, reg);

  return SL_STATUS_OK;
}

sl_status_t mikroe_bma400_soft_reset(void)
{
  accel5_soft_reset(&accel5);

  return SL_STATUS_OK;
}
