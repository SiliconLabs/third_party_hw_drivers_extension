/***************************************************************************//**
 * @file drv_i2c_master.h
 * @brief mikroSDK 2.0 Click Peripheral Drivers - I2C Master
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

#ifndef _DRV_I2C_MASTER_H_
#define _DRV_I2C_MASTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "drv_name.h"
#include "sl_i2cspm.h"

typedef enum {
  I2C_MASTER_SUCCESS = 0, I2C_MASTER_ERROR = (-1)
} i2c_master_err_t;

typedef enum {
  I2C_MASTER_SPEED_STANDARD = 0, I2C_MASTER_SPEED_FULL, I2C_MASTER_SPEED_FAST
} i2c_master_speed_t;

typedef struct {
  uint8_t addr;
  pin_name_t sda;
  pin_name_t scl;
  uint32_t speed;
  uint16_t timeout_pass_count;
} i2c_master_config_t;

typedef struct {
  sl_i2cspm_t *handle;
  i2c_master_config_t config;
} i2c_master_t;

void i2c_master_configure_default(i2c_master_config_t *config);
err_t i2c_master_open(i2c_master_t *obj, i2c_master_config_t *config);
err_t i2c_master_set_speed(i2c_master_t *obj, uint32_t speed);
err_t i2c_master_set_timeout(i2c_master_t *obj, uint16_t timeout_pass_count);
err_t i2c_master_set_slave_address(i2c_master_t *obj, uint8_t address);
err_t i2c_master_write(i2c_master_t *obj,
                       uint8_t *write_data_buf,
                       size_t len_write_data);
err_t i2c_master_read(i2c_master_t *obj,
                      uint8_t *read_data_buf,
                      size_t len_read_data);
err_t i2c_master_write_then_read(i2c_master_t *obj,
                                 uint8_t *write_data_buf,
                                 size_t len_write_data,
                                 uint8_t *read_data_buf,
                                 size_t len_read_data);
void i2c_master_close(i2c_master_t *obj);

#ifdef __cplusplus
}
#endif

#endif // _DRV_I2C_MASTER_H_
// ------------------------------------------------------------------------- END
