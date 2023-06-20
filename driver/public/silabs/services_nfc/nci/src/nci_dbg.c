/***************************************************************************//**
 * @file   nci_dbg.c
 * @brief  NCI debug helper APIs.
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

#include "nci.h"

/**************************************************************************//**
 * @brief
 *  Log corresponding NCI event.
 *
 * @param[in] header
 *  NCI event header.
 *****************************************************************************/
void nci_evt_print(nci_evt_header_t header)
{
  switch (header) {
    case nci_evt_none:
      break;
    case nci_evt_startup:
      nci_log_ln("NCI event: start up. ");
      break;
    case nci_evt_data_packet_rec:
      nci_log_ln("NCI event: data packet received. ");
      break;

    /* NCI Core events. */
    case nci_evt_core_reset_rsp_rec:
      nci_log_ln("NCI event: core reset response received. ");
      break;
    case nci_evt_core_reset_ntf_rec:
      nci_log_ln("NCI event: core reset notification received. ");
      break;
    case nci_evt_core_init_rsp_rec:
      nci_log_ln("NCI event: core init response received. ");
      break;
    case nci_evt_core_set_config_rsp_rec:
      nci_log_ln("NCI event: core set config response received. ");
      break;
    case nci_evt_core_get_config_rsp_rec:
      nci_log_ln("NCI event: core get config response received. ");
      break;
    case nci_evt_core_conn_create_rsp_rec:
      nci_log_ln("NCI event: core conn create response received. ");
      break;
    case nci_evt_core_conn_close_rsp_rec:
      nci_log_ln("NCI event: core conn close response received. ");
      break;
    case nci_evt_core_conn_credits_ntf_rec:
      nci_log_ln("NCI event: core conn credits notification received. ");
      break;
    case nci_evt_core_generic_error_ntf_rec:
      nci_log_ln("NCI event: core generic error notification received. ");
      break;
    case nci_evt_core_interface_error_ntf_rec:
      nci_log_ln("NCI event: core interface error notification received. ");
      break;

    /* RF Management events. */
    case nci_evt_rf_discover_map_rsp_rec:
      nci_log_ln("NCI event: rf discover map response received. ");
      break;
    case nci_evt_rf_set_listen_mode_routing_rsp_rec:
      nci_log_ln("NCI event: rf set listen mode routing response received. ");
      break;
    case nci_evt_rf_get_listen_mode_routing_rsp_rec:
      nci_log_ln("NCI event: rf get listen mode routing response received. ");
      break;
    case nci_evt_rf_get_listen_mode_routing_ntf_rec:
      nci_log_ln("NCI event: rf get listen mode routing notification received. ");
      break;
    case nci_evt_rf_discover_rsp_rec:
      nci_log_ln("NCI event: rf discover response received. ");
      break;
    case nci_evt_rf_discover_ntf_rec:
      nci_log_ln("NCI event: rf discover notification received.. ");
      break;
    case nci_evt_rf_discover_select_rsp_rec:
      nci_log_ln("NCI event: rf discover select response received. ");
      break;
    case nci_evt_rf_intf_activated_ntf_rec:
      nci_log_ln("NCI event: rf interface activated notification received. ");
      break;
    case nci_evt_rf_deactivate_rsp_rec:
      nci_log_ln("NCI event: rf deactivate response received. ");
      break;
    case nci_evt_rf_deactivate_ntf_rec:
      nci_log_ln("NCI event: rf deactivate notification received. ");
      break;
    case nci_evt_rf_field_info_ntf_rec:
      nci_log_ln("NCI event: rf field info notification received. ");
      break;
    case nci_evt_rf_t3t_polling_rsp_rec:
      nci_log_ln("NCI event: rf t3t polling response received. ");
      break;
    case nci_evt_rf_t3t_polling_ntf_rec:
      nci_log_ln("NCI event: rf t3t polling notification received. ");
      break;
    case nci_evt_rf_nfcee_action_ntf_rec:
      nci_log_ln("NCI event: rf nfcee action notification received. ");
      break;
    case nci_evt_rf_nfcee_discovery_req_ntf_rec:
      nci_log_ln("NCI event: rf nfcee discovery req notification received. ");
      break;
    case nci_evt_rf_parameter_update_rsp_rec:
      nci_log_ln("NCI event: rf parameter update response received. ");
      break;

    /* NFCEE Management events. */
    case nci_evt_nfcee_discover_rsp_rec:
      nci_log_ln("NCI event: nfcee discover response received. ");
      break;
    case nci_evt_nfcee_discover_ntf_rec:
      nci_log_ln("NCI event: nfcee discover notification received. ");
      break;
    case nci_evt_nfcee_mode_set_rsp_rec:
      nci_log_ln("NCI event: nfcee mode set response received. ");
      break;

    /* Proprietary events. */
    case nci_evt_proprietary_nxp_act_rsp_rec:
      nci_log_ln("NCI event: NCI Proprietary NXP act response received. ");
      break;

    /* Event not recognized. */
    default:
      nci_log_ln("NCI event not recognized. ");
      break;
  }
}
