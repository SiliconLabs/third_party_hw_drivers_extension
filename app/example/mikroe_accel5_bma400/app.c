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
#include "sl_simple_timer.h"
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#include "app_assert.h"
#include "mikroe_bma400.h"

static sl_simple_timer_t bma400_timer;

static void app_bma400_timer_handle(sl_simple_timer_t *timer, void *data);
static sl_status_t app_bma400_init(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t ret;

  app_log("- BMA400 - Accel 5 Click board driver, example application. -\n");

  ret = app_bma400_init();
  app_assert_status(ret);

  app_log("> BMA400 - Accel 5 Click board driver is initialized successful.\n");
  app_log("> Starting measurement.\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}

static sl_status_t app_bma400_init(void)
{
  sl_status_t ret;

  ret = mikroe_bma400_init(sl_i2cspm_mikroe);
  if (SL_STATUS_OK != ret) {
    return ret;
  }

  mikroe_bma400_soft_reset();
  sl_sleeptimer_delay_millisecond(500);

  ret = mikroe_bma400_default_cfg(MIKROE_BMA400_CFG_0_NORMAL_MODE,
                                  MIKROE_BMA400_CFG_1_ACC_RANGE_4g);
  if (SL_STATUS_OK != ret) {
    return ret;
  }

  ret = sl_simple_timer_start(&bma400_timer,
                              200,
                              app_bma400_timer_handle,
                              NULL,
                              true);
  return ret;
}

static void app_bma400_timer_handle(sl_simple_timer_t *timer, void *data)
{
  (void)timer;
  (void)data;

  int16_t x_axis_data = 0;
  int16_t y_axis_data = 0;
  int16_t z_axis_data = 0;

  // Only get data if BMA400 is available on the bus
  if (SL_STATUS_OK != mikroe_bma400_present()) {
    return;
  }

  mikroe_bma400_get_axis(MIKROE_BMA400_X_AXIS, &x_axis_data);
  app_log(" X axis : %d\r\n", x_axis_data);

  mikroe_bma400_get_axis(MIKROE_BMA400_Y_AXIS, &y_axis_data);
  app_log(" Y axis : %d\r\n", y_axis_data);

  mikroe_bma400_get_axis(MIKROE_BMA400_Z_AXIS, &z_axis_data);
  app_log(" Z axis : %d\r\n\n", z_axis_data);
}
