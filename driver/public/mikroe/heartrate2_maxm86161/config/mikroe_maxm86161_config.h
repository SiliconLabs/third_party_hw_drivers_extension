/***************************************************************************//**
 * @file scl_maxm86161.h
 * @brief SCL MAXM86161 Configuration
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

#ifndef SCL_MAXM86161_CONFIG_H_
#define SCL_MAXM86161_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// A CMSIS annotation block starts with the following line:
// <<< Use Configuration Wizard in Context Menu >>>

// <o MIKROE_MAXM86161_DEFAULT_DEVICE_ADDRESS> I2C Address <0x1..0x7E:0x1><f.h>
#define MIKROE_MAXM86161_DEFAULT_DEVICE_ADDRESS 0x62

// The block ends with the following line or at the end of the file:
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MAXM86161_CS
// $[GPIO_MAXM86161_CS]
#define MAXM86161_CS_PORT                        gpioPortC
#define MAXM86161_CS_PIN                         3
// [GPIO_MAXM86161_CS]$

// <gpio optional=true> MAXM86161_INT
// $[GPIO_MAXM86161_INT]
#define MAXM86161_INT_PORT                       gpioPortB
#define MAXM86161_INT_PIN                        3
// [GPIO_MAXM86161_INT]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* SCL_MAXM86161_CONFIG_H_ */
