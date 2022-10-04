/***************************************************************************//**
 * @file drv_spi_master.h
 * @brief mikroSDK 2.0 Click Peripheral Drivers
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

#include "drv_spi_master.h"
#include "drv_digital_out.h"

static spi_master_t *_owner = NULL;

static spi_master_chip_select_polarity_t spi_master_chip_select_polarity =
  SPI_MASTER_CHIP_SELECT_DEFAULT_POLARITY;

static err_t _acquire(spi_master_t *obj, bool obj_open_state)
{
  if ((obj_open_state == true) && (_owner == obj)) {
    return ACQUIRE_FAIL;
  }

  if (_owner != obj) {
    _owner = obj;
  }

  return ACQUIRE_SUCCESS;
}

static void spi_master_get_init_data(spi_master_t *obj, SPIDRV_Init_t *initData)
{
  SPIDRV_Init_t *p_initData = &obj->handle->initData;
  // Get SPI driver instance initialization structure.
  memcpy(initData, p_initData, sizeof(SPIDRV_Init_t));
}

static void spi_master_configure_gpio_pin(digital_out_t *out, pin_name_t name)
{
  out->pin.base = (GPIO_Port_TypeDef) hal_gpio_port_index(name);
  out->pin.mask = 1 << hal_gpio_pin_index(name);
}

void spi_master_configure_default(spi_master_config_t *config)
{
  config->default_write_data = 0;
  config->sck = 0xFFFFFFFF;
  config->miso = 0xFFFFFFFF;
  config->mosi = 0xFFFFFFFF;
  config->speed = 100000;
  config->mode = SPI_MASTER_MODE_DEFAULT;
}

err_t spi_master_open(spi_master_t *obj, spi_master_config_t *config)
{
  spi_master_config_t *p_config = &obj->config;
  memcpy(p_config, config, sizeof(spi_master_config_t));
  return _acquire(obj, true);
}

void spi_master_select_device(pin_name_t chip_select)
{
  digital_out_t struct_cs;

  if (chip_select == 0xFFFFFFFF) {
    return;
  }

  spi_master_configure_gpio_pin(&struct_cs, chip_select);

  if (spi_master_chip_select_polarity
      == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH) {
    digital_out_high(&struct_cs);
  } else {
    digital_out_low(&struct_cs);
  }
}

void spi_master_deselect_device(pin_name_t chip_select)
{
  digital_out_t struct_cs;

  if (chip_select == 0xFFFFFFFF) {
    return;
  }

  spi_master_configure_gpio_pin(&struct_cs, chip_select);

  if (spi_master_chip_select_polarity
      == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH) {
    digital_out_low(&struct_cs);
  } else {
    digital_out_high(&struct_cs);
  }
}

void spi_master_set_chip_select_polarity(
  spi_master_chip_select_polarity_t polarity)
{
  if (polarity == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH) {
    spi_master_chip_select_polarity =
      SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH;
  } else {
    spi_master_chip_select_polarity =
      SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW;
  }
}

/***************************************************************************//**
 * Set SPI Master driver communication speed.
 ******************************************************************************/
err_t spi_master_set_speed(spi_master_t *obj, uint32_t speed)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  obj->config.speed = speed;
  // Set SPI bus bitrate.
  if (SPIDRV_SetBitrate(obj->handle, speed) != ECODE_EMDRV_SPIDRV_OK) {
    return SPI_MASTER_ERROR;
  }

  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Set SPI Master driver communication mode.
 ******************************************************************************/
err_t spi_master_set_mode(spi_master_t *obj, spi_master_mode_t mode)
{
  SPIDRV_Init_t initData;

  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  obj->config.mode = mode;

  spi_master_get_init_data(obj, &initData);
  initData.clockMode = (SPIDRV_ClockMode_t) mode;
  // DeInitialize an SPI driver instance.
  SPIDRV_DeInit(obj->handle);
  // Initialize an SPI driver instance with new mode.
  SPIDRV_Init(obj->handle, &initData);

  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Set SPI Master driver default ( dummy ) write data.
 ******************************************************************************/
err_t spi_master_set_default_write_data(spi_master_t *obj,
                                        uint8_t default_write_data)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  obj->config.default_write_data = default_write_data;
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Write byte to SPI bus.
 ******************************************************************************/
err_t spi_master_write(spi_master_t *obj,
                       uint8_t *write_data_buffer,
                       size_t write_data_length)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if (SPIDRV_MTransmitB(obj->handle, write_data_buffer,
                        write_data_length) != ECODE_EMDRV_SPIDRV_OK) {
    return SPI_MASTER_ERROR;
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Read byte from SPI bus.
 ******************************************************************************/
err_t spi_master_read(spi_master_t *obj,
                      uint8_t *read_data_buffer,
                      size_t read_data_length)
{
  uint8_t write_data_buffer[read_data_length];

  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  // The data transmit buffer will be populated with user-defined data
  for (size_t i = 0; i < read_data_length; i++) {
    write_data_buffer[i] = _owner->config.default_write_data;
  }

  if (SPIDRV_MTransferB(obj->handle, write_data_buffer, read_data_buffer,
                        read_data_length) != ECODE_EMDRV_SPIDRV_OK) {
    return SPI_MASTER_ERROR;
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Perform a sequence of SPI Master writes
 * immediately followed by a SPI Master read.
 ******************************************************************************/
err_t spi_master_write_then_read(spi_master_t *obj,
                                 uint8_t *write_data_buffer,
                                 size_t length_write_data,
                                 uint8_t *read_data_buffer,
                                 size_t length_read_data)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  spi_master_write(obj, write_data_buffer, length_write_data);
  spi_master_read(obj, read_data_buffer, length_read_data);

  return SPI_MASTER_SUCCESS;
}

void spi_master_close(spi_master_t *obj)
{
  obj->handle = NULL;
  _owner = NULL;
}

// ------------------------------------------------------------------------- END
