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
#include "app_log.h"
#include "mikroe_ds3231m.h"

#define READING_INTERVAL_MSEC 1000

static volatile bool flag = false;
static uint8_t sec_flag;
static sl_sleeptimer_timer_handle_t app_timer_handle;

/***************************************************************************//**
 * Display day of week function.
 ******************************************************************************/
static void display_day_of_the_week(uint8_t day_of_the_week)
{
  switch (day_of_the_week) {
    case 1:
      app_log("      Monday    \r\n\n ");
      break;
    case 2:
      app_log("      Tuesday   \r\n\n ");
      break;
    case 3:
      app_log("     Wednesday  \r\n\n ");
      break;
    case 4:
      app_log("     Thursday   \r\n\n ");
      break;
    case 5:
      app_log("      Friday    \r\n\n ");
      break;
    case 6:
      app_log("      Sunday    \r\n\n ");
      break;
    case 7:
      app_log("     Saturday   \r\n\n ");
      break;
    default:
      break;
  }
}

/***************************************************************************//**
 * Application sleeptimer callback function.
 ******************************************************************************/
static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  flag = true;
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_log("---- Application Init ----");

  //  Click initialization.
  mikroe_ds3231m_init(sl_i2cspm_mikroe);

  sl_sleeptimer_delay_millisecond(1000);

  sec_flag = 0xFF;

  app_log("------------------- \r\n");
  app_log("  Hardware  Reset   \r\n");
  mikroe_ds3231m_hw_reset();
  sl_sleeptimer_delay_millisecond(1000);

  // Set Time: 18h, 00 min and 00 sec
  mikroe_ds3231m_set_time(18, 0, 0);
  sl_sleeptimer_delay_millisecond(10);

  // Set Date: 6 ( Day of the week: Wednesday ), 6 ( day ), 12 ( month ) and
  //   2023 ( year )
  mikroe_ds3231m_set_date(3, 6, 12, 2023);
  sl_sleeptimer_delay_millisecond(100);

  app_log("-------------------  \r\n");
  app_log("  Enable Counting  \r\n");
  app_log("------------------- \r\n");
  app_log("     Start RTC      \r\n");
  app_log("------------------- \r\n");
  mikroe_ds3231m_enable_counting();
  sl_sleeptimer_delay_millisecond(100);
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (flag) {
    uint8_t time_hours = 0;
    uint8_t time_minutes = 0;
    uint8_t time_seconds = 0;

    uint8_t day_of_the_week = 0;
    uint8_t date_day = 0;
    uint8_t date_month = 0;
    uint8_t date_year = 0;

    float temperature;

    mikroe_ds3231m_get_time(&time_hours, &time_minutes, &time_seconds);
    sl_sleeptimer_delay_millisecond(100);

    mikroe_ds3231m_get_date(&day_of_the_week, &date_day, &date_month,
                            &date_year);
    sl_sleeptimer_delay_millisecond(100);

    if (sec_flag != time_seconds) {
      app_log(" \r\n\n Time: %02u:%02u:%02u  ",
              (uint16_t)time_hours,
              (uint16_t)time_minutes,
              (uint16_t)time_seconds);

      app_log("Date: %u. %u. 20%u. ",
              (uint16_t)date_day,
              (uint16_t)date_month,
              (uint16_t)date_year);
      display_day_of_the_week(day_of_the_week);

      if (time_seconds == 0) {
        temperature = mikroe_ds3231m_get_temperature();

        app_log("\r\n\n Temp: %.2f C", temperature);
      }
      app_log("--------------------------------------------");

      sec_flag = time_seconds;
    }
    flag = false;
  }
}
