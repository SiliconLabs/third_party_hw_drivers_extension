/***************************************************************************//**
 * @file mikroe_bma400_i2c.c
 * @brief I2C abstraction used by BMA400
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

#include "mikroe_bma400_i2c.h"

// Read write length varies based on user requirement
#define READ_WRITE_LENGTH  UINT8_C(46)

// Variable to store the device address
static uint8_t dev_addr;
// Variable to store the i2cspm instance
static sl_i2cspm_t *i2c_handle;

// Local prototypes
static int8_t bma400_i2c_read(uint8_t reg_addr,
                              uint8_t *reg_data,
                              uint32_t len,
                              void *intf_ptr);
static int8_t bma400_i2c_write(uint8_t reg_addr,
                               const uint8_t *reg_data,
                               uint32_t len,
                               void *intf_ptr);
static void bma400_delay_us(uint32_t period, void *intf_ptr);

/***************************************************************************//**
 * @brief
 *  Initialize I2C Interface for the BMA400.
 *
 * @param[in] i2cspm
 *  The I2CSPM instance to use.
 * @param[in] bma400_i2c_addr
 *  The I2C address of the BMA400.
 * @param[out] bma400
 *  The BMA400 device structure.
 *
 * @return
 *  @ref BMA400_OK on success.
 *  @ref On failure, BMA400_E_NULL_PTR is returned.
 ******************************************************************************/
int8_t bma400_i2c_init(sl_i2cspm_t *i2cspm,
                       uint8_t bma400_i2c_addr,
                       struct bma400_dev *bma400)
{
  if (bma400 == NULL) {
    return BMA400_E_NULL_PTR;
  }

  // The device needs startup time
  sl_udelay_wait(10000);

  // Update i2cspm instance
  i2c_handle = i2cspm;
  // Update device addr
  dev_addr = bma400_i2c_addr;

  bma400->read = bma400_i2c_read;
  bma400->write = bma400_i2c_write;
  bma400->intf = BMA400_I2C_INTF;

  bma400->intf_ptr = &dev_addr;
  bma400->delay_us = bma400_delay_us;
  bma400->read_write_len = READ_WRITE_LENGTH;

  return BMA400_OK;
}

/***************************************************************************//**
 * @brief I2C read function map to platform
 ******************************************************************************/
static int8_t bma400_i2c_read(uint8_t reg_addr,
                              uint8_t *reg_data,
                              uint32_t len,
                              void *intf_ptr)
{
  (void) intf_ptr;
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];

  seq.addr = dev_addr << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  i2c_write_data[0] = reg_addr;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len = 1;

  // Select length of data to be read
  seq.buf[1].data = reg_data;
  seq.buf[1].len = (uint16_t)len;
  ret = I2CSPM_Transfer(i2c_handle, &seq);
  if (ret != i2cTransferDone) {
    return BMA400_E_COM_FAIL;
  }

  return BMA400_OK;
}

/***************************************************************************//**
 * @brief I2C write function map to platform
 ******************************************************************************/
static int8_t bma400_i2c_write(uint8_t reg_addr,
                               const uint8_t *reg_data,
                               uint32_t len,
                               void *intf_ptr)
{
  (void) intf_ptr;
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[len + 1];
  uint8_t i2c_read_data[1];

  seq.addr = dev_addr << 1;
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
    return BMA400_E_COM_FAIL;
  }

  return BMA400_OK;
}

/***************************************************************************//**
 * @brief delay function map to platform
 ******************************************************************************/
static void bma400_delay_us(uint32_t period, void *intf_ptr)
{
  (void) intf_ptr;
  sl_udelay_wait(period);
}
