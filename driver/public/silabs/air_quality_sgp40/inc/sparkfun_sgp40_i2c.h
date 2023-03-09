/***************************************************************************//**
 * @file sparkfun_sgp40_i2c.h
 * @brief sparkfun_sgp40_i2c header file.
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
#ifndef SPARKFUN_SGP40_I2C_H_
#define SPARKFUN_SGP40_I2C_H_

#include "sl_i2cspm.h"
#include "sl_status.h"

/***************************************************************************//**
 * @addtogroup SGP40 - Air Quality Sensor
 * @brief This file contains I2C driver for Air Quality Sensor.
 ******************************************************************************/
/// @brief  I2C Address
#define SGP40_ADDRESS                                     0x59

/***************************************************************************//**
 * @brief
 *    Function to perform I2C write to the air quality sensor
 *
 * @param[in] i2c_instance
 *    The I2C peripheral to use.
 * @param[in] src
 *    Pointer to data send buffer.
 * @param[in] len
 *    Length of data send.
 *
 * @return
 *   @li @ref SL_STATUS_OK Success.
 *
 *   @li @ref SL_STATUS_FAIL Failure.
 ******************************************************************************/
sl_status_t i2c_write_blocking(sl_i2cspm_t *i2c_instance,
                               uint8_t *src,
                               uint32_t len);

/***************************************************************************//**
 * @brief
 *    Function to perform I2C read from the air quality sensor
 *
 * @param[in] i2c_instance
 *    The I2C peripheral to use.
 * @param[in] dest
 *    Pointer to store data.
 * @param[in] len
 *    Length of data receive buffer.
 *
 * @return
 *   @li @ref SL_STATUS_OK Success.
 *
 *   @li @ref SL_STATUS_FAIL Failure.
 ******************************************************************************/
sl_status_t i2c_read_blocking(sl_i2cspm_t *i2c_instance,
                              uint8_t *dest,
                              uint32_t len);

#endif /* SPARKFUN_SGP40_I2C_H_ */
