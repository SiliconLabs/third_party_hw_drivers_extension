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
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#include "mikroe_bma400.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_log(
    "- BMA400 - Accel 5 Click board driver, example application. -\n");
  if (SL_STATUS_OK != mikroe_bma400_init(sl_i2cspm_mikroe)) {
    app_log("> BMA400 - Accel 5 Click board driver init failed.\n");
  }
  mikroe_bma400_soft_reset();
  sl_sleeptimer_delay_millisecond(500);

  if (SL_STATUS_OK
      == mikroe_bma400_default_cfg(MIKROE_BMA400_CFG_0_NORMAL_MODE,
                                   MIKROE_BMA400_CFG_1_ACC_RANGE_4g)) {
    app_log("The module has been configured!\n");
  }
  sl_sleeptimer_delay_millisecond(500);
  app_log("> App init done.\n");
  app_log("> Starting measurement.\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  int16_t x_axis_data = 0;
  int16_t y_axis_data = 0;
  int16_t z_axis_data = 0;

  mikroe_bma400_get_axis(MIKROE_BMA400_X_AXIS, &x_axis_data);
  app_log(" X axis : %d\r\n", x_axis_data);

  mikroe_bma400_get_axis(MIKROE_BMA400_Y_AXIS, &y_axis_data);
  app_log(" Y axis : %d\r\n", y_axis_data);

  mikroe_bma400_get_axis(MIKROE_BMA400_Z_AXIS, &z_axis_data);
  app_log(" Z axis : %d\r\n\n", z_axis_data);
  sl_sleeptimer_delay_millisecond(100);
}
