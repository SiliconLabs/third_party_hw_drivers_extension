/***************************************************************************//**
 * @file press_length_timer_callbacks.c
 * @brief press_length_timer_callbacks source file.
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

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------
#include "press_length_timer_callbacks.h"
#include "app_log.h"
#include "sl_btmesh_generic_model_capi_types.h"
#include "sl_btmesh_lib.h"

// -----------------------------------------------------------------------------
//                               Macros
// -----------------------------------------------------------------------------
#define TOTAL_RETRANSMISSION_COUNT                         3
#define MESSAGE_INTERVAL_MS                                50
#define DELTA                                              1260
#define LEVEL_CHANGE_TRANSITION_TIME_MS                    200

static uint8_t transaction_id = 0;

void increment_tid(void)
{
  transaction_id += 1;
}

void schedule_enocean_proxy_event(enocean_switch_t *enocean_switch,
                                  uint16_t time_delta_ms,
                                  void (*callback)(enocean_switch_t *))
{
  uint32_t tick_count = sl_sleeptimer_get_tick_count();
  uint32_t current_ms = sl_sleeptimer_tick_to_ms(tick_count);
  enocean_switch->next_event_time = current_ms + time_delta_ms;
  enocean_switch->next_event_callback = callback;
}

void press_callback_base(app_timer_t *handle, void *data)
{
  (void)handle;
  enocean_switch_t *enocean_switch = (enocean_switch_t *)data;
  enocean_switch->next_event_time = 0;
  enocean_switch->next_event_callback(enocean_switch);
  start_timer_for_next_scheduled_event();
}

void keep_adjusting_level(enocean_switch_t *enocean_switch)
{
  if (!enocean_switch->flags.is_pressed) {
    enocean_switch->flags.is_dimming_mode_active = false;
    return;
  }
  schedule_enocean_proxy_event(enocean_switch, 100, keep_adjusting_level);

  struct mesh_generic_request request;
  request.kind = mesh_generic_request_level_delta;

  // Increase/decrease the delta on every transmission
  int16_t delta_change =
    (enocean_switch->transmission_params.on_off) ? DELTA : -DELTA;
  request.delta = enocean_switch->transmission_params.previous_delta
                  + delta_change;
  enocean_switch->transmission_params.previous_delta = request.delta;
  sl_status_t sc = mesh_lib_generic_client_publish(
    MESH_GENERIC_LEVEL_CLIENT_MODEL_ID,
    enocean_switch->element_index,
    transaction_id,
    &request,
    LEVEL_CHANGE_TRANSITION_TIME_MS,
    0,
    0);
  app_log_status_f(sc, "Level client publish failed\n");
}

void start_adjusting_level(enocean_switch_t *enocean_switch)
{
  enocean_switch->transmission_params.retransmit_count += 1;
  if (enocean_switch->transmission_params.retransmit_count
      < TOTAL_RETRANSMISSION_COUNT) {
    schedule_enocean_proxy_event(enocean_switch,
                                 MESSAGE_INTERVAL_MS,
                                 start_adjusting_level);
  }
  uint16_t delay =
    (TOTAL_RETRANSMISSION_COUNT
     - enocean_switch->transmission_params.retransmit_count)
    * MESSAGE_INTERVAL_MS;

  struct mesh_generic_request request;
  request.kind = mesh_generic_request_level_delta;
  request.delta = (enocean_switch->transmission_params.on_off) ? DELTA : -DELTA;

  sl_status_t sc = mesh_lib_generic_client_publish(
    MESH_GENERIC_LEVEL_CLIENT_MODEL_ID,
    enocean_switch->element_index,
    transaction_id,
    &request,
    LEVEL_CHANGE_TRANSITION_TIME_MS,
    delay,
    0);
  app_log_status_f(sc, "Level client publish failed\n");

  if (enocean_switch->transmission_params.retransmit_count
      >= TOTAL_RETRANSMISSION_COUNT) {
    if (enocean_switch->flags.is_pressed) {
      enocean_switch->transmission_params.previous_delta = request.delta;
      keep_adjusting_level(enocean_switch);
    } else {
      enocean_switch->flags.is_dimming_mode_active = false;
    }
  }
}

void initial_long_press_timeout_expired(enocean_switch_t *enocean_switch)
{
  if (enocean_switch->flags.is_pressed) {
    enocean_switch->flags.is_dimming_mode_active = true;
    enocean_switch->transmission_params.retransmit_count = 0;
    start_adjusting_level(enocean_switch);
  }
}

void send_on_off(enocean_switch_t *enocean_switch)
{
  enocean_switch->transmission_params.retransmit_count += 1;
  if (enocean_switch->transmission_params.retransmit_count
      < TOTAL_RETRANSMISSION_COUNT) {
    schedule_enocean_proxy_event(enocean_switch,
                                 MESSAGE_INTERVAL_MS,
                                 send_on_off);
  }

  uint16_t delay =
    (TOTAL_RETRANSMISSION_COUNT
     - enocean_switch->transmission_params.retransmit_count)
    * MESSAGE_INTERVAL_MS;

  struct mesh_generic_request request;
  request.kind = mesh_generic_request_on_off;
  request.on_off =
    (enocean_switch->transmission_params.on_off) ? MESH_GENERIC_ON_OFF_STATE_ON
    :
    MESH_GENERIC_ON_OFF_STATE_OFF;

  sl_status_t sc = mesh_lib_generic_client_publish(
    MESH_GENERIC_ON_OFF_CLIENT_MODEL_ID, enocean_switch->element_index,
    transaction_id, &request, 0, delay, 0);
  app_log_status_f(sc, "On/Off Client Publish failed\n");
}
