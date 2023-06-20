/***************************************************************************//**
 * @file   nci_evt.h
 * @brief  NCI event type.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef __NCI_EVT_H__
#define __NCI_EVT_H__

/// Enum to hold all NCI event headers.
typedef enum {
  nci_evt_none,
  nci_evt_startup,
  nci_evt_data_packet_rec,

  /* NCI Core */
  nci_evt_core_reset_rsp_rec,
  nci_evt_core_reset_ntf_rec,
  nci_evt_core_init_rsp_rec,
  nci_evt_core_set_config_rsp_rec,
  nci_evt_core_get_config_rsp_rec,
  nci_evt_core_conn_create_rsp_rec,
  nci_evt_core_conn_close_rsp_rec,
  nci_evt_core_conn_credits_ntf_rec,
  nci_evt_core_generic_error_ntf_rec,
  nci_evt_core_interface_error_ntf_rec,

  /* RF Management */
  nci_evt_rf_discover_map_rsp_rec,
  nci_evt_rf_set_listen_mode_routing_rsp_rec,
  nci_evt_rf_get_listen_mode_routing_rsp_rec,
  nci_evt_rf_get_listen_mode_routing_ntf_rec,
  nci_evt_rf_discover_rsp_rec,
  nci_evt_rf_discover_ntf_rec,
  nci_evt_rf_discover_select_rsp_rec,
  nci_evt_rf_intf_activated_ntf_rec,
  nci_evt_rf_deactivate_rsp_rec,
  nci_evt_rf_deactivate_ntf_rec,
  nci_evt_rf_field_info_ntf_rec,
  nci_evt_rf_t3t_polling_rsp_rec,
  nci_evt_rf_t3t_polling_ntf_rec,
  nci_evt_rf_nfcee_action_ntf_rec,
  nci_evt_rf_nfcee_discovery_req_ntf_rec,
  nci_evt_rf_parameter_update_rsp_rec,

  /* NFCEE Management */
  nci_evt_nfcee_discover_rsp_rec,
  nci_evt_nfcee_discover_ntf_rec,
  nci_evt_nfcee_mode_set_rsp_rec,

  /* Proprietary */
  nci_evt_proprietary_nxp_act_rsp_rec
} nci_evt_header_t;

#endif
