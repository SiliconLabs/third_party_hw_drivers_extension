/***************************************************************************//**
 * @file sparkfun_mlx90640_i2c.h
 * @brief SPARKFUN MLX90640 IR Array sensor I2C header file.
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

#ifndef SPARKFUN_MLX90640_I2C_H_
#define SPARKFUN_MLX90640_I2C_H_

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------

#include <stdint.h>
#include "sl_status.h"
#include "sl_i2cspm.h"

// -----------------------------------------------------------------------------
//                          Public Function Declarations
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  Assigns an I2CSPM instance and the default slave address for the driver to
 *  use
 *
 * @param[in] i2cspm_instace - Pointer to the I2CSPM instance
 ******************************************************************************/
sl_status_t sparkfun_mlx90640_i2c_init(sl_i2cspm_t *i2cspm_instance,
                                       uint8_t i2c_addr);

/***************************************************************************//**
 * Assigns a new I2CSPM instance and a slave address for the driver to use
 ******************************************************************************/
sl_status_t sparkfun_mlx90640_i2c_change_bus_and_address(
  sl_i2cspm_t *i2cspm_instance,
  uint8_t i2c_addr);

/***************************************************************************//**
 * @brief
 *  Updates the I2C slave address of the device.
 *  In case the address is already changed then it only stores the address to
 *  use
 *
 * @param[in] slave_addr - new I2C slave address to use
 ******************************************************************************/
sl_status_t sparkfun_mlx90640_i2c_set_own_address(uint8_t slave_addr);

/***************************************************************************//**
 * @brief
 *  Retrieves the current I2C slave address used by the driver
 *
 * @param[out] current_address - Current I2C slave address used
 ******************************************************************************/
sl_status_t sparkfun_mlx90640_i2c_get_current_own_addr(
  uint8_t *current_address);

/***************************************************************************//**
 * @brief
 *  Issues an I2C general reset
 ******************************************************************************/
sl_status_t sparkfun_mlx90640_i2c_general_reset(void);

/***************************************************************************//**
 * @brief
 *  Initiates an I2C read of the device
 *
 * @param[in] startAddress - EEPROM memory address of the device to read out
 *  from
 * @param[in] nMemAddressRead - Length of the requested data
 * @param[out] data - pointer to an array where the received data will be stored
 ******************************************************************************/
sl_status_t sparkfun_mlx90640_i2c_read(uint16_t startAddress,
                                       uint16_t nMemAddressRead,
                                       uint16_t *data);

/***************************************************************************//**
 * @brief
 *  Initiates an I2C write to the device
 *
 * @param[in] writeAddress - EEPROM memory address of the device to write to
 * @param[in] data - 16bit data to send to the device
 ******************************************************************************/
sl_status_t sparkfun_mlx90640_i2c_write(uint16_t writeAddress, uint16_t data);

/***************************************************************************//**
 * @brief
 *  Sets I2C base frequency to a given setting
 *
 * @param[in] freq - new frequency in Hz to set for the I2C base frequency
 ******************************************************************************/
void sparkfun_mlx90640_i2c_freq_set(int freq);

#endif /* SPARKFUN_MLX90640_I2C_H_ */
