/***************************************************************************//**
 * @file sparkfun_weather_station_wind_speed.h
 * @brief Sparkfun Wind Speed Sensor header file.
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

#ifndef SPARKFUN_WEATHER_STATON_WIND_SPEED_H
#define SPARKFUN_WEATHER_STATON_WIND_SPEED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sl_status.h"

#define WINDSPEED_PCNT_PRS_CH 0

/***************************************************************************//**
 * @brief
 *  Initializes the wind speed detection
 ******************************************************************************/
void sparkfun_weatherstation_windspeed_init(void);

/***************************************************************************//**
 * @brief
 *  Reads the wind speed calculated at the last measurement
 *
 * @param[out] wind_speed
 *  The wind speed in km/h
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Stored windspeed is invalid
 * @retval SL_STATUS_NULL_POINTER the output pointer is null
 ******************************************************************************/
sl_status_t sparkfun_weatherstation_windspeed_get(float *wind_speed);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SPARKFUN_WEATHER_STATON_WIND_SPEED_H */
