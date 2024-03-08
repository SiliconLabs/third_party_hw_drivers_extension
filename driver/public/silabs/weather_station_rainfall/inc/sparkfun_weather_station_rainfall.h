/***************************************************************************//**
 * @file sparkfun_weather_station_rainfall.h
 * @brief Sparkfun Rainfall Sensor header file.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef SPARKFUN_WEATHER_STATON_RAINFALL_H
#define SPARKFUN_WEATHER_STATON_RAINFALL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sl_status.h"

#define SPARKFUN_CALIBRATION_MIN_MILLIS_PER_RAINFALL 100
#define SPARKFUN_MM_PER_RAINFALL_COUNTS              0.2794;

/***************************************************************************//**
 * @brief
 *  Initializes the rainfall detection
 ******************************************************************************/
void sparkfun_weatherstation_rainfall_init(void);

/***************************************************************************//**
 * @brief
 *  Resets the rainfall event counter
 ******************************************************************************/
void sparkfun_weatherstation_rainfall_reset_rainfall_count(void);

/***************************************************************************//**
 * @brief
 *  Gets the rainfall event counter's current value
 *
 * @param[out] count
 *  The number of rainfall events detected since the last reset
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_NULL_POINTER the output pointer is null
 ******************************************************************************/
sl_status_t sparkfun_weatherstation_rainfall_get_rainfall_count(
  uint32_t *count);

/***************************************************************************//**
 * @brief
 *  Gets the rainfall amount in mm
 *
 * @param[out] raifall
 *  The amount of rainfall since the last reset in mm
 * *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_NULL_POINTER the output pointer is null
 ******************************************************************************/
sl_status_t sparkfun_weatherstation_rainfall_get_rainfall_amount(
  float *rainfall);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SPARKFUN_WEATHER_STATON_RAINFALL_H */
