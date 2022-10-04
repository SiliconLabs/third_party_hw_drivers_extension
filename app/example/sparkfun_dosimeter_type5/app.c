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
#include "sparkfun_type5.h"
#include "app_log.h"

static void noise_callback(void);
static void radiation_callback(void);

void app_init(void)
{
  sparkfun_type5_init();
  sparkfun_type5_register_noise_callback(noise_callback);
  sparkfun_type5_register_radiation_callback(radiation_callback);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sparkfun_type5_process();
}

/**************************************************************************//**
 *  Callback function for radiation detection.
 *****************************************************************************/
void radiation_callback(void)
{
  app_log("A wild gamma ray appeared\r\n");
  app_log("%2f uSv/h +/- %.2f\r\n",
          sparkfun_type5_get_usvh(),
          sparkfun_type5_get_usvh_error());
}

/**************************************************************************//**
 *  Callback function for noise detection.
 *****************************************************************************/
void noise_callback(void)
{
  app_log("Noise! Please stop moving\r\n");
}
