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
#include "sl_sleeptimer.h"
#include "sl_spidrv_instances.h"
#include "sl_status.h"
#include "app_log.h"
#include "app_assert.h"
#include "mikroe_cap1166.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t ret_code;

  ret_code = mikroe_cap1166_init(sl_spidrv_mikroe_handle);
  app_assert_status(ret_code);

  mikroe_cap1166_reset();

  sl_sleeptimer_delay_millisecond(200);

  mikroe_cap1166_default_cfg();
  sl_sleeptimer_delay_millisecond(100);

  app_log("\r\n>> Cap Touch 2 is initialized ");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t cnt;
  uint8_t sensor_results[6];
  sl_status_t ret_code;

  ret_code = mikroe_cap1166_detect_touch(&sensor_results[0]);
  if (ret_code != SL_STATUS_INVALID_PARAMETER) {
    for (cnt = 0; cnt < 6; cnt++ ) {
      if (sensor_results[cnt] == 1) {
        app_log("\r\nInput %d is touched", cnt + 1);
      } else if (sensor_results[cnt] == 2) {
        app_log("\r\nInput %d is released", cnt + 1);
      }
    }
  }
}
