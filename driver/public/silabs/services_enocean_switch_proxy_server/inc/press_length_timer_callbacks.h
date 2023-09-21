/***************************************************************************//**
 * @file press_length_timer_callbacks.h
 * @brief press_length_timer_callbacks header file.
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef PRESS_LENGTH_TIMER_CALLBACKS_H
#define PRESS_LENGTH_TIMER_CALLBACKS_H

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------
#include "app_timer.h"
#include "enocean_proxy.h"

// -----------------------------------------------------------------------------
//                               Public Function
// -----------------------------------------------------------------------------
void schedule_enocean_proxy_event(enocean_switch_t *enocean_switch,
                                  uint16_t time_delta_ms,
                                  void (*callback)(enocean_switch_t *));
void initial_long_press_timeout_expired(enocean_switch_t *enocean_switch);
void send_on_off(enocean_switch_t *enocean_switch);
void press_callback_base(app_timer_t *handle, void *data);
void increment_tid(void);

#endif /* PRESS_LENGTH_TIMER_CALLBACKS_H */
