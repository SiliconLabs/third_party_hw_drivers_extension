/***************************************************************************//**
 * @file
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
#include "spidrv.h"
#include "sl_udelay.h"
#include "mikroe_bma400_spi.h"
#include "bma400.h"

// Read write length varies based on user requirement
#define READ_WRITE_LENGTH  UINT8_C(46)

// Variable to store the device address
static uint8_t dev_addr;
// Variable to store the i2cspm instance
static SPIDRV_Handle_t spi_handle;

// Local prototypes
static sl_status_t sl_bma400_spi_write_byte(SPIDRV_Handle_t spidrv,
                                            uint8_t reg_addr,
                                            uint8_t reg_data);
static sl_status_t sl_bma400_spi_block_read(SPIDRV_Handle_t spidrv,
                                            uint8_t reg_addr,
                                            uint8_t *reg_data,
                                            uint16_t len);
static sl_status_t sl_bma400_spi_block_write(SPIDRV_Handle_t spidrv,
                                             uint8_t reg_addr,
                                             uint8_t const *reg_data,
                                             uint16_t len);
static int8_t bma400_spi_read(uint8_t reg_addr,
                              uint8_t *reg_data,
                              uint32_t len,
                              void *intf_ptr);
static int8_t bma400_spi_write(uint8_t reg_addr,
                               const uint8_t *reg_data,
                               uint32_t len,
                               void *intf_ptr);
static void sl_bma400_delay_us(uint32_t period, void *intf_ptr);

/***************************************************************************//**
 * @brief
 *  Write a byte to BMA400 over SPI.
 *
 * @param[in] spidrv
 *  The SPIDRV instance to use.
 * @param[in] address
 *  Address of Register to write.
 * @param[in] data
 *  The data write to the sensor.
 *
 * @return
 *  @ref SL_STATUS_OK on success.
 *  @ref On failure, SL_STATUS_TRANSMIT is returned.
 ******************************************************************************/
static sl_status_t sl_bma400_spi_write_byte(SPIDRV_Handle_t spidrv,
                                            uint8_t reg_addr,
                                            uint8_t reg_data)
{
  uint8_t txBuffer[2];
  Ecode_t ret_code;

  txBuffer[0] = reg_addr;
  txBuffer[1] = reg_data;

  ret_code = SPIDRV_MTransmitB(spidrv, txBuffer, 2);
  if (ret_code != ECODE_EMDRV_SPIDRV_OK) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Read block of data from the BMA400.
 *
 * @param[in] spidrv
 *  The SPIDRV instance to use.
 * @param[in] reg_addr
 *  The start address of the register to send to device.
 *  See the bma400_defs.h for details.
 * @param[out] reg_data
 *  The data read from the sensor.
 * @param[in] len
 *  Number of registers to read.
 *
 * @return
 *  @ref SL_STATUS_OK on success.
 *  @ref On failure, SL_STATUS_TRANSMIT is returned.
 ******************************************************************************/
static sl_status_t sl_bma400_spi_block_read(SPIDRV_Handle_t spidrv,
                                            uint8_t reg_addr,
                                            uint8_t *reg_data,
                                            uint16_t len)
{
  const uint16_t comm_length = len + 1;
  uint8_t txBuffer[comm_length];
  uint8_t rxBuffer[comm_length];
  Ecode_t ret_code;

  txBuffer[0] = reg_addr;

  for (uint16_t i = 1; i < comm_length; i++) {
    txBuffer[i] = 0xff;
  }

  ret_code = SPIDRV_MTransferB(spidrv, txBuffer, rxBuffer, comm_length);
  if (ret_code != ECODE_EMDRV_SPIDRV_OK) {
    return SL_STATUS_TRANSMIT;
  }
  // Copy the receive payload (without the dummy byte) to the output buffer data
  for (uint16_t i = 0; i < len; i++) {
    reg_data[i] = rxBuffer[i + 1];
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Write block of data to the BMA400.
 *
 * @param[in] spidrv
 *  The SPIDRV instance to use.
 * @param[in] reg_addr
 *  The start address of the register to send to device.
 *  See the bma400_defs.h for details.
 * @param[out] reg_data
 *  The data read from the sensor.
 * @param[in] len
 *  Number of registers to write.
 *
 * @return
 *    @ref SL_STATUS_OK on success.
 *    @ref On failure, SL_STATUS_TRANSMIT is returned.
 ******************************************************************************/
static sl_status_t sl_bma400_spi_block_write(SPIDRV_Handle_t spidrv,
                                             uint8_t reg_addr,
                                             uint8_t const *reg_data,
                                             uint16_t len)
{
  sl_status_t ret = SL_STATUS_OK;

  for (uint16_t count = 0; ((count < len) && (ret == SL_STATUS_OK)); count++) {
    ret = sl_bma400_spi_write_byte(spidrv, reg_addr, reg_data[count]);
    reg_addr++;
  }

  return ret;
}

/***************************************************************************//**
 * @brief SPI read function map to platform
 ******************************************************************************/
static int8_t bma400_spi_read(uint8_t reg_addr,
                              uint8_t *reg_data,
                              uint32_t len,
                              void *intf_ptr)
{
  sl_status_t ret;

  (void)intf_ptr;
  ret = sl_bma400_spi_block_read(spi_handle,
                                 reg_addr,
                                 reg_data,
                                 (uint16_t)len);
  if (ret != SL_STATUS_OK) {
    return BMA400_E_COM_FAIL;
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
  sl_status_t ret;

  (void)intf_ptr;
  ret = sl_bma400_spi_block_write(spi_handle,
                                  reg_addr,
                                  reg_data,
                                  (uint16_t)len);
  if (ret != SL_STATUS_OK) {
    return BMA400_E_COM_FAIL;
  }

  return BMA400_OK;
}

/***************************************************************************//**
 * @brief delay function map to platform
 ******************************************************************************/
static void sl_bma400_delay_us(uint32_t period, void *intf_ptr)
{
  (void) intf_ptr;
  sl_udelay_wait(period);
}

/***************************************************************************//**
 * @brief
 *  Initialize SPI Interface for the BMA400.
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

  /* The device needs startup time */
  sl_udelay_wait(10000);

  /* Update spidrv instance */
  spi_handle = spidrv;

  dev_addr = 0xff;
  bma400->read = bma400_spi_read;
  bma400->write = bma400_spi_write;
  bma400->intf = BMA400_SPI_INTF;

  bma400->intf_ptr = &dev_addr;
  bma400->delay_us = sl_bma400_delay_us;
  bma400->read_write_len = READ_WRITE_LENGTH;

  return BMA400_OK;
}
