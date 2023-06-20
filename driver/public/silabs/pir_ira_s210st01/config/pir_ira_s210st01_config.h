/***************************************************************************//**
 * @file pir_ira_s210st01_config.h
 * @brief Configuration file for Sparkfun dosimeter type5.
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

#ifndef PIR_IRA_S210ST01_CONFIG_H_
#define PIR_IRA_S210ST01_CONFIG_H_

#include "em_gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

// <<< sl:start pin_tool >>>

// <gpio optional=false> PIR_IRA_S210ST01_ADC_P
// $[GPIO_PIR_IRA_S210ST01_ADC_P]
#define PIR_IRA_S210ST01_ADC_P_PORT              gpioPortD
#define PIR_IRA_S210ST01_ADC_P_PIN               8
// [GPIO_PIR_IRA_S210ST01_ADC_P]$

// <gpio optional=false> PIR_IRA_S210ST01_ADC_N
// $[GPIO_PIR_IRA_S210ST01_ADC_N]
#define PIR_IRA_S210ST01_ADC_N_PORT              gpioPortA
#define PIR_IRA_S210ST01_ADC_N_PIN               6
// [GPIO_PIR_IRA_S210ST01_ADC_N]$

// <gpio optional=true> PIR_IRA_S210ST01_LDO_SHDN_B
// $[GPIO_PIR_IRA_S210ST01_LDO_SHDN_B]
#define PIR_IRA_S210ST01_LDO_SHDN_B_PORT         gpioPortC
#define PIR_IRA_S210ST01_LDO_SHDN_B_PIN          9
// [GPIO_PIR_IRA_S210ST01_LDO_SHDN_B]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* PIR_IRA_S210ST01_CONFIG_H_ */
