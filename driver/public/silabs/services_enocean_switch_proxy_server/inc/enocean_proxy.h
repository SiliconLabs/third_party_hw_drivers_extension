/***************************************************************************//**
 * @file enocean_proxy.h
 * @brief enocean_proxy header file.
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

#ifndef ENOCEAN_PROXY_H
#define ENOCEAN_PROXY_H

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------
#include "enocean_proxy_template.h"
#include "psa/crypto.h"
#include "sl_bt_api.h"
#include "sl_btmesh_api.h"

// -----------------------------------------------------------------------------
//                               Typedefs
// -----------------------------------------------------------------------------
typedef struct {
  bool is_dimming_mode_active : 1;
  bool is_bound : 1;
  bool is_pressed : 1;
} enocean_switch_flags_t;

typedef struct {
  int32_t previous_delta;
  uint8_t retransmit_count;
  bool on_off;
} enocean_switch_transmission_params_t;

typedef struct enocean_switch {
  bd_addr address;
  uint8_t element_index;
  enocean_switch_flags_t flags;
  uint32_t sequence_counter;
  psa_key_id_t key_id;
  uint32_t next_event_time;
  void (*next_event_callback)(struct enocean_switch *);
  enocean_switch_transmission_params_t transmission_params;
} enocean_switch_t;

// -----------------------------------------------------------------------------
//                               Public Function
// -----------------------------------------------------------------------------
void enocean_proxy_bt_on_event(sl_bt_msg_t *evt);
void enocean_proxy_btmesh_on_event(sl_btmesh_msg_t *evt);
sl_status_t start_timer_for_next_scheduled_event(void);

#endif /* ENOCEAN_PROXY_H */
