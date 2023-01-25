/***************************************************************************//**
* @file max17048_config.h
* @brief MAX17048 Driver Configuration
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
#ifndef MAXIM_FUEL_GAUGE_MAX17048_CONFIG_H_
#define MAXIM_FUEL_GAUGE_MAX17048_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>
// <h> Connection
// <o MAX17048_I2C_ADDRESS> I2C Address <0x1..0x7E:0x1><f.h>
// <i> Default: 0x36
#define MAX17048_I2C_ADDRESS                        0x36
// </h>

// <h> User-Settings

// <o MAX17048_CONFIG_STABILIZATION_DELAY> Stabilization delay (ms)
// <i> Default: 0
#define MAX17048_CONFIG_STABILIZATION_DELAY         0

// <o MAX17048_CONFIG_RCOMP_UPDATE_INTERVAL_MS> Rcomp update interval (ms)
// <i> Default: 60000
// <i> The driver calculates and updates the RCOMP factor at a rate of
// <i> 1000 ms <= MAX17048_CONFIG_RCOMP_UPDATE_INTERVAL_MS <= 60000 ms
// <i> and defaults to 1 minute (60000 ms = 1 minute).
#define MAX17048_CONFIG_RCOMP_UPDATE_INTERVAL_MS    60000

// <q MAX17048_CONFIG_ENABLE_HW_QSTRT> Enable battery quick start
// <i> Default: 1
#define MAX17048_CONFIG_ENABLE_HW_QSTRT             1

// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MAX17048_CONFIG_ALRT
// $[GPIO_MAX17048_CONFIG_ALRT]
#define MAX17048_CONFIG_ALRT_PORT                   gpioPortC
#define MAX17048_CONFIG_ALRT_PIN                    1
// [GPIO_MAX17048_CONFIG_ALRT]$

// <gpio> MAX17048_CONFIG_ENABLE_QSTRT
// $[GPIO_MAX17048_CONFIG_ENABLE_QSTRT]
#define MAX17048_CONFIG_ENABLE_QSTRT_PORT           gpioPortC
#define MAX17048_CONFIG_ENABLE_QSTRT_PIN            2
// [GPIO_MAX17048_CONFIG_ENABLE_QSTRT]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* MAXIM_FUEL_GAUGE_MAX17048_CONFIG_H_ */
