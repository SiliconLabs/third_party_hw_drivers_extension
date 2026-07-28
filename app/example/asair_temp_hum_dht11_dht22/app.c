/***************************************************************************//**
 * @file  app.c
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2026 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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

#ifdef SLI_SI917
#include "sl_gpio_board.h"
#include "rsi_debug.h"
#else
#include "sl_device_gpio.h"
#include "app_log.h"
#endif

#include "sl_sleeptimer.h"
#include "dht.h"

#ifdef SLI_SI917
#define DHT_DATA_PORT              SL_SI91X_ULP_GPIO_6_PORT
#define DHT_DATA_PIN               SL_SI91X_ULP_GPIO_6_PIN
#define DHT_APP_LOG(...)           DEBUGOUT(__VA_ARGS__)
#elif defined(BOARD_BRD4182A)
#define DHT_DATA_PORT              SL_GPIO_PORT_B
#define DHT_DATA_PIN               3
#define DHT_APP_LOG(...)           app_log(__VA_ARGS__)
#elif defined(BOARD_BRD4187C)
#define DHT_DATA_PORT              SL_GPIO_PORT_C
#define DHT_DATA_PIN               7
#define DHT_APP_LOG(...)           app_log(__VA_ARGS__)
#else
// Select the GPIO for custom board or other radio boards
#define DHT_DATA_PORT              SL_GPIO_PORT_A
#define DHT_DATA_PIN               0
#define DHT_APP_LOG(...)           app_log(__VA_ARGS__)
#endif

// Select DHT sensor model
#define DHT_SENSOR_MODEL           DHT_SENSOR_DHT11
// Interval between sensor reading
#define SENSOR_READING_INTERVAL_MS 5000

static sl_sleeptimer_timer_handle_t sensor_timer;
static volatile bool is_timer_triggered = false;

static void print_data(int16_t rh_x10, int16_t t_x10);

void sensor_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t status;

  // DHT sensor initialization
  status = dht_init(DHT_DATA_PORT, DHT_DATA_PIN, DHT_SENSOR_MODEL);
  if (status != SL_STATUS_OK) {
    DHT_APP_LOG("DHT sensor initialization failed\r\n");
    return;
  }

  // Start periodic timer (non-blocking)
  status = sl_sleeptimer_start_periodic_timer_ms(
    &sensor_timer,
    SENSOR_READING_INTERVAL_MS,
    sensor_timer_callback,
    (void *)NULL,
    0,
    0
    );
  if (status != SL_STATUS_OK) {
    DHT_APP_LOG("Timer not started.\r\n");
    return;
  }
  DHT_APP_LOG("Initialization completed\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (is_timer_triggered) {
    int16_t rh_x10 = 0;
    int16_t t_x10 = 0;
    if (dht_read_x10(&rh_x10, &t_x10) == SL_STATUS_OK) {
      print_data(rh_x10, t_x10);
    } else {
      DHT_APP_LOG("%s read error\r\n",
                  (DHT_SENSOR_MODEL == DHT_SENSOR_DHT11) ? "DHT11" : "DHT22");
    }

    is_timer_triggered = false;
  }
}

/***************************************************************************//**
 * Timer callback function used to trigger periodic sensor readings.
 *
 * @param[in] handle The Timer Handle
 * @param[in] data User data pointer
 ******************************************************************************/
void sensor_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;
  is_timer_triggered = true;
}

/***************************************************************************//**
 * Print temperature and humidity data to the console.
 *
 * @param[in] temperature Temperature value in x10 format
 * @param[in] humidity Humidity value in x10 format
 ******************************************************************************/
static void print_data(int16_t rh_x10, int16_t t_x10)
{
  // RH formatting: e.g., 521 -> 52.1
  int rh_i = rh_x10 / 10;
  int rh_f = (rh_x10 < 0 ? -rh_x10 : rh_x10) % 10;

  // Temp formatting: e.g., -252 -> -25.2
  int sign = (t_x10 < 0) ? -1 : 1;
  int t_abs = (t_x10 < 0) ? -t_x10 : t_x10;
  int t_i = t_abs / 10;
  int t_f = t_abs % 10;

  DHT_APP_LOG("%s -> RH: %d.%d %%  |  T: %s%d.%d °C\r\n",
              (DHT_SENSOR_MODEL == DHT_SENSOR_DHT11) ? "DHT11" : "DHT22",
              rh_i, rh_f, (sign < 0 ? "-" : ""), t_i, t_f);
}

/***************************************************************************//**
 * This is used for RTOS based application to make app_process_action() being
 * called repeatedly.
 ******************************************************************************/
bool sl_main_start_task_should_continue(void)
{
  return true;
}
