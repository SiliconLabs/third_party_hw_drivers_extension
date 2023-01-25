
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
#include "mikroe_relay2.h"
#include "sl_sleeptimer.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  mikroe_relay2_init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  mikroe_relay2_relay1_control(0);
  mikroe_relay2_relay2_control(0);

  sl_sleeptimer_delay_millisecond(1000);

  mikroe_relay2_relay1_control(1);
  mikroe_relay2_relay2_control(1);

  sl_sleeptimer_delay_millisecond(1000);
}
