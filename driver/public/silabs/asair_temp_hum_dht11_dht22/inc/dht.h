/***************************************************************************//**
 * @file  dht.h
 * @brief DHT11/DHT22 Temperature/Humidity Sensor driver (implementation).
 *******************************************************************************
 * # License
 * <b>Copyright 2026 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef DHT_H
#define DHT_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_status.h"      // Status codes (SL_STATUS_OK, etc.)

// Supported sensors
typedef enum {
  DHT_SENSOR_DHT11 = 0,
  DHT_SENSOR_DHT22,
  DHT_SENSOR_INVALID
} dht_sensor_t;

/***************************************************************************//**
 * Initialize the DHT driver and configure the data GPIO.
 *
 * @param[in] gpioPort The GPIO Port connecting to DHT sensor data pin.
 * @param[in] gpioPin The GPIO Pin connecting to DHT sensor data pin.
 * @param[in] sensor Target DHT sensor - DHT_SENSOR_DHT11 or DHT_SENSOR_DHT22.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
sl_status_t dht_init(uint8_t gpioPort, uint8_t gpioPin, dht_sensor_t sensor);

/***************************************************************************//**
 * Read temperature and humidity data from the DHT sensor.
 *
 * Returned values are scaled by 10 to preserve one decimal place.
 *
 * @param[out] humidity_x10 Humidity value in x10 format.
 * @param[out] temperature_x10 Temperature value in x10 format.
 *
 * @return SL_STATUS_OK if the read operation succeeds.
 ******************************************************************************/
sl_status_t dht_read_x10(int16_t *humidity_x10, int16_t *temperature_x10);

#endif // DHT_H
