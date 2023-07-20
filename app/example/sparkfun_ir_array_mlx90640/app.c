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

#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "app_assert.h"
#include "app_log.h"

#include "sparkfun_mlx90640.h"
#include "sparkfun_mlx90640_config.h"

static float mlx90640_image[SPARKFUN_MLX90640_NUM_OF_PIXELS];
static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
  uint16_t refrate = 0;
  sc = sparkfun_mlx90640_init(sl_i2cspm_qwiic,
                              MLX90640_I2C_ADDRESS);

  if (sc == SL_STATUS_OK) {
    app_log("\nMLX90640 initialized successfully\n");
    sc = sparkfun_mlx90640_set_refresh_rate(0x03);
    app_assert_status(sc);

    sc = sparkfun_mlx90640_get_refresh_rate(&refrate);
    app_assert_status(sc);

    app_log("RefreshRate: %x\n", refrate);

    sc = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                               250,
                                               app_timer_cb,
                                               (void *) NULL,
                                               0,
                                               0);
    app_assert_status(sc);
  } else {
    app_log("\nMLX90640 initialization failed!\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (app_timer_expire == true) {
    app_timer_expire = false;
    sparkfun_mlx90640_get_image_array(mlx90640_image);

    for (int i = 0; i < SPARKFUN_MLX90640_NUM_OF_PIXELS; i++)
    {
      app_log("%3.2f,", mlx90640_image[i]);
    }
    app_log("\n");
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
