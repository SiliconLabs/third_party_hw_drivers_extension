/***************************************************************************//**
 * @file drv_digital_in_si91x.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - Digital IN for Si91x
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

#include "drv_digital_in.h"
#include "sl_si91x_peripheral_gpio.h"

static err_t drv_digital_in_init(digital_in_t *in,
                                 pin_name_t name,
                                 sl_gpio_mode_t mode,
                                 uint32_t out);

static uint8_t sl_si91x_gpio_pad_mapping(unsigned int pin)
{
  switch (pin) {
    case 6:
      return 1;
    case 7:
      return 2;
    case 8:
      return 3;
    case 9:
      return 4;
    case 10:
      return 5;
    case 11:
      return 6;
    case 12:
      return 7;
    case 15:
      return 8;
    case 31:
    case 32:
    case 33:
    case 34:
      return 9;
    case 46:
      return 10;
    case 47:
      return 11;
    case 48:
      return 12;
    case 49:
      return 13;
    case 50:
      return 14;
    case 51:
      return 15;
    case 52:
      return 16;
    case 53:
      return 17;
    case 54:
      return 18;
    case 55:
      return 19;
    case 56:
      return 20;
    case 57:
      return 21;
    default:
      return 0;
  }
}

err_t digital_in_init(digital_in_t *in, pin_name_t name)
{
  return drv_digital_in_init(in, name, SL_GPIO_MODE_0, 0);
}

err_t digital_in_pullup_init(digital_in_t *in, pin_name_t name)
{
  return drv_digital_in_init(in, name, SL_GPIO_MODE_0, 1);
}

err_t digital_in_pulldown_init(digital_in_t *in, pin_name_t name)
{
  return drv_digital_in_init(in, name, SL_GPIO_MODE_0, 0);
}

uint8_t digital_in_read(digital_in_t *in)
{
  return sl_gpio_get_pin_input(in->pin.base, in->pin.mask);
}

static err_t drv_digital_in_init(digital_in_t *in,
                                 pin_name_t name,
                                 sl_gpio_mode_t mode,
                                 uint32_t out)
{
  sl_gpio_port_t port_index;
  unsigned int pin_index;
  uint8_t pad_number;

  in->pin.base = (uint32_t) -1;
  in->pin.mask = 0;
  if (HAL_PIN_NC == name) {
    return DIGITAL_IN_UNSUPPORTED_PIN;
  }

  port_index = (sl_gpio_port_t) hal_gpio_port_index(name);
  pin_index = hal_gpio_pin_index(name);

  if (!SL_GPIO_VALIDATE_PORT(port_index)) {
    return DIGITAL_IN_UNSUPPORTED_PIN;
  }
  if (port_index == SL_ULP_GPIO_PORT) {
    if (!SL_GPIO_VALIDATE_ULP_PORT_PIN(port_index, pin_index)) {
      return DIGITAL_IN_UNSUPPORTED_PIN;
    }
  } else {
    if (!SL_GPIO_NDEBUG_PORT_PIN(port_index, pin_index)) {
      return DIGITAL_IN_UNSUPPORTED_PIN;
    }
  }

  if (port_index == SL_ULP_GPIO_PORT) {
    sl_si91x_gpio_enable_clock((sl_si91x_gpio_select_clock_t)ULPCLK_GPIO);
    sl_si91x_gpio_enable_ulp_pad_receiver(pin_index);
  } else {
    sl_si91x_gpio_enable_clock((sl_si91x_gpio_select_clock_t)M4CLK_GPIO);
    sl_si91x_gpio_enable_pad_receiver(name);
    pad_number = sl_si91x_gpio_pad_mapping(name);
    if (pad_number) {
      sl_si91x_gpio_enable_pad_selection(pad_number);
    }
  }

  sl_gpio_set_pin_mode(port_index, pin_index, mode, out);
  sl_si91x_gpio_set_pin_direction(port_index,
                                  pin_index,
                                  (sl_si91x_gpio_direction_t)GPIO_INPUT);

  in->pin.base = port_index;
  in->pin.mask = pin_index;
  return DIGITAL_IN_SUCCESS;
}

// ------------------------------------------------------------------------- END
