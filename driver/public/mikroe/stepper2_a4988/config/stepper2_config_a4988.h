/***************************************************************************//**
 * @file stepper_config_a4988.h
 * @brief Stepper motor driver configuration.
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
#ifndef STEPPER_CONFIG_A4988_H_
#define STEPPER_CONFIG_A4988_H_

#ifdef __cplusplus
extern "C"
{
#endif

// <<< Use Configuration Wizard in Context Menu

// <h>Stepper Motor settings

// <o A4988_STEPPER_MAX_RPM> Stepper Motor Max RPM
// <i> Default: 320
#define A4988_STEPPER_MAX_RPM             320

// <o A4988_STEPPER_MOTOR_STEPS_PER_REV> Stepper Motor Steps Per REV
// <i> Default: 64
#define A4988_STEPPER_MOTOR_STEPS_PER_REV 64
// </h> end Stepper Motor config

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> A4988_STEPPER_STEP
// $[GPIO_A4988_STEPPER_STEP]
#define A4988_STEPPER_STEP_PORT           gpioPortB
#define A4988_STEPPER_STEP_PIN            4
// [GPIO_A4988_STEPPER_STEP]$

// <gpio optional=true> A4988_STEPPER_DIR
// $[GPIO_A4988_STEPPER_DIR]
#define A4988_STEPPER_DIR_PORT            gpioPortB
#define A4988_STEPPER_DIR_PIN             3
// [GPIO_A4988_STEPPER_DIR]$

// <gpio optional=true> A4988_STEPPER_ENABLE
// $[GPIO_A4988_STEPPER_ENABLE]
#define A4988_STEPPER_ENABLE_PORT         gpioPortB
#define A4988_STEPPER_ENABLE_PIN          0
// [GPIO_A4988_STEPPER_ENABLE]$

// <gpio optional=true> A4988_STEPPER_RST
// $[GPIO_A4988_STEPPER_RST]
#define A4988_STEPPER_RST_PORT            gpioPortC
#define A4988_STEPPER_RST_PIN             6
// [GPIO_A4988_STEPPER_RST]$

// <gpio optional=true> A4988_STEPPER_SLEEP
// $[GPIO_A4988_STEPPER_SLEEP]
#define A4988_STEPPER_SLEEP_PORT          gpioPortC
#define A4988_STEPPER_SLEEP_PIN           3
// [GPIO_A4988_STEPPER_SLEEP]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* STEPPER_CONFIG_A4988_H_ */
