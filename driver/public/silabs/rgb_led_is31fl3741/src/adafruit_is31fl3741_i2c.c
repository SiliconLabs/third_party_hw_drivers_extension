/***************************************************************************//**
 * @file adafruit_is31fl3741_i2c.c
 * @brief Adafruit RGB LED platform source file.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include "adafruit_is31fl3741_i2c.h"

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
static sl_i2cspm_t *is31fl3741_i2cspm_instance = NULL;
static uint8_t is31fl3741_i2cspm_addr = IS31FL3741_DEFAULT_I2C_ADDR;

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 *  Assign the I2C instance for adafruit RGB LED.
 *****************************************************************************/
void adafruit_is31fl3741_i2c_set_instance(sl_i2cspm_t *i2cspm)
{
  is31fl3741_i2cspm_instance = i2cspm;
}

/**************************************************************************//**
 *  Write data to register of RGB LED IS31FL3741.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_i2c_write_reg(uint8_t reg_addr,
                                              uint8_t *data,
                                              uint8_t len)

{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr = (uint16_t)(is31fl3741_i2cspm_addr << 1);
  seq.flags = I2C_FLAG_WRITE_WRITE;

  seq.buf[0].data = &reg_addr;
  seq.buf[0].len = 1;

  seq.buf[1].data = data;
  seq.buf[1].len = len;

  ret = I2CSPM_Transfer(is31fl3741_i2cspm_instance, &seq);
  return ret;
}

/**************************************************************************//**
 *  Read data from register of RGB LED IS31FL3741.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_i2c_read_reg(uint8_t reg_addr,
                                             uint8_t *data,
                                             uint8_t len)

{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr = (uint16_t)(is31fl3741_i2cspm_addr << 1);
  seq.flags = I2C_FLAG_WRITE_READ;

  seq.buf[0].data = &reg_addr;
  seq.buf[0].len = 1;

  seq.buf[1].data = data;
  seq.buf[1].len = len;

  ret = I2CSPM_Transfer(is31fl3741_i2cspm_instance, &seq);
  return ret;
}

/**************************************************************************//**
 *  Write data to RGB LED IS31FL3741.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_i2c_write(uint8_t *data, uint8_t len)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr = (uint16_t)(is31fl3741_i2cspm_addr << 1);
  seq.flags = I2C_FLAG_WRITE;

  seq.buf[0].data = data;
  seq.buf[0].len = len;

  ret = I2CSPM_Transfer(is31fl3741_i2cspm_instance, &seq);
  return ret;
}

/**************************************************************************//**
 *  Select device via I2C address.
 *****************************************************************************/
void adafruit_is31fl3741_i2c_select_device(uint8_t addr)
{
  is31fl3741_i2cspm_addr = addr;
}
