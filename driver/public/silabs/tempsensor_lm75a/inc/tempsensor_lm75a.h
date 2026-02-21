/*
 * lm75_driver.h
 *
 *  Created on: Feb 20, 2026
 *      Author: nalethak
 */

#ifndef LM75_DRIVER_H_
#define LM75_DRIVER_H_


#include "sl_status.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/***************************************************************************//**
 * Initialize LM75 driver
 ******************************************************************************/
sl_status_t lm75_init(void);

/***************************************************************************//**
 * Read temperature in Celsius
 ******************************************************************************/
sl_status_t lm75_get_temperature(float *temp_c);

/***************************************************************************//**
 * Convert Celsius to Fahrenheit
 ******************************************************************************/
float lm75_celsius_to_fahrenheit(float c);

#endif /* LM75_DRIVER_H_ */
