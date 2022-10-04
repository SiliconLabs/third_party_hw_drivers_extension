/***************************************************************************//**
 * @file app.c
 * @brief Example application
 * @version 1.0.0
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

#include "sl_sleeptimer.h"
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#include "mikroe_shtc3.h"

static bool sensor_init_ok = false;

void app_init(void)
{
  app_log(
    "-- SHTC3 - Temp&Hum 9 Click board driver, example application. --\n");

  if (SL_STATUS_OK != mikroe_shtc3_init(sl_i2cspm_mikroe)) {
    app_log("> SHTC3 - Temp&Hum 9 Click board driver init failed.\n");
  } else {
    sensor_init_ok = true;
  }

  if (sensor_init_ok) {
    mikroe_shtc3_send_command(MIKROE_SHTC3_CMD_SLEEP);
    sl_sleeptimer_delay_millisecond(500);
    mikroe_shtc3_send_command(MIKROE_SHTC3_CMD_WAKEUP);
    sl_sleeptimer_delay_millisecond(100);

    app_log("> App init done.\n");
    app_log("> Starting periodic measurement.\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  mikroe_shtc3_measurement_data_t measurement_data;

  if (sensor_init_ok) {
    mikroe_shtc3_get_temperature_and_humidity(SHTC3_DATA_MODE_NORMAL,
                                              &measurement_data);

    app_log(">> Temp: %.2f °C RH: %.2f %%\n", measurement_data.temperature,
            measurement_data.humidity);
    sl_sleeptimer_delay_millisecond(1000);
  }
}
