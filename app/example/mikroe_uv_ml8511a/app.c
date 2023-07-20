/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "sl_spidrv_instances.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "mikroe_ml8511a.h"

#define READING_INTERVAL_MSEC 1000

static volatile bool uv_timer_expire = false;
static sl_sleeptimer_timer_handle_t uv_timer;

static void uv_timer_cb(sl_sleeptimer_timer_handle_t *timer, void *data);
static void application_callback(char *message);

void app_init(void)
{
  sl_status_t sc;

  app_log("---- Application Init ----\r\n");

  sc = mikroe_ml8511a_init(sl_spidrv_mikroe_handle);

  if (sc != SL_STATUS_OK) {
    app_log("---- Application Init Error ----");
    app_log("---- Please, run program again ----");
    for ( ; ; ) {}
  }
  mikroe_ml8511a_set_callback_handler(application_callback);
  mikroe_ml8511a_device_enable();
  sl_sleeptimer_delay_millisecond(1000);
  app_log("---- Application Init Done ----\r\n");
  sl_sleeptimer_start_periodic_timer_ms(&uv_timer,
                                        READING_INTERVAL_MSEC,
                                        uv_timer_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static float uv_voltage = 0;
  static uint8_t uv_index = 0;

  if (uv_timer_expire == false) {
    return;
  }
  uv_timer_expire = false;

  if (mikroe_ml8511a_read_adc_voltage(&uv_voltage) == SL_STATUS_OK) {
    mikroe_ml8511a_calc_index(uv_voltage, &uv_index);

    app_log(" UV Index [0-15] : %u\r\n", (uint16_t)uv_index);
    app_log(" UV ADC Voltage [V] : %.2f\r\n", uv_voltage);
    app_log("------------------------------\r\n");
  }
}

static void application_callback(char *message)
{
  app_log("*** %s ***\r\n", message);
}

static void uv_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  uv_timer_expire = true;
}
