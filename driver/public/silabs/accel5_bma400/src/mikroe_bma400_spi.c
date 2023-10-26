/***************************************************************************//**
 * @file mikroe_bma400_spi.c
 * @brief SPI abstraction used by BMA400
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

#include "mikroe_bma400_spi.h"

// Read write length varies based on user requirement
#define READ_WRITE_LENGTH  UINT8_C(46)

// Variable to store the device address
static uint8_t dev_addr;
// Variable to store the spidrv instance
static SPIDRV_Handle_t spi_handle;

// Local prototypes
static int8_t bma400_spi_read(uint8_t reg_addr,
                              uint8_t *reg_data,
                              uint32_t len,
                              void *intf_ptr);
static int8_t bma400_spi_write(uint8_t reg_addr,
                               const uint8_t *reg_data,
                               uint32_t len,
                               void *intf_ptr);
static void bma400_delay_us(uint32_t period, void *intf_ptr);

/***************************************************************************//**
 * @brief
 *  Initialize an SPI interface for the BMA400.
 *
 * @param[in] spidrv
 *  The SPIDRV instance to use.
 * @param[out] bma400
 *  The BMA400 device structure.
 *
 * @return
 *  @ref BMA400_OK on success.
 *  @ref On failure, BMA400_E_NULL_PTR is returned.
 ******************************************************************************/
int8_t bma400_spi_init(SPIDRV_Handle_t spidrv, struct bma400_dev *bma400)
{
  if (bma400 == NULL) {
    return BMA400_E_NULL_PTR;
  }

  // Update the spidrv instance
  spi_handle = spidrv;

  /* The device needs startup time */
  sl_udelay_wait(10000);

  bma400->read = bma400_spi_read;
  bma400->write = bma400_spi_write;
  bma400->intf = BMA400_SPI_INTF;

  dev_addr = 0xff;
  bma400->intf_ptr = &dev_addr;
  bma400->delay_us = bma400_delay_us;
  bma400->read_write_len = READ_WRITE_LENGTH;

  return BMA400_OK;
}

/***************************************************************************//**
 * @brief SPI read function map to platform
 ******************************************************************************/
static int8_t bma400_spi_read(uint8_t reg_addr,
                              uint8_t *reg_data,
                              uint32_t len,
                              void *intf_ptr)
{
  (void) intf_ptr;
  Ecode_t ret_code;
  uint8_t txBuffer[len + 1];
  uint8_t rxBuffer[len + 1];

  txBuffer[0] = reg_addr;

  // Fullfill the remaining elements of the txBuffer with dummy
  for (uint16_t i = 0; i < (uint16_t)len; i++) {
    txBuffer[i + 1] = 0xff;
  }

  ret_code = SPIDRV_MTransferB(spi_handle, txBuffer, rxBuffer, len + 1);
  if (ret_code != ECODE_EMDRV_SPIDRV_OK) {
    return BMA400_E_COM_FAIL;
  }
  // Copy the receive payload (without the dummy byte) to the output buffer data
  for (uint16_t i = 0; i < len; i++) {
    reg_data[i] = rxBuffer[i + 1];
  }

  return BMA400_OK;
}

/***************************************************************************//**
 * @brief SPI write function map to platform
 ******************************************************************************/
static int8_t bma400_spi_write(uint8_t reg_addr,
                               const uint8_t *reg_data,
                               uint32_t len,
                               void *intf_ptr)
{
  (void) intf_ptr;
  Ecode_t ret_code;
  uint8_t txBuffer[len + 1];

  txBuffer[0] = reg_addr;

  // Fullfill the remaining elements of the txBuffer with dummy
  for (uint16_t i = 0; i < (uint16_t)len; i++) {
    txBuffer[i + 1] = reg_data[i];
  }

  ret_code = SPIDRV_MTransmitB(spi_handle, txBuffer, len + 1);
  if (ret_code != ECODE_EMDRV_SPIDRV_OK) {
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
