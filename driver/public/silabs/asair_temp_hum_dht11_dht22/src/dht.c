/***************************************************************************//**
 * @file  dht.c
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
 * This software is provided 'as-is', without any express or implied
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

#include "dht.h"

#ifdef SLI_SI917
#include "sl_si91x_peripheral_gpio.h"
#include "sl_si91x_driver_gpio.h"
#else
#include "sl_device_gpio.h"
#include "sl_hal_gpio.h"
#include "sl_udelay.h"    // Microsecond delay API (sl_udelay_wait)
#endif

#define GPIO_PIN_LOW             0u
#define GPIO_PIN_HIGH            1u
#define DATA_PIN_TIMEOUT_US      100u
#define DATA_BIT_SAMPLE_US       35u

#ifdef SLI_SI917
// Tune this value per clock configuration if the bit-banged delay drifts.
#define DELAY_US_COUNTER         20u
// 917 doesn't have an accurate microsecond timer.
// With the current direct-register read path, ~50 us(Not accurate) is a reliable threshold
// for distinguishing a DHT data '1' from a data '0'.
#define DATA_BIT_1_DURATION_US   15u
static sl_si91x_gpio_pin_config_t gpio_config;
#else
static sl_gpio_t data_pin;
#endif

static dht_sensor_t dht_sensor;
static bool         is_initialized = false;

static sl_status_t dht_platform_init_pin(uint8_t gpioPort, uint8_t gpioPin);
static sl_status_t dht_platform_drive_low(void);
static sl_status_t dht_platform_drive_high(void);
static sl_status_t dht_platform_release_line(void);
static uint8_t dht_platform_get_pin(void);
static void dht_delay_us(uint32_t delay_us);
static sl_status_t wait_data_pin(uint8_t current_state, uint16_t timeout_us);
static sl_status_t dht_read_raw(uint8_t out[5]);

/***************************************************************************//**
 * Initialize the DHT driver and configure the data GPIO.
 *
 * @param[in] gpioPort The GPIO Port connecting to DHT sensor data pin.
 * @param[in] gpioPin The GPIO Pin connecting to DHT sensor data pin.
 * @param[in] sensor Target DHT sensor - DHT_SENSOR_DHT11 or DHT_SENSOR_DHT22.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
sl_status_t dht_init(uint8_t gpioPort, uint8_t gpioPin, dht_sensor_t sensor)
{
  sl_status_t status;

  if (sensor >= DHT_SENSOR_INVALID) {
    return SL_STATUS_FAIL;
  }

  dht_sensor = sensor;

  status = dht_platform_init_pin(gpioPort, gpioPin);
  if (status != SL_STATUS_OK) {
    return status;
  }

  is_initialized = true;
  return SL_STATUS_OK;
}

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
sl_status_t dht_read_x10(int16_t *humidity_x10, int16_t *temperature_x10)
{
  uint8_t data[5];
  sl_status_t ret;
  uint8_t checksum;

  if ((humidity_x10 == NULL) || (temperature_x10 == NULL)) {
    return SL_STATUS_FAIL;
  }
  if (!is_initialized) {
    return SL_STATUS_NOT_READY;
  }

  ret = dht_read_raw(data);
  if (ret != SL_STATUS_OK) {
    return ret;
  }

  // Common checksum rule: (sum of first 4 bytes) & 0xFF must equal byte 4.
  checksum = (uint8_t)(data[0] + data[1] + data[2] + data[3]);
  if (checksum != data[4]) {
    return SL_STATUS_FAIL;
  }

  if (dht_sensor == DHT_SENSOR_DHT11) {
    int16_t rh_x10 = (int16_t)((int16_t)data[0] * 10 + (int16_t)data[1]);
    bool is_negative = ((data[3] & 0x80u) != 0u);
    int16_t t_abs_x10 = (int16_t)((int16_t)data[2] * 10
                                  + (int16_t)(data[3] & 0x7Fu));
    int16_t t_x10 = is_negative ? (int16_t)(-t_abs_x10) : t_abs_x10;

    *humidity_x10    = rh_x10;
    *temperature_x10 = t_x10;
  } else { // DHT_SENSOR_DHT22
    uint16_t rh_raw = (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
    bool is_negative = ((data[2] & 0x80u) != 0u);
    uint16_t t_abs = (uint16_t)((((uint16_t)data[2] & 0x7Fu) << 8)
                                | (uint16_t)data[3]);

    int16_t rh_x10 = (rh_raw > 32767u) ? 32767 : (int16_t)rh_raw;
    int16_t t_x10  = (int16_t)t_abs;
    if (is_negative) {
      t_x10 = (int16_t)(-t_x10);
    }

    *humidity_x10    = rh_x10;
    *temperature_x10 = t_x10;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Read raw 40-bit data from the DHT sensor.
 *
 * @param[out] data Buffer to store raw sensor bytes.
 *
 * @return SL_STATUS_OK if the read operation succeeds.
 ******************************************************************************/
static sl_status_t dht_read_raw(uint8_t out[5])
{
  if (!is_initialized) {
    return SL_STATUS_NOT_READY;
  }

  // 1) Start signal:
  //    - DHT11: hold LOW >= 18 ms
  //    - DHT22: hold LOW >= 1 ms
  if (dht_platform_drive_low() != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }

  if (dht_sensor == DHT_SENSOR_DHT11) {
    dht_delay_us(20000);  // ~20 ms
  } else {
    dht_delay_us(1100);   // ~1.1 ms
  }

  // 2) Pull high ~30 us, then release to input.
  if (dht_platform_drive_high() != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  dht_delay_us(30);
  if (dht_platform_release_line() != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }

  // 3) Sensor response: ~80 us low -> ~80 us high -> first data low.
  if (wait_data_pin(GPIO_PIN_HIGH, DATA_PIN_TIMEOUT_US) != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  if (wait_data_pin(GPIO_PIN_LOW, DATA_PIN_TIMEOUT_US) != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  if (wait_data_pin(GPIO_PIN_HIGH, DATA_PIN_TIMEOUT_US) != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }

  // 4) Read 40 bits MSB-first. Each bit: ~50 us low, then high.
  //    ~26-28 us high => 0, ~70 us high => 1.
  for (uint8_t i = 0; i < 5; ++i) {
    uint8_t byte = 0;

    for (uint8_t bit = 0; bit < 8; ++bit) {
#ifdef SLI_SI917
      uint16_t high_time_us = 0;

      // Wait for the low period to end.
      if (wait_data_pin(GPIO_PIN_LOW, DATA_PIN_TIMEOUT_US) != SL_STATUS_OK) {
        return SL_STATUS_FAIL;
      }

      // Measure the following high pulse width.
      while (dht_platform_get_pin() == GPIO_PIN_HIGH) {
        if (++high_time_us > DATA_PIN_TIMEOUT_US) {
          return SL_STATUS_FAIL;
        }
        dht_delay_us(1);
      }

      if (high_time_us >= DATA_BIT_1_DURATION_US) {
        byte |= (uint8_t)(1u << (7u - bit));
      }
#else
      // Wait for the low period to end.
      if (wait_data_pin(GPIO_PIN_LOW, DATA_PIN_TIMEOUT_US) != SL_STATUS_OK) {
        return SL_STATUS_FAIL;
      }

      // Sample ~35 us into the high pulse.
      dht_delay_us(DATA_BIT_SAMPLE_US);
      if (dht_platform_get_pin() == GPIO_PIN_HIGH) {
        byte |= (uint8_t)(1u << (7u - bit));
      }

      // Wait for the line to return low before reading the next bit.
      if (wait_data_pin(GPIO_PIN_HIGH, DATA_PIN_TIMEOUT_US) != SL_STATUS_OK) {
        return SL_STATUS_FAIL;
      }
#endif
    }

    out[i] = byte;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Wait until the data pin changes from the specified state or timeout occurs.
 *
 * @param[in] current_state The current state of data pin.
 * @param[in] timeout_us Timeout duration in microseconds.
 *
 * @return SL_STATUS_OK if the pin changes state before timeout.
 ******************************************************************************/
static sl_status_t wait_data_pin(uint8_t current_state, uint16_t timeout_us)
{
  uint16_t time_us = 0;

  while (dht_platform_get_pin() == current_state) {
    if (++time_us > timeout_us) {
      return SL_STATUS_FAIL;
    }
    dht_delay_us(1);
  }

  return SL_STATUS_OK;
}

#ifdef SLI_SI917
/***************************************************************************//**
 * Initialize the platform-specific GPIO used by the DHT sensor.
 *
 * @param[in] gpioPort The GPIO Port connecting to DHT sensor data pin.
 * @param[in] gpioPin The GPIO Pin connecting to DHT sensor data pin.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
static sl_status_t dht_platform_init_pin(uint8_t gpioPort, uint8_t gpioPin)
{
  sl_status_t status;

  gpio_config.port_pin.port = (sl_gpio_port_t)gpioPort;
  gpio_config.port_pin.pin  = gpioPin;
  gpio_config.direction     = GPIO_OUTPUT;

  status = sl_gpio_driver_init();
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = sl_gpio_set_configuration(gpio_config);
  if (status != SL_STATUS_OK) {
    return status;
  }

  return sl_gpio_driver_set_pin(&gpio_config.port_pin);
}

/***************************************************************************//**
 * Drive the DHT data line low.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
static sl_status_t dht_platform_drive_low(void)
{
  sl_si91x_gpio_driver_set_pin_direction(gpio_config.port_pin.port,
                                          gpio_config.port_pin.pin,
                                          GPIO_OUTPUT);
  return sl_gpio_driver_clear_pin(&gpio_config.port_pin);
}

/***************************************************************************//**
 * Drive the DHT data line high.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
static sl_status_t dht_platform_drive_high(void)
{
  sl_si91x_gpio_driver_set_pin_direction(gpio_config.port_pin.port,
                                          gpio_config.port_pin.pin,
                                          GPIO_OUTPUT);
  return sl_gpio_driver_set_pin(&gpio_config.port_pin);
}

/***************************************************************************//**
 * Release the DHT data line and switch to input mode.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
static sl_status_t dht_platform_release_line(void)
{
  sl_si91x_gpio_driver_set_pin_direction(gpio_config.port_pin.port,
                                          gpio_config.port_pin.pin,
                                          GPIO_INPUT);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Get the current logic level of the DHT data pin.
 *
 * @return Current pin state.
 ******************************************************************************/
static uint8_t dht_platform_get_pin(void)
{
  // Direct register read is used to avoid extra driver-call overhead during
  // the timing-critical bit-read phase.
  return (uint8_t)ULP_GPIO->PIN_CONFIG[gpio_config.port_pin.pin].BIT_LOAD_REG;
}

/***************************************************************************//**
 * Delay execution for the specified number of microseconds.
 *
 * @param[in] us Delay duration in microseconds.
 ******************************************************************************/
static void dht_delay_us(uint32_t delay_us)
{
  // The accuracy is not good.
  // WiseConnect doesn't provide an accurate microsecond timer.
  for (uint32_t x = 0; x < (DELAY_US_COUNTER * delay_us); x++) {
    __NOP();
  }
}

#else
/***************************************************************************//**
 * Initialize the platform-specific GPIO used by the DHT sensor.
 *
 * @param[in] gpioPort The GPIO Port connecting to DHT sensor data pin.
 * @param[in] gpioPin The GPIO Pin connecting to DHT sensor data pin.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
static sl_status_t dht_platform_init_pin(uint8_t gpioPort, uint8_t gpioPin)
{
  data_pin.port = gpioPort;
  data_pin.pin  = gpioPin;

  // Configure the pin as input with pull-up initially; the data line idles high.
  sl_hal_gpio_set_pin_mode(&data_pin, SL_GPIO_MODE_INPUT_PULL, GPIO_PIN_HIGH);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Drive the DHT data line low.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
static sl_status_t dht_platform_drive_low(void)
{
  sl_hal_gpio_set_pin_mode(&data_pin, SL_GPIO_MODE_PUSH_PULL, GPIO_PIN_LOW);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Drive the DHT data line high.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
static sl_status_t dht_platform_drive_high(void)
{
  sl_hal_gpio_set_pin(&data_pin);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Release the DHT data line and switch to input mode.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
static sl_status_t dht_platform_release_line(void)
{
  sl_hal_gpio_set_pin_mode(&data_pin, SL_GPIO_MODE_INPUT_PULL, GPIO_PIN_HIGH);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Get the current logic level of the DHT data pin.
 *
 * @return Current pin state.
 ******************************************************************************/
static uint8_t dht_platform_get_pin(void)
{
  return (uint8_t)sl_hal_gpio_get_pin_input(&data_pin);
}

/***************************************************************************//**
 * Delay execution for the specified number of microseconds.
 *
 * @param[in] us Delay duration in microseconds.
 ******************************************************************************/
static void dht_delay_us(uint32_t delay_us)
{
  sl_udelay_wait(delay_us);
}
#endif
