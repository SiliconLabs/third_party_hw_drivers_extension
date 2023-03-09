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
#include "gpiointerrupt.h"
#include "sl_sleeptimer.h"
#include "app_log.h"

#include "mikroe_water_detect.h"
#include "mikroe_water_detect_config.h"

#define READING_INTERVAL_MSEC 200

#define MIKROE_WATER_DETECT_MODE_INTERRUPT
// #define MIKROE_WATER_DETECT_MODE_POLLING

static volatile uint8_t wd_state = 0;
static uint8_t wd_state_old = 0;

#ifdef MIKROE_WATER_DETECT_MODE_INTERRUPT
static void wd_int_callback(uint8_t intNo)
{
  (void) intNo;
  wd_state = mikroe_water_detect_get_status();
}

#endif
#ifdef MIKROE_WATER_DETECT_MODE_POLLING
static sl_sleeptimer_timer_handle_t wd_timer_handle;
static void wd_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;
  wd_state = mikroe_water_detect_get_status();
}

#endif

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  mikroe_water_detect_init();

#ifdef MIKROE_WATER_DETECT_MODE_POLLING
  sl_sleeptimer_start_periodic_timer(&wd_timer_handle,
                                     READING_INTERVAL_MSEC,
                                     wd_timer_callback,
                                     (void *) NULL,
                                     0,
                                     0);
#endif
#ifdef MIKROE_WATER_DETECT_MODE_INTERRUPT
  GPIO_ExtIntConfig(WATER_DETECT_INT_PORT,
                    WATER_DETECT_INT_PIN,
                    WATER_DETECT_INT_PIN,
                    1,
                    1,
                    1);
  GPIOINT_CallbackRegister(WATER_DETECT_INT_PIN, wd_int_callback);
  GPIO_IntEnable(WATER_DETECT_INT_PIN);
#endif
  app_log("  Initialization Driver   \r\n");
  app_log("------------------------- \r\n");
  app_log("  Wait to detect water... \r\n");
  app_log("------------------------- \r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (wd_state > wd_state_old) {
    app_log(" >  Water is detected  < \r\n");
    app_log("------------------------- \r\n");
    wd_state_old = 1;
  }

  if (wd_state < wd_state_old) {
    app_log(" There is no water \r\n");
    app_log("------------------- \r\n");
    wd_state_old = 0;
  }
}
