/***************************************************************************//**
 * @file mikroe_servo_config.h
 * @brief Mikroe Servo Click Configuration
 * @version 1.2.0
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

#ifndef MIKORE_SERVO_CONFIG_H_
#define MIKORE_SERVO_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> I2C address
// <o SERVO_PCA9685_ADDRESS> SERVO PCA9685 ADDRESS
// <0=> None
// <0x40=> 0x40
// <0x41=> 0x41
// <0x42=> 0x42
// <0x43=> 0x43
// <0x44=> 0x44
// <0x45=> 0x45
// <0x46=> 0x46
// <0x47=> 0x47
// <i> Default: 0
#define SERVO_PCA9685_ADDRESS           0x40

// <o SERVO_LTC2497_ADDRESS> SERVO LTC2497 ADDRESS
// <0=> None
// <0x14=> 0x14
// <0x16=> 0x16
// <0x26=> 0x26
// <0x34=> 0x34
// <0x56=> 0x56
// <0x64=> 0x64
// <0x74=> 0x74
// <0x76=> 0x76
// <i> Default: 0x14
#define SERVO_LTC2497_ADDRESS           0x14
// </h>

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> SERVO_OUTPUT_ENABLE
// $[GPIO_SERVO_OUTPUT_ENABLE]
#warning "OE pin for Servo Click is not configured"
// #define SERVO_OUTPUT_ENABLE_PORT                   gpioPortB
// #define SERVO_OUTPUT_ENABLE_PIN                    0
// [GPIO_SERVO_OUTPUT_ENABLE]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}

#endif
#endif /* MIKORE_SERVO_CONFIG_H_ */
