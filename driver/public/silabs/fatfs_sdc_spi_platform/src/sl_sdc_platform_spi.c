/***************************************************************************//**
 * @file sl_sdc_platform_spi.c
 * @brief Storage Device Controls SD Card platform
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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
#include "sl_sdc_platform_spi.h"

/***************************************************************************//**
 * Exchange a byte.
 ******************************************************************************/
sl_status_t sdc_xchg_spi(SPIDRV_Handle_t spi_handle, uint8_t tx, uint8_t *rx)
{
  sl_status_t retval;

  if (!spi_handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  retval = SPIDRV_MTransferB(spi_handle, &tx, rx, 1);
  if (retval != ECODE_EMDRV_SPIDRV_OK) {
    *rx = 0;
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Multi-byte SPI transaction (transmit).
 ******************************************************************************/
sl_status_t sdc_xmit_spi_multi(SPIDRV_Handle_t spi_handle,
                               const uint8_t *buff,
                               uint16_t cnt)
{
  sl_status_t retval;

  if (!spi_handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  retval = SPIDRV_MTransmitB(spi_handle, buff, cnt);
  if (retval != ECODE_EMDRV_SPIDRV_OK) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Multi-byte SPI transaction (receive).
 ******************************************************************************/
sl_status_t sdc_rcvr_spi_multi(SPIDRV_Handle_t spi_handle,
                               uint8_t *buff,
                               uint16_t cnt)
{
  sl_status_t retval;
  uint8_t dummy = 0xff;

  if (!spi_handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  retval = SPIDRV_MTransferB(spi_handle, &dummy, buff, cnt);
  if (retval != ECODE_EMDRV_SPIDRV_OK) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

sl_status_t sdc_platform_set_bit_rate(SPIDRV_Handle_t spi_handle,
                                      uint32_t bit_rate)
{
  sl_status_t retval;

  if (!spi_handle) {
    return SL_STATUS_NULL_POINTER;
  }

  retval = SPIDRV_SetBitrate(spi_handle, bit_rate);

  if (retval != ECODE_EMDRV_SPIDRV_OK) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}
