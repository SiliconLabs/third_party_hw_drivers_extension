/***************************************************************************//**
 * @file app.c
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

#include "sl_sleeptimer.h"
#include "sl_spidrv_instances.h"

#include "app_log.h"

#include "mikroe_mq131.h"

#define READING_INTERVAL_MSEC 1000

static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t app_timer;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *timer, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  app_log("---- Application Init ----\r\n");
  sc = mikroe_ozone2_init(sl_spidrv_mikroe_handle);
  if (sc != SL_STATUS_OK) {
    app_log("---- Application Init Error ----");
    app_log("---- Please, run program again ----");
    for ( ; ; ) {}
  }
  sl_sleeptimer_start_periodic_timer_ms(&app_timer,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint16_t o3_ppm;

  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  if (mikroe_ozone2_read_measurement(&o3_ppm) == SL_STATUS_OK) {
    app_log("  O3 [ppm] : %u\r\n", o3_ppm);
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
