/***************************************************************************//**
 * @file
 * @brief cli baremetal examples functions
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

#ifndef CLI_BAREMETAL_H
#define CLI_BAREMETAL_H

#define SIGNAL_LOG_DATA    10

uint8_t get_device_count(void);
void get_device_mac(uint8_t index, uint8_t *mac);

/***************************************************************************//**
 * Initialize example
 ******************************************************************************/
void cli_app_init(void);

#endif // CLI_BAREMETAL_H
