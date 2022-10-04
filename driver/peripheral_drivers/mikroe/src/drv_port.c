/***************************************************************************//**
 * @file drv_port.c
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

#include "drv_port.h"
#include "em_gpio.h"

void port_init(port_t *port,
               port_name_t name,
               port_size_t mask,
               pin_direction_t direction)
{
  GPIO_Port_TypeDef port_index;
  unsigned int pin_index, port_size;

  port->port.base = (uint32_t)-1;
  port->port.mask = 0;
  if (HAL_PORT_NC != name) {
    port->name = name;
  }

  port_index = (GPIO_Port_TypeDef)hal_gpio_port_index(name);
  port_size = _GPIO_PORT_SIZE(port_index);
  if ((GPIO_PORT_VALID(port_index)) && (port_size > 0)) {
    port->port.base = port_index;
    port->port.base = 0;
    for (pin_index = 0; pin_index < port_size; pin_index++) {
      if (mask & (1 << pin_index)) {
        if (PIN_DIRECTION_DIGITAL_INPUT == direction) {
          GPIO_PinModeSet(port_index,
                          pin_index,
                          gpioModeInput,
                          0);
          port->port.mask |= (1 << pin_index);
        } else if (PIN_DIRECTION_DIGITAL_OUTPUT == direction) {
          GPIO_PinModeSet(port_index,
                          pin_index,
                          gpioModePushPull,
                          0);
          port->port.mask |= (1 << pin_index);
        } else {
          // Invalid mode
          port->port.base = GPIO_PORT_MAX + 1;
          port->port.mask = 0;
        }
      }
    }
  } else {
    port->port.base = GPIO_PORT_MAX + 1;
    port->port.mask = 0;
  }
}

void port_write(port_t *port, port_size_t value)
{
  if (GPIO_PORT_VALID(port->port.base) && port->port.mask) {
    GPIO_PortOutSetVal((GPIO_Port_TypeDef)port->port.base,
                       value,
                       port->port.mask);
  }
}

port_size_t port_read(port_t *port)
{
  if (GPIO_PORT_VALID(port->port.base) && port->port.mask) {
    return GPIO_PortInGet((GPIO_Port_TypeDef)port->port.base) & port->port.mask;
  }
  return 0;
}

// ------------------------------------------------------------------------- END
