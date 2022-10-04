/***************************************************************************//**
 * @file drv_port.h
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

#ifndef _DRV_PORT_H_
#define _DRV_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "drv_name.h"
#include "hal_gpio.h"

typedef enum {
  PIN_DIRECTION_DIGITAL_INPUT = 0, PIN_DIRECTION_DIGITAL_OUTPUT = 1
} pin_direction_t;

typedef struct {
  hal_gpio_port_t port;
  port_name_t name;
} port_t;

void port_init(port_t *port,
               port_name_t name,
               port_size_t mask,
               pin_direction_t direction);
void port_write(port_t *port, port_size_t value);
port_size_t port_read(port_t *port);

#ifdef __cplusplus
}
#endif

#endif // _DRV_PORT_H_
// ------------------------------------------------------------------------- END
