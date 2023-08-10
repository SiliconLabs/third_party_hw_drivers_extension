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
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "app_log.h"

#include "mikroe_mq7.h"

#define READING_INTERVAL_MSEC 3000

static volatile bool timer_is_expire = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  if (mikroe_mq7_init(IADC0) == SL_STATUS_OK) {
    app_log("Initialize CO sensor successfully\n");

    sl_sleeptimer_start_periodic_timer(&app_timer_handle,
                                       READING_INTERVAL_MSEC,
                                       app_timer_cb,
                                       (void *) NULL,
                                       0,
                                       0);
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  float value;
  uint16_t data_out;

  if (timer_is_expire == true) {
    timer_is_expire = false;

    if (mikroe_mq7_read_an_pin_voltage(&value) == SL_STATUS_OK) {
      app_log("AN Voltage: %f V\n", value);
    } else {
      app_log("Fail to read\n");
    }

    if (mikroe_mq7_read_an_pin_value(&data_out) == SL_STATUS_OK) {
      app_log("ADC Value: %d\n", data_out);
    } else {
      app_log("Fail to read\n");
    }
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  timer_is_expire = true;
}
