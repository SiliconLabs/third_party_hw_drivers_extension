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

#include "sl_status.h"
#include "app_log.h"
#include "app_assert.h"
#include "mikroe_alcohol_mq3.h"
#include "em_iadc.h"
#include "sl_sleeptimer.h"
#include "alcohol.h"

static sl_sleeptimer_timer_handle_t alcohol_timer;

static void application_task(void);
static void alcohol_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_log("Application Init\n");
  mikroe_mq3_setup();

  if (mikroe_mq3_init(IADC0) != SL_STATUS_OK) {
    app_log("Application Init Error!\n");
    app_log("Please, run program again ...\n");

    for (;;) {}
  }
  app_log("Alcohol Click is initialized.\n");
  sl_sleeptimer_delay_millisecond(100);
  sl_sleeptimer_start_periodic_timer_ms(&alcohol_timer,
                                        1000,
                                        alcohol_sleeptimer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
static void application_task(void)
{
  uint16_t alcohol_an_value = 0;

  if (mikroe_mq3_read_an_pin_value(&alcohol_an_value) == SL_STATUS_OK) {
    app_log("ADC Value: %u\r\n", alcohol_an_value);
  }

  float alcohol_an_voltage = 0;
  if (mikroe_mq3_read_an_pin_voltage(&alcohol_an_voltage) == SL_STATUS_OK) {
    app_log("AN Voltage: %.3f[V]\r\n", alcohol_an_voltage);
  }
}

/***************************************************************************//**
 * Sleeptimer callback function.
 ******************************************************************************/
static void alcohol_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data)
{
  (void)timer;
  (void)data;
  application_task();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
