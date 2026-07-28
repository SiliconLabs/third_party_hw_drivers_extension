/***************************************************************************//**
 * @file lm75a_temp_sensor.c
 * @brief LM75A temperature sensor source file.
 *******************************************************************************
 * # License
 * <b>Copyright 2026 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "lm75a_temp_sensor.h"

/*******************************************************************************
 * Platform specific includes
 ******************************************************************************/
#ifdef SLI_SI917

#include "sl_si91x_i2c.h"

static sl_i2c_instance_t lm75_i2c_instance;
static sl_i2c_config_t lm75_i2c_config;

#else

#include "sl_i2cspm.h"

static sl_i2cspm_t *lm75_i2c_instance = NULL;

#endif

/*******************************************************************************
 * Initialize the LM75A temperature sensor.
 *
 * Configures the I2C peripheral and prepares the LM75A sensor
 * for temperature measurements.
 *
 * @param[in] i2c_instance Pointer to the platform-specific I2C instance.
 * @param[in] i2c_config Pointer to the platform-specific I2C configuration.
 *                            Required for SI91x, unused for Series 2.
 * 
 * @return SL_STATUS_OK on success.
 * @return SL_STATUS_NULL_POINTER if i2c_instance is NULL.
 * @return SL_STATUS_FAIL if I2C initialization fails.
 ******************************************************************************/
sl_status_t lm75_init(void *i2c_instance, void *i2c_config)
{
  if (i2c_instance == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

#ifdef SLI_SI917
  sl_i2c_status_t status;

  if (i2c_config == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  lm75_i2c_instance = *((sl_i2c_instance_t *)i2c_instance);
  lm75_i2c_config = *((sl_i2c_config_t *)i2c_config);

  status = sl_i2c_driver_init(lm75_i2c_instance, &lm75_i2c_config);
  if (status != SL_I2C_SUCCESS) {
    return SL_STATUS_FAIL;
  }

  sl_i2c_driver_configure_fifo_threshold(lm75_i2c_instance, 0, 0);

#else

  (void)i2c_config;
  lm75_i2c_instance = (sl_i2cspm_t *)i2c_instance;

#endif

  return SL_STATUS_OK;
}

/*******************************************************************************
 * Read temperature from the LM75A sensor.
 *
 * Performs an I2C write-read transaction to read the temperature register,
 * converts the raw sensor value into fixed-point Celsius format,
 * and returns the result as Celsius x 100.
 *
 * Example:
 *  2550 represents 25.50 °C
 *  -550 represents -5.50 °C
 *
 * @param[out] temp_c Pointer to store temperature in Celsius x 100 format.
 *
 * @return SL_STATUS_OK on success.
 * @return SL_STATUS_NULL_POINTER if temp_c is NULL.
 * @return SL_STATUS_TRANSMIT if I2C transmit fails.
 * @return SL_STATUS_RECEIVE if I2C receive fails.
 ******************************************************************************/
sl_status_t lm75_get_temperature(int16_t *temp_c)
{
  uint8_t reg;
  uint8_t data[2];
  int16_t raw;

#ifndef SLI_SI917
  I2C_TransferSeq_TypeDef seq;
#endif

  if (temp_c == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  reg     = LM75_TEMP_REG;
  data[0] = 0;
  data[1] = 0;

#ifdef SLI_SI917

  if (sl_i2c_driver_send_data_blocking(lm75_i2c_instance,
                                       LM75_I2C_ADDR,
                                       &reg,
                                       1) != SL_I2C_SUCCESS) {
    return SL_STATUS_TRANSMIT;
  }

  if (sl_i2c_driver_receive_data_blocking(lm75_i2c_instance,
                                          LM75_I2C_ADDR,
                                          data,
                                          2) != SL_I2C_SUCCESS) {
    return SL_STATUS_RECEIVE;
  }

#else

  seq.addr = LM75_I2C_ADDR << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  seq.buf[0].data = &reg;
  seq.buf[0].len  = 1;

  seq.buf[1].data = data;
  seq.buf[1].len  = 2;

  if (I2CSPM_Transfer(lm75_i2c_instance, &seq) != i2cTransferDone) {
    return SL_STATUS_TRANSMIT;
  }

#endif

  raw = (int16_t)((data[0] << 8) | data[1]);
  raw >>= 7;

  // LM75 resolution is 0.5 C.
  // Return temperature as Celsius x 100.
  *temp_c = raw * 50;

  return SL_STATUS_OK;
}