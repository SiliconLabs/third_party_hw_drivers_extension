/***************************************************************************//**
 * @file sparkfun_weather_station_wind_direction.h
 * @brief Sparkfun Wind Direction Sensor header file.
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

#ifndef SPARKFUN_WEATHER_STATON_WIND_DIRECTION_H
#define SPARKFUN_WEATHER_STATON_WIND_DIRECTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sl_status.h"

#define CLK_SRC_ADC_FREQ                   20000000 // CLK_SRC_ADC
#define CLK_ADC_FREQ                       10000000 // CLK_ADC - 10 MHz max in
                                                    //   normal mode

#define SPARKFUN_WIND_DIRECTION_COUNT      16
#define SPARKFUN_WIND_DIRECTION_INCREMENTS 22.5

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif

/***************************************************************************//**
 * @brief
 *  Initializes the wind direction detection
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure setting up ADC pin
 ******************************************************************************/
sl_status_t sparkfun_weatherstation_winddirection_init(void);

/***************************************************************************//**
 * @brief
 *  Reads the wind direction from the sensor
 *
 * @param[out] wind_direction_degrees
 *  The direction of the wind in degrees
 * *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_NULL_POINTER the output pointer is null
 ******************************************************************************/
sl_status_t sparkfun_weatherstation_winddirection_read_direction(
  float *wind_direction_degrees);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SPARKFUN_WEATHER_STATON_WIND_DIRECTION_H */
