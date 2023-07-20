/***************************************************************************//**
 * @file app.c
 * @brief Top level application functions
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
 * # Experimental Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "sl_sleeptimer.h"
#include "sl_i2cspm_instances.h"

#include "app_log.h"
#include "app_assert.h"

#include "sparkfun_soil_moisture.h"

#define MOISTURE_THRESHOLD          70
#define READING_INTERVAL_MSEC       1000

static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t sc;
  uint16_t addr[255];
  uint8_t num_dev;

  app_log("\r\t\tSEN-17731 Soil moisture sensor test program\n");

  sc = sparkfun_soil_moisture_init(sl_i2cspm_qwiic,
                                   SPARKFUN_SOIL_MOISTURE_DEFAULT_ADDR);
  app_assert(sc == SL_STATUS_OK, "\rSparkfun soil moisture init fail\n");

  app_log("\r\nScanning address of all soil moisture sensors...\n");
  sc = sparkfun_soil_moisture_scan_address(addr, &num_dev);
  app_assert((sc == SL_STATUS_OK) || (num_dev != 0), "\rNo device connected\n");
  for (int i = 0; i < num_dev; i++) {
    app_log("\rDevice %d: address: 0x%x\n", i + 1, *(addr + i));
  }

  app_log("\r\nStart calibrating the sensor...\n");
  sparkfun_soil_moisture_set_dry_value(90);
  sparkfun_soil_moisture_set_wet_value(1023);
  app_log("\rCalibrating done...\n");

  sc = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                             READING_INTERVAL_MSEC,
                                             app_timer_cb,
                                             NULL,
                                             0,
                                             1);
  if (sc != SL_STATUS_OK) {
    app_log("\r\n > Start periodic measuring soil moisture Fail\n");
  } else {
    app_log("\r\n > Start periodic measuring soil moisture...\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t moisture;
  sl_status_t sc;

  if (app_timer_expire == false) {
    return;
  }

  app_timer_expire = false;
  sc = sparkfun_soil_moisture_get_moisture(&moisture);

  if (sc != SL_STATUS_OK) {
    app_log("\r > Reading data failed\n");
  } else {
    app_log("\r > Soil moisture: %d%%\n", moisture);
    if (moisture > MOISTURE_THRESHOLD) {
      sparkfun_soil_moisture_led_on();
    } else {
      sparkfun_soil_moisture_led_off();
    }
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
