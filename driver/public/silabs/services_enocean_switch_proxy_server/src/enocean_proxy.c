/***************************************************************************//**
 * @file enocean_proxy.c
 * @brief enocean_proxy source file.
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
#include "enocean_proxy.h"
#include "press_length_timer_callbacks.h"
#include "psa_status_to_sl_status.h"
#include "app_assert.h"
#include "app_log.h"
#include "psa/crypto.h"
#include "psa_crypto_storage.h"
#include "sl_bt_api.h"
#include "sl_btmesh_api.h"
#include "sl_btmesh_dcd.h"

// -----------------------------------------------------------------------------
//                               Macros & Typedefs
// -----------------------------------------------------------------------------
#define AD_TYPE_MANUFACTURER_SPECIFIC_DATA       0xff
#define SILABS_VENDOR_ID                         0x02ff

#define ENOCEAN_PROXY_PS_KEY_NUM_STORED_SWITCHES 0x4065
#define ENOCEAN_PROXY_PS_KEY_SWITCHES_BEGIN      ( \
    ENOCEAN_PROXY_PS_KEY_NUM_STORED_SWITCHES + 1)
#define ENOCEAN_PROXY_SECURITY_KEY_SIZE          16
#define ENOCEAN_PROXY_SIGNATURE_LENGTH           4
#define ENOCEAN_PROXY_VENDOR_MODEL_OPCODE        9

typedef struct {
  bool button_b_down : 1;
  bool button_b_up : 1;
  bool button_a_down : 1;
  bool button_a_up : 1;
  bool is_press_action : 1;
} switch_action_t;

typedef struct {
  uint16_t elem_index;
  uint16_t vendor_id;
  uint16_t model_id;
} enocean_proxy_model_t;

enum enocean_proxy_subopcode {
  subopcode_bind_switch = 0x01,
  subopcode_bind_switch_status,
  subopcode_unbind_switch,
  subopcode_unbind_switch_status,
  subopcode_enter_commissioning_mode,
  subopcode_enter_commissioning_mode_status,
  subopcode_get_switch_id,
  subopcode_get_switch_id_status,
  subopcode_get_full_switch_data,
  subopcode_get_full_switch_data_status,
};

typedef uint8_t enocean_proxy_subopcode_t;

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
enocean_switch_t enocean_switches[MAX_NUM_SWITCHES] = { 0 };
static uint8_t commissioning_to_element_index = 0xff;
static bool is_provisioned = false;
static bool is_commissioning_mode_active = false;
static app_timer_t commisioning_mode_timer;
static app_timer_t long_press_timer;

SL_WEAK void enocean_proxy_num_switches_changed(uint8_t new_switch_count)
{
  (void)new_switch_count;
}

SL_WEAK void enocean_proxy_commissioning_mode_entered(void)
{
}

SL_WEAK void enocean_proxy_commissioning_mode_exited(void)
{
}

void print_switch_status(uint8_t switch_index, switch_action_t switch_action)
{
  app_log("SWITCH %u ", switch_index);
  app_log("%s ", switch_action.is_press_action ? "PRESS" : "RELEASE");
  if ((switch_action.button_a_up && switch_action.button_b_up)
      || (switch_action.button_a_down && switch_action.button_b_down)) {
    app_log("LEFT & RIGHT ");
  } else {
    app_log("%s ",
            (switch_action.button_a_up
             || switch_action.button_a_down) ? "LEFT" : "RIGHT");
  }
  app_log("BUTTON %s\n",
          (switch_action.button_a_up
           || switch_action.button_b_up) ? "UP" : "DOWN");
}

uint8_t get_num_switches(void)
{
  uint8_t num_bound_switches = 0;
  for (uint8_t i = 0; i < MAX_NUM_SWITCHES; i++) {
    if (enocean_switches[i].flags.is_bound) {
      num_bound_switches += 1;
    }
  }
  return num_bound_switches;
}

enocean_switch_t * get_switch_by_address(const uint8_t *address)
{
  for (uint8_t i = 0; i < MAX_NUM_SWITCHES; i++) {
    if (!enocean_switches[i].flags.is_bound) {
      continue;
    }
    bool is_matching = true;
    for (int j = 0; j < 6; j++) {
      if (address[j] != enocean_switches[i].address.addr[j]) {
        is_matching = false;
        break;
      }
    }
    if (is_matching) {
      return &enocean_switches[i];
    }
  }
  return NULL;
}

enocean_switch_t * get_switch_by_element_index(uint8_t element_index)
{
  for (uint8_t i = 0; i < MAX_NUM_SWITCHES; i++) {
    if (!enocean_switches[i].flags.is_bound) {
      continue;
    }
    if (enocean_switches[i].element_index == element_index) {
      return &enocean_switches[i];
    }
  }
  return NULL;
}

uint16_t enocean_proxy_get_lowest_unused_element_index(void)
{
  uint8_t *element_indices = get_all_switch_element_indices();
  for (int i = 0; i < MAX_NUM_SWITCHES; i++) {
    if (!get_switch_by_element_index(element_indices[i])) {
      return element_indices[i];
    }
  }
  return 0xffff;
}

struct enocean_switch_persistent_data {
  uint32_t sequence_counter;
  psa_key_id_t key_id;
  bd_addr address;
  uint8_t element_index;
};

sl_status_t store_state_in_persistent_storage(void)
{
  uint8_t count = 0;
  sl_status_t sc;
  for (int i = 0; i < MAX_NUM_SWITCHES; i++) {
    if (!enocean_switches[i].flags.is_bound) {
      continue;
    }
    struct enocean_switch_persistent_data persistent_data = {
      .sequence_counter = enocean_switches[i].sequence_counter,
      .key_id = enocean_switches[i].key_id,
      .address = enocean_switches[i].address,
      .element_index = enocean_switches[i].element_index
    };
    sc =
      sl_bt_nvm_save(ENOCEAN_PROXY_PS_KEY_SWITCHES_BEGIN + count,
                     sizeof(persistent_data),
                     (const uint8_t *)&persistent_data);
    if (sc) {
      app_log_status_error_f(sc, "sl_bt_nvm_save failed\n");
      return sc;
    }
    count += 1;
  }

  sc =
    sl_bt_nvm_save(ENOCEAN_PROXY_PS_KEY_NUM_STORED_SWITCHES, sizeof(count),
                   (const uint8_t *)&count);
  app_log_status_error_f(sc, "sl_bt_nvm_save failed\n");
  return sc;
}

sl_status_t load_state_from_persistent_storage(void)
{
  sl_status_t sc;

  uint8_t num_bound_switches;
  sc = sl_bt_nvm_load(ENOCEAN_PROXY_PS_KEY_NUM_STORED_SWITCHES,
                      sizeof(num_bound_switches),
                      NULL,
                      (uint8_t *)&num_bound_switches);
  if (sc) {
    if (sc == SL_STATUS_BT_PS_KEY_NOT_FOUND) {
      sc = SL_STATUS_OK;
    } else {
      app_log_status_f(sc, "sl_bt_nvm_load failed\n");
      return sc;
    }
  }
  app_assert(num_bound_switches <= MAX_NUM_SWITCHES,
             "Invalid switch_count value loaded\n");

  memset(enocean_switches, MAX_NUM_SWITCHES, sizeof(enocean_switch_t));
  struct enocean_switch_persistent_data persistent_data;
  for (int i = 0; i < num_bound_switches; i++) {
    sc = sl_bt_nvm_load(ENOCEAN_PROXY_PS_KEY_SWITCHES_BEGIN + i,
                        sizeof(persistent_data),
                        NULL,
                        (uint8_t *)&persistent_data);
    if (sc) {
      app_log_status_f(sc, "sl_bt_nvm_load failed\n");
      return sc;
    }
    if (persistent_data.element_index >= SL_BTMESH_CONFIG_MAX_ELEMENTS) {
      app_log("Invalid element index loaded\n");
      continue;
    }
    enocean_switches[i].address = persistent_data.address;
    enocean_switches[i].sequence_counter = persistent_data.sequence_counter;
    enocean_switches[i].key_id = persistent_data.key_id;
    enocean_switches[i].flags.is_bound = true;
    enocean_switches[i].element_index = persistent_data.element_index;
  }
  return sc;
}

sl_status_t remove_switch_by_element_index(uint8_t element_index)
{
  enocean_switch_t *enocean_switch = get_switch_by_element_index(element_index);
  if (!enocean_switch) {
    return SL_STATUS_NOT_FOUND;
  }
  enocean_switch->flags.is_bound = false;
  sl_status_t sc =
    psa_status_to_sl_status(psa_destroy_key(enocean_switch->key_id));
  app_log_status_error_f(sc, "psa_destroy_key failed\n");

  store_state_in_persistent_storage();
  enocean_proxy_num_switches_changed(get_num_switches());

  return SL_STATUS_OK;
}

sl_status_t import_key(const uint8_t *security_key,
                       uint8_t key_index,
                       psa_key_id_t *key_id)
{
  psa_key_attributes_t attributes = { 0 };
  psa_set_key_usage_flags(&attributes,
                          PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_EXPORT);
  psa_set_key_algorithm(&attributes,
                        PSA_ALG_AEAD_WITH_SHORTENED_TAG(PSA_ALG_CCM, 4));
  psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
  psa_set_key_id(&attributes, PSA_KEY_ID_USER_MIN + key_index);

  sl_status_t sc = psa_status_to_sl_status(
    psa_import_key(&attributes, security_key, ENOCEAN_PROXY_SECURITY_KEY_SIZE,
                   key_id));
  if (sc == SL_STATUS_ALREADY_EXISTS) {
    // Overwrite any previous key in persistent memory with the same id
    sc =
      psa_status_to_sl_status(psa_destroy_persistent_key(psa_get_key_id(
                                                           &attributes)));
    app_log_status_error_f(sc, "psa_destroy_persistent_key failed\n");
    sc = psa_status_to_sl_status(
      psa_import_key(&attributes, security_key, ENOCEAN_PROXY_SECURITY_KEY_SIZE,
                     key_id));
  }
  app_log_status_error_f(sc, "psa_import_key failed\n");
  return sc;
}

sl_status_t get_free_slot(uint8_t *index)
{
  for (uint8_t i = 0; i < MAX_NUM_SWITCHES; i++) {
    if (!enocean_switches[i].flags.is_bound) {
      *index = i;
      return SL_STATUS_OK;
    }
  }
  return SL_STATUS_NO_MORE_RESOURCE;
}

sl_status_t bind_switch(const uint8_t *address,
                        const uint8_t *security_key,
                        uint32_t sequence_counter,
                        uint8_t element_index)
{
  uint8_t free_index;
  if (get_free_slot(&free_index) == SL_STATUS_NO_MORE_RESOURCE) {
    return SL_STATUS_NO_MORE_RESOURCE;
  }
  if (get_switch_by_address(address)) {
    return SL_STATUS_ALREADY_EXISTS;
  }
  remove_switch_by_element_index(element_index);

  sl_status_t sc = SL_STATUS_OK;

  enocean_switch_t *new_switch = &enocean_switches[free_index];
  memcpy(new_switch->address.addr, address, sizeof(new_switch->address.addr));
  new_switch->sequence_counter = sequence_counter;
  new_switch->flags.is_bound = true;
  new_switch->flags.is_pressed = false;
  new_switch->flags.is_dimming_mode_active = false;
  new_switch->next_event_time = 0;
  new_switch->element_index = element_index;

  sc = import_key(security_key, free_index, &new_switch->key_id);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  app_log("NEW SWITCH ADDED TO ELEMENT %u, id: 0x", element_index);
  for (int i = 0; i < 6; i++) {
    app_log("%x", new_switch->address.addr[5 - i]);
  }
  app_log("\n");

  sc = store_state_in_persistent_storage();
  if (sc != SL_STATUS_OK) {
    psa_destroy_key(new_switch->key_id);
    return sc;
  }

  enocean_proxy_num_switches_changed(get_num_switches());
  return SL_STATUS_OK;
}

void exit_commissioning_mode(app_timer_t *handle, void *data)
{
  (void)handle;
  (void)data;
  is_commissioning_mode_active = false;
  enocean_proxy_commissioning_mode_exited();
}

sl_status_t commission_switch(const uint8_t *payload)
{
  sl_status_t sc =
    bind_switch(&payload[24],
                &payload[8],
                *(uint32_t *)&payload[4],
                commissioning_to_element_index);
  app_timer_stop(&commisioning_mode_timer);
  exit_commissioning_mode(NULL, NULL);
  if (sc == SL_STATUS_ALREADY_EXISTS) {
    sc = SL_STATUS_OK;
  }
  return sc;
}

switch_action_t parse_switch_action(uint8_t byte)
{
  switch_action_t switch_action;
  switch_action.button_b_down = (byte & (1 << 4)) ? 1 : 0;
  switch_action.button_b_up = (byte & (1 << 3)) ? 1 : 0;
  switch_action.button_a_down = (byte & (1 << 2)) ? 1 : 0;
  switch_action.button_a_up = (byte & (1 << 1)) ? 1 : 0;
  switch_action.is_press_action = byte & 1;
  return switch_action;
}

bool authenticate_data_telegram(uint32_t sequence_counter,
                                const uint8_t *source_address,
                                const uint8_t *input_data,
                                uint8_t input_data_length,
                                uint32_t signature,
                                psa_key_id_t key_id)
{
  uint8_t nonce[13] = { 0 };
  memcpy(nonce, source_address, 6);
  memcpy(&nonce[6], &sequence_counter, 4);

  uint8_t tag[ENOCEAN_PROXY_SIGNATURE_LENGTH];
  size_t tag_length;
  psa_status_t status = psa_aead_encrypt(key_id,
                                         PSA_ALG_AEAD_WITH_SHORTENED_TAG(
                                           PSA_ALG_CCM, 4),
                                         nonce,
                                         sizeof(nonce),
                                         input_data,
                                         input_data_length,
                                         NULL,
                                         0,
                                         tag,
                                         ENOCEAN_PROXY_SIGNATURE_LENGTH,
                                         &tag_length);
  if (status != PSA_SUCCESS) {
    app_log_error("psa_aead_encrypt failed: 0x%lx\n", status);
    return false;
  }
  return *(uint32_t *)&tag == signature;
}

sl_status_t start_timer_for_next_scheduled_event(void)
{
  uint32_t earliest_time = UINT32_MAX;
  enocean_switch_t *event_switch = NULL;

  for (uint8_t i = 0; i < MAX_NUM_SWITCHES; i++) {
    if (!enocean_switches[i].flags.is_bound) {
      continue;
    }
    if ((enocean_switches[i].next_event_time != 0)
        && (enocean_switches[i].next_event_time < earliest_time)) {
      app_assert(enocean_switches[i].next_event_callback != NULL,
                 "No callback set\n");
      earliest_time = enocean_switches[i].next_event_time;
      event_switch = &enocean_switches[i];
    }
  }
  if (earliest_time == UINT32_MAX) {
    // No events scheduled
    return SL_STATUS_OK;
  }
  uint32_t current_time = sl_sleeptimer_tick_to_ms(
    sl_sleeptimer_get_tick_count());
  uint32_t timer_delay =
    (earliest_time < current_time) ? 0 : earliest_time - current_time;
  sl_status_t sc = app_timer_start(&long_press_timer,
                                   timer_delay,
                                   press_callback_base,
                                   event_switch,
                                   false);
  app_log_status_error_f(sc, "Could not start timer\n");
  return sc;
}

void handle_switch_action(switch_action_t switch_action,
                          enocean_switch_t *enocean_switch)
{
  if (switch_action.is_press_action) {
    schedule_enocean_proxy_event(enocean_switch,
                                 400,
                                 initial_long_press_timeout_expired);
    enocean_switch->flags.is_pressed = true;
    enocean_switch->transmission_params.on_off =
      (switch_action.button_a_up || switch_action.button_b_up);
    increment_tid();
    start_timer_for_next_scheduled_event();
  } else { /* release action */
    if (!enocean_switch->flags.is_dimming_mode_active) {
      enocean_switch->transmission_params.retransmit_count = 0;
      schedule_enocean_proxy_event(enocean_switch, 0, send_on_off);
    }
    enocean_switch->flags.is_pressed = false;
  }
}

enum payload_type {
  DATA,
  COMMISSIONING,
  UNKNOWN
};

typedef uint8_t payload_type_t;

sl_status_t handle_advertisement(
  const sl_bt_evt_scanner_legacy_advertisement_report_t *advertisement_report)
{
  sl_status_t sc;
  const uint8_t *payload = &advertisement_report->data.data[0];
  uint8_t payload_length = advertisement_report->data.len;
  // uint8_t ad_field_length = payload[0];
  uint8_t ad_field_type = payload[1];

  if (payload_length < 4) {
    return SL_STATUS_OK;
  }
  uint16_t manufacturer_id = *(uint16_t *)&payload[2];
  if ((ad_field_type != AD_TYPE_MANUFACTURER_SPECIFIC_DATA)
      || (manufacturer_id != 0x03da)) {
    return SL_STATUS_OK;
  }

  payload_type_t payload_type = UNKNOWN;
  if ((13 <= payload_length) && (payload_length <= 17)) {
    payload_type = DATA;
  } else if ((payload_length == 30) || (payload_length == 29)) {
    payload_type = COMMISSIONING;
  }
  if (payload_type == UNKNOWN) {
    app_log("Unknown payload type\n");
    return SL_STATUS_INVALID_TYPE;
  }

  if ((payload_type == COMMISSIONING) && is_commissioning_mode_active) {
    sc = commission_switch(payload);
    return sc;
  }

  enocean_switch_t *enocean_switch = get_switch_by_address(
    advertisement_report->address.addr);
  if (!enocean_switch) {
    return SL_STATUS_OK;
  }

  uint32_t sequence_counter = *(uint32_t *)&payload[4];
  if (sequence_counter <= enocean_switch->sequence_counter) {
    return SL_STATUS_INVALID_COUNT;
  }
  enocean_switch->sequence_counter = sequence_counter;
  switch_action_t switch_action = parse_switch_action(payload[8]);
  uint8_t element_index = enocean_switch - &enocean_switches[0]
                          + ENOCEAN_PROXY_SWITCH_1;
  print_switch_status(element_index, switch_action);

  uint8_t authentication_input_length = payload_length
                                        - ENOCEAN_PROXY_SIGNATURE_LENGTH;
  if (!authenticate_data_telegram(sequence_counter,
                                  enocean_switch->address.addr,
                                  payload, authentication_input_length,
                                  *(uint32_t *)&payload[
                                    authentication_input_length],
                                  enocean_switch->key_id)) {
    app_log("Telegram authentication failed\n");
    return SL_STATUS_INVALID_SIGNATURE;
  }

  if (!is_provisioned) {
    app_log("Not provisioned\n");
    return SL_STATUS_INVALID_STATE;
  }

  handle_switch_action(switch_action, enocean_switch);
  return SL_STATUS_OK;
}

sl_status_t enocean_proxy_enter_commissioning_mode(uint8_t element_index)
{
  is_commissioning_mode_active = true;
  commissioning_to_element_index = element_index;
  sl_status_t sc = app_timer_start(&commisioning_mode_timer, 60000,
                                   exit_commissioning_mode, NULL, false);
  app_assert_status_f(sc, "enter_commissioning_mode: failed to start timer\n");
  app_log("Commissioning to element %u\n", element_index);
  enocean_proxy_commissioning_mode_entered();
  return sc;
}

void handle_vendor_model_receive(const sl_btmesh_msg_t *evt)
{
  sl_status_t sc;
  sl_btmesh_evt_vendor_model_receive_t *rx_event =
    (sl_btmesh_evt_vendor_model_receive_t *)&evt->data;
  uint8_t response[30];
  uint8_t *response_subopcode_ptr = &response[0];
  uint32_t *response_status_ptr = (uint32_t *)&response[1];
  size_t response_length = 0;

  if (rx_event->payload.len == 0) {
    *response_status_ptr = SL_STATUS_COMMAND_IS_INVALID;
    *response_subopcode_ptr = subopcode_bind_switch_status;
    response_length = 5;
  } else {
    switch (rx_event->payload.data[0]) {
      case subopcode_bind_switch:
        if (rx_event->payload.len < 27) {
          *response_status_ptr = SL_STATUS_COMMAND_IS_INVALID;
          *response_subopcode_ptr = subopcode_bind_switch_status;
          response_length = 5;
        }
        sc = bind_switch(&rx_event->payload.data[1],
                         &rx_event->payload.data[7],
                         *(uint32_t *)&rx_event->payload.data[22],
                         rx_event->elem_index);
        *response_subopcode_ptr = subopcode_bind_switch_status;
        *response_status_ptr = sc;
        response_length = 5;
        break;
      case subopcode_unbind_switch:
        sc = remove_switch_by_element_index(rx_event->elem_index);
        *response_subopcode_ptr = subopcode_unbind_switch_status;
        *response_status_ptr = sc;
        response_length = 5;
        break;
      case subopcode_enter_commissioning_mode:
        sc = enocean_proxy_enter_commissioning_mode(rx_event->elem_index);
        *response_subopcode_ptr = subopcode_enter_commissioning_mode_status;
        *response_status_ptr = sc;
        response_length = 5;
        break;
      case subopcode_get_switch_id: {
        *response_subopcode_ptr = subopcode_get_switch_id_status;
        enocean_switch_t *enocean_switch = get_switch_by_element_index(
          rx_event->elem_index);
        if (enocean_switch) {
          for (int i = 0; i < 6; i++) {
            // Addresses are stored in reverse order
            response[1 + i] = enocean_switch->address.addr[5 - i];
          }
          response_length = 7;
        } else {
          response[1] = 0x00;
          response_length = 2;
        }
        break;
      }
      case subopcode_get_full_switch_data:
        *response_subopcode_ptr = subopcode_get_full_switch_data_status;
        enocean_switch_t *enocean_switch = get_switch_by_element_index(
          rx_event->elem_index);
        if (!enocean_switch) {
          response[1] = 0x00;
          response_length = 2;
          break;
        }
        for (int i = 0; i < 6; i++) {
          // Addresses are stored in reverse order
          response[1 + i] = enocean_switch->address.addr[5 - i];
        }
        size_t data_length_out;
        sc =
          psa_status_to_sl_status(psa_export_key(enocean_switch->key_id,
                                                 &response[7], 16,
                                                 &data_length_out));
        app_log_status_error_f(sc, "psa_export_key failed\n");
        if (sc || (data_length_out != 16)) {
          response[1] = 0x00;
          response_length = 2;
          break;
        }
        uint32_t *response_sequence_counter_ptr = (uint32_t *)&response[23];
        *response_sequence_counter_ptr = enocean_switch->sequence_counter;
        response_length = sizeof(*response_subopcode_ptr)
                          + sizeof(enocean_switch->address.addr)
                          + data_length_out
                          + sizeof(enocean_switch->sequence_counter);
        // This is a segmented message: send only once
        sc = sl_btmesh_vendor_model_send(rx_event->source_address,
                                         rx_event->va_index,
                                         rx_event->appkey_index,
                                         rx_event->elem_index,
                                         SILABS_VENDOR_ID,
                                         ECOCEAN_SWITCH_PROXY_CONFIG_SERVER_MID,
                                         rx_event->nonrelayed,
                                         ENOCEAN_PROXY_VENDOR_MODEL_OPCODE,
                                         true,
                                         response_length,
                                         response);
        app_log_status_error_f(sc, "Vendor model: failed to send response\n");
        return;
      default:
        app_log("Unknown subopcode\n");
        return;
    }
  }
  // Send unsegmented messages multiple times
  for (int i = 0; i < 3; i++) {
    sc = sl_btmesh_vendor_model_send(rx_event->source_address,
                                     rx_event->va_index,
                                     rx_event->appkey_index,
                                     rx_event->elem_index,
                                     SILABS_VENDOR_ID,
                                     ECOCEAN_SWITCH_PROXY_CONFIG_SERVER_MID,
                                     rx_event->nonrelayed,
                                     ENOCEAN_PROXY_VENDOR_MODEL_OPCODE,
                                     true,
                                     response_length,
                                     response);
    app_log_status_error_f(sc, "Vendor model: failed to send response\n");
  }
}

sl_status_t initialize(bool is_already_provisioned)
{
  uint8_t opcode = ENOCEAN_PROXY_VENDOR_MODEL_OPCODE;
  sl_status_t sc;
  uint8_t *element_indices = get_all_switch_element_indices();
  for (int i = 0; i < MAX_NUM_SWITCHES; i++) {
    sc = sl_btmesh_vendor_model_init(element_indices[i],
                                     SILABS_VENDOR_ID,
                                     ECOCEAN_SWITCH_PROXY_CONFIG_SERVER_MID,
                                     1,
                                     1,
                                     &opcode);
    app_log_status_error_f(sc, "Failed to initialize vendor model\n");
  }
  sc = sl_btmesh_generic_client_init();
  app_log_status_error_f(sc, "Failed to generic client\n");
  sc = sl_sleeptimer_init();
  app_log_status_error_f(sc, "Failed to init sleeptimer\n");
  sc = psa_status_to_sl_status(psa_crypto_init());
  app_log_status_error_f(sc, "Failed to init crypto\n");

  if (is_already_provisioned) {
    is_provisioned = true;
    sc = load_state_from_persistent_storage();
    app_log_status_error_f(sc,
                           "Failed to load switch data from persistent storage\n");
  }
  enocean_proxy_num_switches_changed(get_num_switches());

  return sc;
}

void enocean_proxy_bt_on_event(sl_bt_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_scanner_legacy_advertisement_report_id: {
      handle_advertisement(&evt->data.evt_scanner_legacy_advertisement_report);
      break;
    }
  }
}

void enocean_proxy_btmesh_on_event(sl_btmesh_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_btmesh_evt_node_initialized_id:
      initialize(evt->data.evt_node_initialized.provisioned);
      break;
    case sl_btmesh_evt_node_provisioned_id:
      is_provisioned = true;
      break;
    case sl_btmesh_evt_vendor_model_receive_id:
      handle_vendor_model_receive(evt);
      break;
  }
}
