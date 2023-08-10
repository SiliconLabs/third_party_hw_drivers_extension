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
#include "mikroe_fsr400.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"

#define READING_INTERVAL_MSEC    500

typedef enum
{
  mikroe_fsr400_status_init = 0,
  mikroe_fsr400_status_light_touch,
  mikroe_fsr400_status_light_squeeze,
  mikroe_fsr400_status_medium_squeeze,
  mikroe_fsr400_status_big_squeeze,
}mikroe_fsr400_status_e;

uint8_t *mikroe_fsr400_status_name[] = {
  (uint8_t *)"mikroe_fsr400_status_release",
  (uint8_t *)"mikroe_fsr400_status_light_touch",
  (uint8_t *)"mikroe_fsr400_status_light_squeeze",
  (uint8_t *)"mikroe_fsr400_status_medium_squeeze",
  (uint8_t *)"mikroe_fsr400_status_big_squeeze",
};

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;
static mikroe_fsr400_status_e fsr400_status = mikroe_fsr400_status_init;
static mikroe_fsr400_status_e fsr400_old_status = mikroe_fsr400_status_init;

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t stt;

  app_log("Hello World - Mikroe Force 3 Click example\r\n");

  stt = mikroe_fsr400_init(sl_i2cspm_mikroe);
  app_log("mikroe_fsr400_init = 0x%x\r\n", (uint16_t)stt);

  app_log("sl_sleeptimer_start_periodic = 0x%lx\r\n",
          sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                                READING_INTERVAL_MSEC,
                                                app_timer_callback,
                                                NULL,
                                                0,
                                                0));

  app_log(">> %s \r\n", mikroe_fsr400_status_name[fsr400_status]);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (trigger_process) {
    trigger_process = false;
    uint16_t raw_data;

    mikroe_fsr400_get_raw_data(&raw_data);

    if (raw_data <= 5) {
      fsr400_status = mikroe_fsr400_status_init;
    } else if ((raw_data > 5) && (raw_data <= 200)) {
      fsr400_status = mikroe_fsr400_status_light_touch;
    } else if ((raw_data > 200) && (raw_data <= 500)) {
      fsr400_status = mikroe_fsr400_status_light_squeeze;
    } else if ((raw_data > 500) && (raw_data <= 800)) {
      fsr400_status = mikroe_fsr400_status_medium_squeeze;
    } else if (raw_data > 800) {
      fsr400_status = mikroe_fsr400_status_big_squeeze;
    }

    if (fsr400_status != fsr400_old_status) {
      fsr400_old_status = fsr400_status;
      app_log(">> %s \r\n", mikroe_fsr400_status_name[fsr400_status]);
      app_log("----------------------\r\n");
    }
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
