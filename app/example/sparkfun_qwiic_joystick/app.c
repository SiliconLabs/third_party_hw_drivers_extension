/***************************************************************************//**
 * @file app.c
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
 * EXPERIMENTAL QUALITY
 * This code has not been formally tested and is provided as-is.
 * It is not suitable for production environments.
 * This code will not be maintained.
 *
 ******************************************************************************/
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "sl_i2cspm_instances.h"
#include "sparkfun_qwiic_joystick.h"

#define READING_INTERVAL_MSEC 200

static volatile bool read_position = false;
// Periodic timer handle.
static sl_sleeptimer_timer_handle_t app_periodic_timer;

static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
  uint8_t addr[255];
  uint8_t num_dev;

  // initialize joystick.
  sc = sparkfun_joystick_init(sl_i2cspm_qwiic, SPARKFUN_JOYSTICK_DEFAULT_ADDR);
  if (sc != SL_STATUS_OK) {
    app_log("Warning! Failed to initialize Joystick\r\n");
  } else {
    app_log("Joystick initialized successfully!\r\n");
  }

  app_log("\r\nScan I2C address of all Joystick connected on the I2C bus.\r\n");
  sc = sparkfun_joystick_scan_address(addr, &num_dev);
  for (int i = 0; i < num_dev; i++) {
    app_log("Device %d: address: 0x%x\r\n", i + 1, *addr + i);
  }

  // Start timer used for periodic indications.
  sl_sleeptimer_start_periodic_timer_ms(&app_periodic_timer,
                                        READING_INTERVAL_MSEC,
                                        app_periodic_timer_cb,
                                        NULL,
                                        0,
                                        1);
  app_log("\r\nStart reading Position\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t sc;
  uint16_t data_pos;
  uint8_t bt_pos;

  if (read_position == false) {
    return;
  }

  read_position = false;

  // Reading current horizontal position
  sc = sparkfun_joystick_read_horizontal_position(&data_pos);
  if (sc != SL_STATUS_OK) {
    app_log("Warning! Invalid Joystick reading\r\n");
  } else {
    app_log("X = %d, ", data_pos);
  }

  // Reading current vertical position
  sc = sparkfun_joystick_read_vertical_position(&data_pos);
  if (sc != SL_STATUS_OK) {
    app_log("Warning! Invalid Joystick reading\r\n");
  } else {
    app_log("Y = %d, ", data_pos);
  }

  // Read button postion
  sc = sparkfun_joystick_read_button_position(&bt_pos);
  if (sc != SL_STATUS_OK) {
    app_log("Warning! Invalid Joystick reading\r\n");
  } else {
    app_log("Button = %d\r\n", bt_pos);
  }
}

/**************************************************************************//**
 * Timer callback
 * Called periodically to reading data from joystick.
 *****************************************************************************/
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)data;
  (void)timer;

  read_position = true;
}
