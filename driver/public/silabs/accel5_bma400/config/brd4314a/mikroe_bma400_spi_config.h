/***************************************************************************//**
 * @file mikroe_bma400_spi_config.h
 * @brief Configuration file for BMA400
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef MIKROE_BMA400_CONFIG_H_
#define MIKROE_BMA400_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "em_gpio.h"

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIKROE_BMA400_INT1
// $[GPIO_MIKROE_BMA400_INT1]
#define MIKROE_BMA400_INT1_PORT                  gpioPortB
#define MIKROE_BMA400_INT1_PIN                   4
// [GPIO_MIKROE_BMA400_INT1]$

// <gpio optional=true> MIKROE_BMA400_INT2
// $[GPIO_MIKROE_BMA400_INT2]
#define MIKROE_BMA400_INT2_PORT                  gpioPortB
#define MIKROE_BMA400_INT2_PIN                   3
// [GPIO_MIKROE_BMA400_INT2]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_BMA400_CONFIG_H_ */
