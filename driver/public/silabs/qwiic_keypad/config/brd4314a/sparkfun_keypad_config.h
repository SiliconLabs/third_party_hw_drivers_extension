/***************************************************************************//**
 * @file
 * @brief Sparkfun Qwiic Keypad Driver Configuration
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef SPARKFUN_KEYPAD_CONFIG_H
#define SPARKFUN_KEYPAD_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio> SPARKFUN_KEYPAD_NEW_EVENT_INTERRUPT
// $[GPIO_SPARKFUN_KEYPAD_NEW_EVENT_INTERRUPT]
#define SPARKFUN_KEYPAD_GPIO_INT_PORT gpioPortB
#define SPARKFUN_KEYPAD_GPIO_INT_PIN  3
// [GPIO_SPARKFUN_KEYPAD_NEW_EVENT_INTERRUPT]$

// <<< sl:end pin_tool >>>

#endif /*SPARKFUN_KEYPAD_CONFIG_H/