/***************************************************************************//**
 * @file ssd1306_i2c.c
 * @brief I2C abstraction used by SSD1306
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
#include <string.h>
#include "micro_oled_ssd1306_i2c.h"
#include "micro_oled_ssd1306_config.h"
#include "sl_i2cspm.h"

static sl_i2cspm_t *i2c_instance_handle = NULL;

static sl_status_t ssd1306_send(const void *data,
                                uint32_t len,
                                uint8_t command_type);

/***************************************************************************//**
 * @brief
 *   Initialize the i2c interface.
 *
 * @detail
 *  The driver instances will be initialized automatically,
 *  during the sl_system_init() call in main.c.
 *****************************************************************************/
void ssd1306_i2c_init(sl_i2cspm_t *i2c_handle)
{
  i2c_instance_handle = i2c_handle;
}

/***************************************************************************//**
 * @brief
 *    Start an blocking command transmit transfer.
 *
 * @note
 *    @n This function is blocking and returns when the transfer is complete.
 *
 * @param[in] command
 *    Transmit command buffer.
 *
 * @param[in] len
 *    Number of bytes in transfer.
 *
 * @return
 *    @ref SL_STATUS_OK on success or @ref SL_STATUS_FAIL on failure
 ******************************************************************************/
sl_status_t ssd1306_send_command(const void *cmd, uint32_t len)
{
  return ssd1306_send(cmd, len, 0x00);
}

/***************************************************************************//**
 * @brief
 *    Start an blocking data transmit transfer.
 *
 * @note
 *    @n This function is blocking and returns when the transfer is complete.
 *
 * @param[in] data
 *    Transmit data buffer.
 *
 * @param[in] count
 *    Number of bytes in transfer.
 *
 * @return
 *    @ref SL_STATUS_OK on success or @ref SL_STATUS_FAIL on failure
 ******************************************************************************/
sl_status_t ssd1306_send_data(const void *data, uint32_t len)
{
  return ssd1306_send(data, len, 0x40);
}

static sl_status_t ssd1306_send(const void *data,
                                uint32_t len,
                                uint8_t command_type)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[len];
  uint8_t i2c_read_data[1];
  const uint8_t *ptr = data;

  if (NULL == i2c_instance_handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  seq.addr = CONFIG_OLED_DISPLAY_ADDRESS << 1;
  seq.flags = I2C_FLAG_WRITE;

  i2c_write_data[0] = command_type;
  for (uint32_t i = 0; i < len; i++) {
    i2c_write_data[i + 1] = ptr[i];
  }
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len = len + 1;

  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len = 0;
  ret = I2CSPM_Transfer(i2c_instance_handle, &seq);
  if (ret != i2cTransferDone) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}
