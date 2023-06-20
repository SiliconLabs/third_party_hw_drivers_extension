/***************************************************************************//**
* @file triac_config.h
* @brief TRIAC Driver Configuration
********************************************************************************
* # License
* <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
********************************************************************************
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
*******************************************************************************/
#ifndef TRIAC_CONFIG_H_
#define TRIAC_CONFIG_H_

// <<< Use Configuration Wizard in Context Menu >>>

// <h>Debug settings

// <q TRIACDRV_DEBUG_ACMPOUT> Enable ACMP output pin for observation
// <i> Default: 0
#define TRIACDRV_DEBUG_ACMPOUT                      0

// <q TRIACDRV_DEBUG_PRSACMP> Enable PRSACMP output pin for observation
// <i> Default: 0
#define TRIACDRV_DEBUG_PRSACMP                      0

// <q TRIACDRV_DEBUG_PULSE_RISE_TIMER_OUT> Enable CC1 output to debug pin
// <i> Default: 0
#define TRIACDRV_DEBUG_PULSE_RISE_TIMER_OUT         0

// <q TRIACDRV_DEBUG_PULSE_FALL_TIMER_OUT> Enable CC2 output to debug pin
// <i> Default: 0
#define TRIACDRV_DEBUG_PULSE_FALL_TIMER_OUT         0

// <q TRIACDRV_DEBUG_PULSE_RISE_PRS_OUT> Enable CC1 PRS output to debug pin
// <i> Default: 0
#define TRIACDRV_DEBUG_PULSE_RISE_PRS_OUT           0

// <q TRIACDRV_DEBUG_PULSE_FALL_PRS_OUT> Enable CC2 PRS output to debug pin
// <i> Default: 0
#define TRIACDRV_DEBUG_PULSE_FALL_PRS_OUT           0

// </h> end Debug settings

// <h>Location settings

// <o TRIACDRV_DEBUG_ACMPOUT_LOC> acmpout loc
// <i> Default: 2
#define TRIACDRV_DEBUG_ACMPOUT_LOC                  2

// <o TRIACDRV_DEBUG_PRSACMP_LOC> prsacmp loc
// <i> Default: 0
#define TRIACDRV_DEBUG_PRSACMP_LOC                  0

// <o TRIACDRV_DEBUG_PULSE_RISE_TIMER_OUT_LOC> pulse rise timer out loc
// <i> Default: 4
#define TRIACDRV_DEBUG_PULSE_RISE_TIMER_OUT_LOC     4

// <o TRIACDRV_DEBUG_PULSE_FALL_TIMER_OUT_LOC> pulse fall timer out loc
// <i> Default: 4
#define TRIACDRV_DEBUG_PULSE_FALL_TIMER_OUT_LOC     4

// <o TRIACDRV_DEBUG_PULSE_RISE_PRS_OUT_LOC> pulse rise prs out loc
// <i> Default: 0
#define TRIACDRV_DEBUG_PULSE_RISE_PRS_OUT_LOC       0

// <o TRIACDRV_DEBUG_PULSE_FALL_PRS_OUT_LOC> pulse fall prs out loc
// <i> Default: 2
#define TRIACDRV_DEBUG_PULSE_FALL_PRS_OUT_LOC       2

// </h> end Location settings

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio> TRIACDRV_DEBUG_ACMPOUT
// $[GPIO_TRIACDRV_DEBUG_ACMPOUT]
#define TRIACDRV_DEBUG_ACMPOUT_PORT                 gpioPortD
#define TRIACDRV_DEBUG_ACMPOUT_PIN                  6
// [GPIO_TRIACDRV_DEBUG_ACMPOUT]$

// <gpio> TRIACDRV_DEBUG_PRSACMP
// $[GPIO_TRIACDRV_DEBUG_PRSACMP]
#define TRIACDRV_DEBUG_PRSACMP_PORT                 gpioPortC
#define TRIACDRV_DEBUG_PRSACMP_PIN                  1
// [GPIO_TRIACDRV_DEBUG_PRSACMP]$

// <gpio> TRIACDRV_DEBUG_PULSE_RISE_TIMER_OUT
// $[GPIO_TRIACDRV_DEBUG_PULSE_RISE_TIMER_OUT]
#define TRIACDRV_DEBUG_PULSE_RISE_TIMER_OUT_PORT    gpioPortD
#define TRIACDRV_DEBUG_PULSE_RISE_TIMER_OUT_PIN     7
// [GPIO_TRIACDRV_DEBUG_PULSE_RISE_TIMER_OUT]$

// <gpio> TRIACDRV_DEBUG_PULSE_FALL_TIMER_OUT
// $[GPIO_TRIACDRV_DEBUG_PULSE_FALL_TIMER_OUT]
#define TRIACDRV_DEBUG_PULSE_FALL_TIMER_OUT_PORT    gpioPortC
#define TRIACDRV_DEBUG_PULSE_FALL_TIMER_OUT_PIN     13
// [GPIO_TRIACDRV_DEBUG_PULSE_FALL_TIMER_OUT]$

// <gpio> TRIACDRV_DEBUG_PULSE_RISE_PRS_OUT
// $[GPIO_TRIACDRV_DEBUG_PULSE_RISE_PRS_OUT]
#define TRIACDRV_DEBUG_PULSE_RISE_PRS_OUT_PORT      gpioPortC
#define TRIACDRV_DEBUG_PULSE_RISE_PRS_OUT_PIN       8
// [GPIO_TRIACDRV_DEBUG_PULSE_RISE_PRS_OUT]$

// <gpio> TRIACDRV_DEBUG_PULSE_FALL_PRS_OUT
// $[GPIO_TRIACDRV_DEBUG_PULSE_FALL_PRS_OUT]
#define TRIACDRV_DEBUG_PULSE_FALL_PRS_OUT_PORT      gpioPortD
#define TRIACDRV_DEBUG_PULSE_FALL_PRS_OUT_PIN       6
// [GPIO_TRIACDRV_DEBUG_PULSE_FALL_PRS_OUT]$

// <<< sl:end pin_tool >>>

#endif /* TRIAC_CONFIG_H_ */
