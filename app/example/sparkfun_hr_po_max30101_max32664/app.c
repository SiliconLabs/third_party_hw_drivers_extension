/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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
 * This code has not been formally tested and is provided as-is. It is not
 * suitable for production environments. In addition, this code will not be
 * maintained and there may be no bug maintenance planned for these resources.
 * Silicon Labs may update projects from time to time.
 ******************************************************************************/
#include "sl_simple_timer.h"
#include "sl_i2cspm_instances.h"

#include "app_log.h"

#include "sparkfun_max30101_max32664.h"

static bio_hub_data_t libBpm;
static sl_simple_timer_t app_bio_hub_timer;

static void app_bio_hub_timer_handle(sl_simple_timer_t *timer, void *data);
static void app_bio_hub_init(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_bio_hub_init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}

static void app_bio_hub_init(void)
{
  uint16_t pulse_width;
  uint16_t sample_val;
  sl_status_t ret_code;

  ret_code = bio_hub_init(sl_i2cspm_qwiic, 0);
  if (SL_STATUS_OK == ret_code) {
    app_log("Init success\r\n");
  } else {
    app_log("Init failed\r\n");
  }

  // Configuring just the BPM settings.
  if (SL_STATUS_OK == bio_hub_config_bpm(BIO_HUB_ALGO_MODE_ONE)) {
    app_log("Sensor configured.\r\n");
  } else {
    app_log("Error configuring sensor.\r\n");
  }

  // Set pulse width.
  if (SL_STATUS_OK == bio_hub_set_pulse_width(411)) {
    app_log("Pulse Width Set.\r\n");
  } else {
    app_log("Could not set Pulse Width.\r\n");
  }

  // Check that the pulse width was set.
  if (SL_STATUS_OK == bio_hub_read_pulse_width(&pulse_width)) {
    app_log("Pulse Width: %d\r\n", pulse_width);
  }

  // Set sample rate per second. Remember that not every sample rate is
  // available with every pulse width. Check hookup guide for more information.
  ret_code = bio_hub_set_sample_rate(400);
  if (SL_STATUS_OK == ret_code) {
    app_log("Sample Rate Set.\r\n");
  } else {
    app_log("Could not set Sample Rate!\r\n");
  }

  // Check sample rate.
  ret_code = bio_hub_read_sample_rate(&sample_val);
  if (SL_STATUS_OK == ret_code) {
    app_log("Sample rate is set to: %d\r\n", sample_val);
  }

  // Data lags a bit behind the sensor, if you're finger is on the sensor when
  // it's being configured this delay will give some time for the data to catch
  // up.
  app_log("Loading up the buffer with data....\r\n");
  sl_sleeptimer_delay_millisecond(4000);

  // starts a periodic timer to get data from sensor
  sl_simple_timer_start(&app_bio_hub_timer,
                        250,
                        app_bio_hub_timer_handle,
                        NULL,
                        true);
}

static void app_bio_hub_timer_handle(sl_simple_timer_t *timer, void *data)
{
  (void)timer;
  (void)data;

  // Information from the bio_hub_read_bpm function will be saved to our "body"
  // variable.
  if (SL_STATUS_OK == bio_hub_read_bpm(&libBpm)) {
    app_log("Heartrate: %d\r\n", libBpm.heart_rate);
    app_log("Confidence: %d\r\n", libBpm.confidence);
    app_log("Oxygen: %d\r\n", libBpm.oxygen);
    app_log("Status: %d\r\n", libBpm.status);
    // Slow it down or your heart rate will go up trying to keep up
    // with the flow of numbers
  } else {
    app_log("Read failed\r\n");
  }
}
