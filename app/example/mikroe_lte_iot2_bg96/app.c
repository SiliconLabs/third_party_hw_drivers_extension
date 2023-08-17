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
#include "app_iostream_cli.h"
#include "at_parser_events.h"
#include "mikroe_bg96.h"

void app_init(void)
{
  app_log_iostream_set(sl_iostream_vcom_handle);
  app_log("Hello World LTE IoT 2 Click !!!\r\n");
  bg96_nb_init(sl_iostream_mikroe_handle);
  app_log("BG96 init done !!!\r\n");
  app_log("Type your command to execute the function !!!\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  app_iostream_cli_process_action();
  at_parser_process();
  at_platform_process();
  at_event_process();
}
