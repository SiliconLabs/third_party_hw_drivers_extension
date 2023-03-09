/***************************************************************************//**
 * @file sparkfun_sgp40_i2c.c
 * @brief sparkfun_sgp40_i2c source file.
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
#include "sparkfun_sgp40_i2c.h"
#include "em_i2c.h"

/***************************************************************************//**
 * Function to perform I2C write to the air quality sensor
 ******************************************************************************/
sl_status_t i2c_write_blocking(sl_i2cspm_t *i2c_instance,
                               uint8_t *src,
                               uint32_t len)
{
  I2C_TransferSeq_TypeDef seq;

  seq.addr = SGP40_ADDRESS << 1;
  seq.flags = I2C_FLAG_WRITE;

  /*Write buffer*/
  seq.buf[0].data = src;
  seq.buf[0].len = len;

  if (I2CSPM_Transfer(i2c_instance, &seq) != i2cTransferDone) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Function to perform I2C read from the air quality sensor
 ******************************************************************************/
sl_status_t i2c_read_blocking(sl_i2cspm_t *i2c_instance,
                              uint8_t *dest,
                              uint32_t len)
{
  I2C_TransferSeq_TypeDef seq;

  seq.addr = SGP40_ADDRESS << 1;
  seq.flags = I2C_FLAG_READ;

  /*Read buffer*/
  seq.buf[0].data = dest;
  seq.buf[0].len = len;

  if (I2CSPM_Transfer(i2c_instance, &seq) != i2cTransferDone) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}
