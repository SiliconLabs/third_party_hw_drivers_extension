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
#include "app_log.h"
#include "app_assert.h"
#include "sl_sleeptimer.h"
#include "mikroe_lb11685av.h"

void app_init(void)
{
  sl_status_t ret;

  ret = mikroe_lb11685av_init();
  app_assert(ret == SL_STATUS_OK, "\rMikroe LB11685AV init fail\n");
  app_log("\rMikroe LB11685AV init done");

  sl_sleeptimer_delay_millisecond(500);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static uint16_t timer = 5000;
  static uint8_t state = 1;

  if (mikroe_lb11685av_get_rd()) {
    app_log("\rMotor lock\n");
    sl_sleeptimer_delay_millisecond(500);
  }
  if (mikroe_lb11685av_get_fg()) {
    app_log("\rFG\n");
    sl_sleeptimer_delay_millisecond(500);
  }
  if (!(timer--)) {
    timer = 5000;

    if (state) {
      app_log("\rMotor stop\n");
    } else {
      app_log("\rMotor rotating\n");
    }
    mikroe_lb11685av_set_en(state);
    state = !state;
  }
  sl_sleeptimer_delay_millisecond(1);
}
