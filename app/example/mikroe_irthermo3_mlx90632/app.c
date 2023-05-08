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

#include "mikroe_mlx90632.h"
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#include "sl_simple_timer.h"

static sl_simple_timer_t irthermo3_timer;

static void app_irthermo3_timer_handle(
  sl_simple_timer_t *timer, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  if (mikroe_mlx90632_init(sl_i2cspm_mikroe) == SL_STATUS_OK) {
    app_log("IrThermo 3 Click initializes successfully\n");
  }

  mikroe_mlx90632_default_config();
  sl_simple_timer_start(&irthermo3_timer,
                        3000,
                        app_irthermo3_timer_handle,
                        NULL,
                        true);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}

static void app_irthermo3_timer_handle(sl_simple_timer_t *timer, void *data)
{
  (void)timer;
  (void)data;

  if (mikroe_mlx90632_present() == SL_STATUS_OK) {
    app_log("Ambient temp: %.2f\n", mikroe_mlx90632_get_ambient_temperature());
    app_log("Object temp: %.2f\n", mikroe_mlx90632_get_object_temperature());
  } else {
    app_log("IrThermo 3 Click is not present on the bus\n");
  }
}
