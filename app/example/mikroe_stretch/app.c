/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "em_iadc.h"
#include "sl_status.h"
#include "app_log.h"
#include "sl_sleeptimer.h"
#include "mikroe_stretch.h"

static volatile bool stretch_timer_expire = false;
static sl_sleeptimer_timer_handle_t stretch_timer;

static void stretch_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_log("Application Init\n");

  if (mikroe_stretch_init(IADC0) != SL_STATUS_OK) {
    app_log("Application Init Error!\n");
    app_log("Please, run program again ...\n");

    for (;;) {}
  }
  app_log("Stretch Click is initialized.\n");
  sl_sleeptimer_start_periodic_timer_ms(&stretch_timer,
                                        500,
                                        stretch_sleeptimer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint16_t stretch_an_value = 0;

  if (stretch_timer_expire == true) {
    stretch_timer_expire = false;
    if (mikroe_stretch_generic_read(&stretch_an_value) == SL_STATUS_OK) {
      app_log("ADC Value: %u\r\n", stretch_an_value);
      if (stretch_an_value < 500) {
        mikroe_stretch_turn_on_led();
      } else {
        mikroe_stretch_turn_off_led();
      }
      app_log("Resistance Value: %u\r\n", stretch_an_value);
    }
  }
}

/***************************************************************************//**
 * Sleeptimer callback function.
 ******************************************************************************/
static void stretch_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data)
{
  (void)timer;
  (void)data;

  stretch_timer_expire = true;
}
