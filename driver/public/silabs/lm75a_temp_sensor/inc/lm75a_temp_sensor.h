/***************************************************************************//**
 * @file lm75a_temp_sensor.h
 * @brief LM75A temperature sensor header file.
 *******************************************************************************
 * # License
 * <b>Copyright 2026 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef LM75_TEMP_SENSOR_H_
#define LM75_TEMP_SENSOR_H_


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "sl_status.h"

/*******************************************************************************
 * Common LM75 defines
 ******************************************************************************/

#define LM75_I2C_ADDR   0x48
#define LM75_TEMP_REG   0x00

/*****************************************************************************
 * Initialize LM75 driver
 ******************************************************************************/
sl_status_t lm75_init(void *i2c_instance, void *i2c_config);

/*****************************************************************************
 * Read temperature in Celsius
 ******************************************************************************/
sl_status_t lm75_get_temperature(int16_t *temp_c);

#endif // LM75_TEMP_SENSOR_H_