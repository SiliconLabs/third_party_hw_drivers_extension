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

#include "app.h"

#include "sparkfun_weather_station_rainfall.h"
#include "sparkfun_weather_station_wind_direction.h"
#include "sparkfun_weather_station_wind_speed.h"

#include "printf.h"

#include "sl_sleeptimer.h"
#include "sl_status.h"

#define APP_MEASUREMENT_PERIOD_MS 500

static volatile bool measurement_trigger = false;
static sl_sleeptimer_timer_handle_t sparkfun_weatherstation_read_handle;
static void sparkfun_weatherstation_read_callback(
  sl_sleeptimer_timer_handle_t *handle,
  void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t status = SL_STATUS_FAIL;

  sparkfun_weatherstation_rainfall_init();
  status = sparkfun_weatherstation_winddirection_init();
  sparkfun_weatherstation_windspeed_init();
  if (status == SL_STATUS_OK) {
    sl_sleeptimer_start_periodic_timer_ms(&sparkfun_weatherstation_read_handle,
                                          APP_MEASUREMENT_PERIOD_MS,
                                          sparkfun_weatherstation_read_callback,
                                          (void *)NULL,
                                          0,
                                          0);
  } else {
    printf("Error initializing wind direction sensor\n");
  }
}

void sparkfun_measurement()
{
  float rainfall;
  float winddirection;
  float windspeed;

  sl_status_t status = SL_STATUS_FAIL;

  sparkfun_weatherstation_rainfall_get_rainfall_amount(&rainfall);
  sparkfun_weatherstation_winddirection_read_direction(&winddirection);
  status = sparkfun_weatherstation_windspeed_get(&windspeed);
  if (status != SL_STATUS_OK) {
    printf("Error reading wind speed sensor\n");
  }
  printf("rain: %fmm - wind direction: %f degrees - wind speed: %fkm/h\n",
         rainfall,
         winddirection,
         windspeed);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (measurement_trigger) {
    measurement_trigger = false;
    sparkfun_measurement();
  }
}

static void sparkfun_weatherstation_read_callback(
  sl_sleeptimer_timer_handle_t *handle,
  void *data)
{
  (void) handle;
  (void) data;

  measurement_trigger = true;
}
