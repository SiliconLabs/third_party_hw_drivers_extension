/***************************************************************************//**
 * @file mikroe_gps_click_lea6s_config.h
 * @brief mikroe_gps_click_lea6s_config.h
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
#ifndef MIKROE_GPS_CLICK_LEA6S_CONFIG_H_
#define MIKROE_GPS_CLICK_LEA6S_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "em_gpio.h"

#define SL_EMLIB_GPIO_INIT_MIKROE_LEAS6_RESET_MODE     gpioModePushPull
#define SL_EMLIB_GPIO_INIT_MIKROE_LEAS6_RESET_DOUT     0
#define SL_EMLIB_GPIO_INIT_MIKROE_LEAS6_RESET_PORT     gpioPortC
#define SL_EMLIB_GPIO_INIT_MIKROE_LEAS6_RESET_PIN      6

#define SL_EMLIB_GPIO_INIT_MIKROE_LEAS6_TIMEPULSE_MODE gpioModeInput
#define SL_EMLIB_GPIO_INIT_MIKROE_LEAS6_TIMEPULSE_DOUT 0
#define SL_EMLIB_GPIO_INIT_MIKROE_LEAS6_TIMEPULSE_PORT gpioPortB
#define SL_EMLIB_GPIO_INIT_MIKROE_LEAS6_TIMEPULSE_PIN  4

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_GPS_CLICK_LEA6S_CONFIG_H_ */
