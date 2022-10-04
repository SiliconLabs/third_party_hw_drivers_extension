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

#include "sl_sleeptimer.h"
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#include "mikroe_maxm86161.h"

bool sensor_init_ok = false;
static uint16_t counter = 1000;

void app_init(void)
{
  uint8_t rd_stat;
  app_log(
    "- MAXM86161 - Heartrate 2 Click board driver, example application. -\n");

  if (SL_STATUS_OK != mikroe_maxm86161_init(sl_i2cspm_mikroe)) {
    app_log("> MAXM86161 - Heartrate 2 Click board driver init failed.\n");
  } else {
    sensor_init_ok = true;
  }

  if (sensor_init_ok) {
    sl_sleeptimer_delay_millisecond(1000);

    mikroe_maxm86161_set_en(MAXM86161_PIN_HIGH);
    sl_sleeptimer_delay_millisecond(100);
    mikroe_maxm86161_soft_reset();
    rd_stat = mikroe_maxm86161_generic_read(MIKROE_MAXM86161_REG_PART_ID);

    if (rd_stat != MIKROE_MAXM86161_DEV_ID) {
      app_log("---- WRONG ID ----\n");
      app_log("Please restart your system.\n");
      for ( ; ; ) {}
    }

    if (mikroe_maxm86161_default_cfg(MAXM86161_CONFIG_GREEN) == SL_STATUS_OK) {
      app_log("The module has been configured!\n");
    }
    sl_sleeptimer_delay_millisecond(1000);

    app_log("> App init done.\n");
    app_log("> Starting measurement.\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  mikroe_maxm86161_fifo_data_t fifo_object;

  mikroe_maxm86161_read_fifo(&fifo_object);

  if (fifo_object.tag == MIKROE_MAXM86161_FIFO_TAG_PPG1_LEDC1) {
    counter++;
    if (fifo_object.data_val > 1000) {
      app_log("%lu;\r\n", fifo_object.data_val);
      counter = 1000;
    } else if (counter > 1000) {
      app_log("Please place your index finger on the sensor.\r\n");
      counter = 0;
    }
  }
}
