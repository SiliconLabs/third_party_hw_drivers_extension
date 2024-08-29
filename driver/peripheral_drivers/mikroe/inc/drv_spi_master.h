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

#ifndef _DRV_SPI_MASTER_H_
#define _DRV_SPI_MASTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "drv_name.h"

typedef const void *mikroe_spi_handle_t; ///< Created SPI handle type

typedef enum {
  SPI_MASTER_SUCCESS = 0, SPI_MASTER_ERROR = (-1)
} spi_master_err_t;

typedef enum {
  SPI_MASTER_MODE_0 = 0,
  SPI_MASTER_MODE_1,
  SPI_MASTER_MODE_2,
  SPI_MASTER_MODE_3,

  SPI_MASTER_MODE_DEFAULT = SPI_MASTER_MODE_0
} spi_master_mode_t;

typedef enum {
  SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW = 0,
  SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH,

  SPI_MASTER_CHIP_SELECT_DEFAULT_POLARITY =
    SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW
} spi_master_chip_select_polarity_t;

typedef struct {
  uint8_t default_write_data;
  pin_name_t sck;
  pin_name_t miso;
  pin_name_t mosi;
  uint32_t speed;
  spi_master_mode_t mode;
} spi_master_config_t;

typedef struct {
  mikroe_spi_handle_t handle;
  spi_master_config_t config;
} spi_master_t;

void spi_master_configure_default(spi_master_config_t *config);
err_t spi_master_open(spi_master_t *obj, spi_master_config_t *config);
void spi_master_select_device(pin_name_t chip_select);
void spi_master_deselect_device(pin_name_t chip_select);
void spi_master_set_chip_select_polarity(
  spi_master_chip_select_polarity_t polarity);
err_t spi_master_set_default_write_data(spi_master_t *obj,
                                        uint8_t default_write_data);
err_t spi_master_set_speed(spi_master_t *obj, uint32_t speed);
err_t spi_master_set_mode(spi_master_t *obj, spi_master_mode_t mode);
err_t spi_master_write(spi_master_t *obj,
                       uint8_t *write_data_buffer,
                       size_t write_data_length);
err_t spi_master_read(spi_master_t *obj,
                      uint8_t *read_data_buffer,
                      size_t read_data_length);
err_t spi_master_exchange(spi_master_t *obj,
                          uint8_t *write_data_buffer,
                          uint8_t *read_data_buffer,
                          size_t exchange_data_length);
err_t spi_master_write_then_read(spi_master_t *obj,
                                 uint8_t *write_data_buffer,
                                 size_t length_write_data,
                                 uint8_t *read_data_buffer,
                                 size_t length_read_data);
void spi_master_close(spi_master_t *obj);

#ifdef __cplusplus
}
#endif

#endif // _DRV_SPI_MASTER_H_
// ------------------------------------------------------------------------- END
