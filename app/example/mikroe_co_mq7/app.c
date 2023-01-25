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
#include "sl_simple_timer.h"
#include "mikroe_mq7.h"
#include "app_log.h"

static float value;
static uint16_t data_out;
static sl_simple_timer_t co_timer;

static void app_co_timer_handle(
  sl_simple_timer_t *timer, void *data);

void app_init(void)
{
  if (mikroe_mq7_init(IADC0) == SL_STATUS_OK) {
    app_log("Initialize CO sensor successfully\n");
  }

  sl_simple_timer_start(&co_timer, 3000, app_co_timer_handle, NULL, true);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}

static void app_co_timer_handle(sl_simple_timer_t *timer, void *data)
{
  (void)timer;
  (void)data;
  if (mikroe_mq7_read_an_pin_voltage(&value) == SL_STATUS_OK) {
    app_log("Value is: %f\n", value);
  } else {
    app_log("Fail to read\n");
  }

  if (mikroe_mq7_read_an_pin_value(&data_out) == SL_STATUS_OK) {
    app_log("Value is: %d\n", data_out);
  } else {
    app_log("Fail to read\n");
  }
}
