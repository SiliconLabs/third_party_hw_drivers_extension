/***************************************************************************//**
 * @file touch_screen_config.h
 * @brief Configuration file for touch screen driver.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#ifndef TOUCH_SCREEN_CONFIG_H_
#define TOUCH_SCREEN_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h>Touch settings

// <o TOUCHSCREEN_WIDTH> Touch screen width
// <i> Default: 240
#define TOUCHSCREEN_WIDTH              240

// <o TOUCHSCREEN_HEIGHT> Touch screen height
// <i> Default: 320
#define TOUCHSCREEN_HEIGHT              320

// </h>

// <h>CALIBRATION settings

// <q CALIBRATION_PRINTF> Enable calibration printf
// <i> Default: 0
#define CALIBRATION_PRINTF       0

// <o CALIB_X_MIN> Calib X-min
// <i> Default: 460
#define CALIB_X_MIN              460

// <o CALIB_X_MAX> Calib X-max
// <i> Default: 3620
#define CALIB_X_MAX              3620

// <o CALIB_Y_MIN> Calib Y-min
// <i> Default: 340
#define CALIB_Y_MIN              340

// <o CALIB_Y_MAX> Calib Y-max
// <i> Default: 3580
#define CALIB_Y_MAX              3580

// </h>

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>
// <gpio optional=true> XP
// $[GPIO_XP]
#define XP_PORT                            gpioPortB
#define XP_PIN                             2
// [GPIO_XP]$

// <gpio optional=true> YP
// $[GPIO_YP]
#define YP_PORT                            gpioPortD
#define YP_PIN                             2
// [GPIO_YP]$

// <gpio optional=true> XM
// $[GPIO_XM]
#define XM_PORT                            gpioPortB
#define XM_PIN                             4
// [GPIO_XM]$

// <gpio optional=true> YM
// $[GPIO_YM]
#define YM_PORT                            gpioPortB
#define YM_PIN                             3
// [GPIO_YM]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
extern "C"
}
#endif

#endif /* TOUCH_SCREEN_CONFIG_H_ */
