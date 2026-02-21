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
#include "tempsensor_lm75a.h"

#ifdef PLATFORM_SI917X
#include "rsi_debug.h"
#elif defined(PLATFORM_SERIES2)
#include "app_log.h"
#endif

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#ifdef PLATFORM_SI917X
  DEBUGINIT();
  DEBUGOUT("\n==== LM75 Temperature Demo (SI91x) ====\n");
#elif defined(PLATFORM_SERIES2)
  app_log("\n==== LM75 Temperature Demo (Series 2) ====\n");
#endif

  if (lm75_init() != SL_STATUS_OK) {
#ifdef PLATFORM_SI917X
    DEBUGOUT("LM75 Init Failed!\n");
#elif defined(PLATFORM_SERIES2)
    app_log("LM75 Init Failed!\n");
#endif
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  float tempC;

      if (lm75_get_temperature(&tempC) == SL_STATUS_OK) {

        float tempF = lm75_celsius_to_fahrenheit(tempC);

    #ifdef PLATFORM_SI917X
        DEBUGOUT("Temp: %.2f C | %.2f F\n", tempC, tempF);
    #elif defined(PLATFORM_SERIES2)
        app_log("Temp: %.2f C | %.2f F\n", tempC, tempF);
    #endif

      } else {

    #ifdef PLATFORM_SI917X
        DEBUGOUT("Temperature Read Failed!\n");
    #elif defined(PLATFORM_SERIES2)
        app_log("Temperature Read Failed!\n");
    #endif
      }
}
