/***************************************************************************//**
 * @file mikroe_bme688_i2c.c
 * @brief I2C abstraction used by BME688
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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

#include "mikroe_bme688_i2c.h"

// Local prototypes
static int8_t mikroe_bme688_i2c_read(uint8_t reg_addr,
                                     uint8_t *reg_data,
                                     uint32_t len,
                                     void *intf_ptr);
static int8_t mikroe_bme688_i2c_write(uint8_t reg_addr,
                                      const uint8_t *reg_data,
                                      uint32_t len,
                                      void *intf_ptr);
static void mikroe_bme688_i2c_delay_us(uint32_t period,
                                       void *intf_ptr);

/***************************************************************************//**
 *  Initializes an I2C interface for BME688.
 *  As entry point, call this API before using other APIs.
 ******************************************************************************/
int8_t mikroe_bme688_i2c_init(bme68x_dev_t *bme688)
{
  if (bme688 == NULL) {
    return BME68X_E_NULL_PTR;
  }

  bme688->read = mikroe_bme688_i2c_read;
  bme688->write = mikroe_bme688_i2c_write;
  bme688->intf = BME68X_I2C_INTF;

  bme688->delay_us = mikroe_bme688_i2c_delay_us;

  /* The ambient temperature in deg C is used for
   * defining the heater temperature
   */
  bme688->amb_temp = 25;

  return BME68X_OK;
}

/***************************************************************************//**
 * @brief
 *   I2C read function map to platform.
 *
 * @param[in] reg_addr
 *  The start address of the register to send to device.
 * @param[out] reg_data
 *  The data read from the sensor.
 * @param[in] len
 *  Number of registers to read.
 * @param[in] intf_ptr
 *  Interface pointer
 *
 * @return
 *  @ref BME68x_OK on success.
 *  @ref On failure, BME68X_E_COM_FAIL is returned.
 ******************************************************************************/
static int8_t mikroe_bme688_i2c_read(uint8_t reg_addr,
                                     uint8_t *reg_data,
                                     uint32_t len,
                                     void *intf_ptr)
{
  sl_i2cspm_t *i2c_handle = ((bme68x_i2c_t *)intf_ptr)->handle;
  uint8_t device_addr = ((bme68x_i2c_t *)intf_ptr)->addr;

  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];

  seq.addr = device_addr << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  i2c_write_data[0] = reg_addr;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len = 1;

  // Select length of data to be read
  seq.buf[1].data = reg_data;
  seq.buf[1].len = (uint16_t)len;
  ret = I2CSPM_Transfer(i2c_handle, &seq);
  if (ret != i2cTransferDone) {
    return BME68X_E_COM_FAIL;
  }

  return BME68X_OK;
}

/***************************************************************************//**
 * @brief
 *   I2C write function map to platform.
 *
 * @param[in] reg_addr
 *  The start address of the register to send to device.
 * @param[out] reg_data
 *  The data write to the sensor.
 * @param[in] len
 *  Number of registers to write.
 * @param[in] intf_ptr
 *  Interface pointer
 *
 * @return
 *  @ref BME68x_OK on success.
 *  @ref On failure, BME68X_E_COM_FAIL is returned.
 ******************************************************************************/
static int8_t mikroe_bme688_i2c_write(uint8_t reg_addr,
                                      const uint8_t *reg_data,
                                      uint32_t len,
                                      void *intf_ptr)
{
  sl_i2cspm_t *i2c_handle = ((bme68x_i2c_t *)intf_ptr)->handle;
  uint8_t device_addr = ((bme68x_i2c_t *)intf_ptr)->addr;

  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[len + 1];
  uint8_t i2c_read_data[1];

  seq.addr = device_addr << 1;
  seq.flags = I2C_FLAG_WRITE;

  // Select register and data to write
  i2c_write_data[0] = reg_addr;
  for (uint16_t i = 0; i < (uint16_t)len; i++) {
    i2c_write_data[i + 1] = reg_data[i];
  }
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len = (uint16_t)len + 1;

  // Select length of data to be read
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len = 0;
  ret = I2CSPM_Transfer(i2c_handle, &seq);
  if (ret != i2cTransferDone) {
    return BME68X_E_COM_FAIL;
  }

  return BME68X_OK;
}

/***************************************************************************//**
 * @brief
 *  Delay function map to platform
 *
 * @param[in] period
 *  Delay in ms
 * @param[in] intf_ptr
 *  Interface pointer
 *
 ******************************************************************************/
static void mikroe_bme688_i2c_delay_us(uint32_t period, void *intf_ptr)
{
  (void) intf_ptr;
  sl_udelay_wait(period);
}
