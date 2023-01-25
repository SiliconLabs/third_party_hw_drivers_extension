/***************************************************************************//**
 * @file w5x00_config.h
 * @brief Wiznet w5x00 config.
 * @version 0.0.1
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
#ifndef Wx500_COMMON_H_
#define Wx500_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

// Enable log printing
#if defined(SL_CATALOG_APP_LOG_PRESENT) && APP_LOG_ENABLE
/// use applog for the log printing
#include "app_log.h"
#define w5x00_log_printf              app_log_append
#define W5x00_LOG_ENABLE
#elif defined(SL_CATALOG_RETARGET_STDIO_PRESENT)
/// use stdio printf for the log printing
#include <stdio.h>
#define w5x00_log_printf              printf
#define W5x00_LOG_ENABLE
#endif

#ifdef __cplusplus
}
#endif
#endif /* Wx500_CONFIG_H_ */
