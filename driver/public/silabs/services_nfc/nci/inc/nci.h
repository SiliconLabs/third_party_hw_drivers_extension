/***************************************************************************//**
 * @file   nci.h
 * @brief  Types, definitions and APIs for NCI.
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

#ifndef __NCI_H__
#define __NCI_H__

/* NCI files */
#include <stdint.h>
#include <stdbool.h>
#include "nci_dbg.h"
#include "nci_err.h"
#include "nci_evt.h"

/* Proprietary files */
#include "nci_nxp_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup NFC Library
 * @brief Types, definitions and APIs for NCI.
 * @{
 ******************************************************************************/

/* NCI 1.0 spec table 6 */
#define NCI_VERSION                                         0x10

#define NCI_PACKET_MT_INDEX                                 (0)
#define NCI_PACKET_PBF_INDEX                                (0)
#define NCI_PACKET_GID_INDEX                                (0)
#define NCI_PACKET_CONN_ID_INDEX                            (0)
#define NCI_PACKET_OID_INDEX                                (1)
#define NCI_PACKET_PAYLOAD_LEN_INDEX                        (2)
#define NCI_PACKET_PAYLOAD_START_INDEX                      (3)

#define NCI_PACKET_MT_M                                     (0xE0)
#define NCI_PACKET_MT_SHIFT                                 (5)
#define NCI_PACKET_PBF_M                                    (0x10)
#define NCI_PACKET_PBF_SHIFT                                (4)
#define NCI_PACKET_GID_M                                    (0x0F)
#define NCI_PACKET_GID_SHIFT                                (0)
#define NCI_PACKET_CONN_ID_M                                (0x0F)
#define NCI_PACKET_CONN_ID_SHIFT                            (0)
#define NCI_PACKET_OID_M                                    (0x3F)
#define NCI_PACKET_OID_SHIFT                                (0)

#define NCI_NFCEE_ID_DH_NFCEE                               (0)

#define NCI_RF_LMR_POWER_STATE_BATTERY_OFF_M                (0x04)
#define NCI_RF_LMR_POWER_STATE_SWITCHED_OFF_M               (0x02)
#define NCI_RF_LMR_POWER_STATE_SWITCHED_ON_M                (0x01)

/**
 * nci_rf_disc_tech_param_nfc_a_poll_t
 * ----------
 * @brief Struct to hold fields for NFC-A Poll Mode Parameters.
 *        See NCI 1.0 spec table 54.
 */
typedef struct {
  uint8_t *sens_res;
  uint8_t  nfcid1_len;
  uint8_t *nfcid1;
  uint8_t  sel_res_len;
  uint8_t  sel_res;
} nci_rf_disc_tech_param_nfc_a_poll_t;

/**
 * nci_rf_disc_tech_param_nfc_b_poll_t
 * ----------
 * @brief Struct to hold fields for NFC-B Poll Mode Parameters.
 *        See NCI 1.0 spec table 56.
 */
typedef struct {
  uint8_t  sensb_res_len;
  uint8_t *sensb_res;
} nci_rf_disc_tech_param_nfc_b_poll_t;

/**
 * nci_rf_disc_nfc_f_param_bit_rate_t
 * ----------
 * @brief Enum to hold values for NFC-F bit rate parameter.
 *        See NCI 1.0 spec table 58.
 */
typedef enum {
  bit_rate_212_kbps                                       = 0x00,
  bit_rate_424_kbps                                       = 0x01
} nci_rf_disc_nfc_f_param_bit_rate_t;

/**
 * nci_rf_disc_tech_param_nfc_f_poll_t
 * ----------
 * @brief Struct to hold fields for NFC-F Poll Mode Parameters.
 *        See NCI 1.0 spec table 58.
 */
typedef struct {
  nci_rf_disc_nfc_f_param_bit_rate_t  bit_rate;
  uint8_t                             sensf_res_len;
  uint8_t *sensf_res;
} nci_rf_disc_tech_param_nfc_f_poll_t;

typedef union {
  nci_rf_disc_tech_param_nfc_a_poll_t nfc_a_poll_params;
  nci_rf_disc_tech_param_nfc_b_poll_t nfc_b_poll_params;
  nci_rf_disc_tech_param_nfc_f_poll_t nfc_f_poll_params;
} nci_rf_discover_ntf_rf_tech_param_t;

/**
 * nci_status_code
 * ----------
 * @brief Enum to hold values for NCI Status Codes.
 *        See NCI 1.0 spec table 94.
 */
typedef enum {
  /* Generic Status Codes */
  nci_status_ok                                           = 0x00,
  nci_status_rejected                                     = 0x01,
  nci_status_rf_frame_corrupted                           = 0x02,
  nci_status_failed                                       = 0x03,
  nci_status_not_initialized                              = 0x04,
  nci_status_syntax_error                                 = 0x05,
  nci_status_semantic_error                               = 0x06,
  nci_status_invalid_param                                = 0x09,
  nci_status_message_size_exceeded                        = 0x0A,

  /* RF Discovery Specific Status Codes */
  nci_discovery_already_started                           = 0xA0,
  nci_discovery_target_activation_failed                  = 0xA1,
  nci_discovery_discovery_tear_down                       = 0xA2,

  /* RF Interface Specific Status Codes */
  nci_rf_transmission_error                               = 0xB0,
  nci_rf_protocol_error                                   = 0xB1,
  nci_rf_timeout_error                                    = 0xB2,

  /* NFCEE Interface Specific Status Codes */
  nci_nfcee_interface_activation_failed                   = 0xC0,
  nci_nfcee_transmission_error                            = 0xC1,
  nci_nfcee_protocol_error                                = 0xC2,
  nci_nfcee_timeour_error                                 = 0xC3
} nci_status_code_t;

/**
 * nci_nfc_rf_technology_t
 * ----------
 * @brief Enum to hold values for NCI RF Technologies.
 *        See NCI 1.0 spec table 95.
 */
typedef enum {
  nci_nfc_rf_technology_a                                 = 0x00,
  nci_nfc_rf_technology_b                                 = 0x01,
  nci_nfc_rf_technology_f                                 = 0x02,
  nci_nfc_rf_technology_15693                             = 0x03
} nci_nfc_rf_technology_t;

/**
 * nci_nfc_rf_tech_and_mode_t
 * ----------
 * @brief Enum to hold values for NCI RF Technology and Mode.
 *        See NCI 1.0 spec table 96.
 */
typedef enum {
  nci_nfc_a_passive_poll_mode                             = 0x00,
  nci_nfc_b_passive_poll_mode                             = 0x01,
  nci_nfc_f_passive_poll_mode                             = 0x02,
  nci_nfc_a_passive_listen_mode                           = 0x80,
  nci_nfc_b_passive_listen_mode                           = 0x81,
  nci_nfc_f_passive_listen_mode                           = 0x82,
} nci_nfc_rf_tech_and_mode_t;

/**
 * nci_nfc_bit_rate_t
 * ----------
 * @brief Enum to hold values for all non proprietary NFC Bit Rates.
 *        See NCI 1.0 spec table 97.
 */
typedef enum {
  nci_nfc_bit_rate_106                                    = 0x00,
  nci_nfc_bit_rate_212                                    = 0x01,
  nci_nfc_bit_rate_424                                    = 0x02,
  nci_nfc_bit_rate_848                                    = 0x03,
  nci_nfc_bit_rate_1695                                   = 0x04,
  nci_nfc_bit_rate_3390                                   = 0x05,
  nci_nfc_bit_rate_6780                                   = 0x06
} nci_nfc_bit_rate_t;

/**
 * nci_rf_protocol_t
 * ----------
 * @brief Enum to hold values for all non proprietary NCI RF Protocols.
 *        See NCI 1.0 spec table 98.
 */
typedef enum {
  nci_protocol_undetermined                               = 0x00,
  nci_protocol_t1t                                        = 0x01,
  nci_protocol_t2t                                        = 0x02,
  nci_protocol_t3t                                        = 0x03,
  nci_protocol_iso_dep                                    = 0x04,
  nci_protocol_nfc_dep                                    = 0x05
} nci_rf_protocol_t;

/**
 * nci_rf_interface_t
 * ----------
 * @brief Enum to hold values for all non proprietary NCI RF Interfaces.
 *        See NCI 1.0 spec table 99.
 */
typedef enum {
  nci_rf_interface_nfcee_direct                           = 0x00,
  nci_rf_interface_frame                                  = 0x01,
  nci_rf_interface_iso_dep                                = 0x02,
  nci_rf_interface_nfc_dep                                = 0x03
} nci_rf_interface_t;

/**
 * nci_nfcee_protocol_interface_t
 * ----------
 * @brief Enum to hold values for all non proprietary NCI NFCEE
 *   Protocols/Interfaces.
 *        See NCI 1.0 spec table 100.
 */
typedef enum {
  nci_nfcee_protocol_interface_apdu                       = 0x00,
  nci_nfcee_protocol_interface_hci_access                 = 0x01,
  nci_nfcee_protocol_interface_t3t_cmd_set                = 0x02,
  nci_nfcee_protocol_interface_transparent                = 0x03
} nci_nfcee_protocol_interface_t;

/**********************************
*             NCI GID            *
**********************************/

/**
 * nci_gid_t
 * ----------
 * @brief Enum to hold values for all NCI Group Identifier (GID).
 *        See NCI 1.0 spec table 102.
 */
typedef enum {
  nci_gid_nci_core                                        = 0x00,
  nci_gid_rf_management                                   = 0x01,
  nci_gid_nfcee_management                                = 0x02,
  nci_gid_proprietary                                     = 0x0F
} nci_gid_t;

/**********************************
*             NCI OID            *
**********************************/

/**
 * nci_oid_core_t
 * ----------
 * @brief Enum to hold values for all NCI Core Opcode Identifier (OID).
 *        See NCI 1.0 spec table 102.
 */
typedef enum {
  nci_oid_core_reset                                      = 0x00,
  nci_oid_core_init                                       = 0x01,
  nci_oid_core_set_config                                 = 0x02,
  nci_oid_core_get_config                                 = 0x03,
  nci_oid_core_conn_create                                = 0x04,
  nci_oid_core_conn_close                                 = 0x05,
  nci_oid_core_conn_credits                               = 0x06,
  nci_oid_core_generic_error                              = 0x07,
  nci_oid_core_interface_error                            = 0x08
} nci_oid_core_t;

/**
 * nci_oid_rf_management_t
 * ----------
 * @brief Enum to hold values for all NCI RF Management Opcode Identifier (OID).
 *        See NCI 1.0 spec table 102.
 */
typedef enum {
  nci_oid_rf_discover_map                                 = 0x00,
  nci_oid_rf_set_listen_mode_routing                      = 0x01,
  nci_oid_rf_get_listen_mode_routing                      = 0x02,
  nci_oid_rf_discover                                     = 0x03,
  nci_oid_rf_discover_select                              = 0x04,
  nci_oid_rf_intf_activated                               = 0x05,
  nci_oid_rf_deactivate                                   = 0x06,
  nci_oid_rf_field_info                                   = 0x07,
  nci_oid_rf_t3t_polling                                  = 0x08,
  nci_oid_rf_nfcee_action                                 = 0x09,
  nci_oid_rf_nfcee_discovery_req                          = 0x0A,
  nci_oid_rf_parameter_update                             = 0x0B
} nci_oid_rf_management_t;

/**
 * nci_oid_nfcee_management_t
 * ----------
 * @brief Enum to hold values for all NCI NFCEE Management Opcode Identifier
 *   (OID).
 *        See NCI 1.0 spec table 102.
 */
typedef enum {
  nci_oid_nfcee_discover                                  = 0x00,
  nci_oid_nfcee_mode_set                                  = 0x01
} nci_oid_nfcee_management_t;

/**********************************
*        NCI Packet Generic      *
**********************************/

/**
 * nci_packet_mt_t
 * ----------
 * @brief Enum to hold values for all NCI Message Types (MT).
 */
typedef enum {
  nci_packet_data                                         = 0x00,
  nci_packet_control_cmd                                  = 0x01,
  nci_packet_control_rsp                                  = 0x02,
  nci_packet_control_ntf                                  = 0x03
} nci_packet_mt_t;

/**
 * nci_packet_pbf_t
 * ----------
 * @brief Enum to hold values for NCI Packet Boundary Flag (PBF).
 */
typedef enum {
  nci_pbf_complete_msg                                    = 0x00,
  nci_pbf_segment_msg                                     = 0x01
} nci_packet_pbf_t;

typedef struct {
  nci_packet_mt_t  mt;
  nci_packet_pbf_t pbf;
  nci_gid_t        gid;
  uint8_t          oid;
  uint8_t          payload_len;
  uint8_t *payload;
} nci_control_packet_t;

typedef struct {
  uint8_t  pbf;
  uint8_t  conn_id;
  uint8_t  payload_len;
  uint8_t *payload;
} nci_data_packet_t;

typedef struct {
  uint8_t  conn_id;
  uint8_t *payload;
} nci_rec_data_packet_t;

/**********************************
*           Core Reset           *
**********************************/

/**
 * nci_core_reset_reset_type_t
 * ----------
 * @brief
 */
typedef enum {
  nci_core_reset_keep_config                              = 0x00,
  nci_core_reset_reset_conig                              = 0x01
} nci_core_reset_reset_type_t;

typedef enum {
  nci_core_reset_config_kept                              = 0x00,
  nci_core_reset_config_reset                             = 0x01
} nci_core_reset_config_status_t;

typedef struct {
  uint8_t  reset_type;
} nci_core_reset_cmd_t;

typedef struct {
  nci_status_code_t               status;
  uint8_t                         nci_version;
  nci_core_reset_config_status_t  config_status;
} nci_core_reset_rsp_t;

typedef struct {
  uint8_t  reason_code;
  uint8_t  config_status;
} nci_core_reset_ntf_t;

/**********************************
*           Core Init            *
**********************************/

typedef struct {
  uint8_t  status;
  uint8_t *nfcc_feature;
  uint8_t  num_rf_interface;
  uint8_t *rf_interfaces;
  uint8_t  max_logical_conn;
  uint8_t *max_routing_tab_size;
  uint8_t  max_ctrl_pkt_payload_size;
  uint8_t *max_size_large_param;
  uint8_t  manu_id;
  uint8_t *manu_spec_info;
} nci_core_init_rsp_t;

/**********************************
*         Core Set Config        *
**********************************/

typedef struct {
  uint8_t  id;
  uint8_t  len;
  uint8_t *value;
} nci_core_set_config_param_t;

typedef struct {
  uint8_t num_of_params;
  nci_core_set_config_param_t *param;
} nci_core_set_config_cmd_t;

typedef struct {
  nci_status_code_t status;
  uint8_t  num_of_param_ids;
  uint8_t *param_ids;
} nci_core_set_config_rsp_t;

/**********************************
*         Core Get Config        *
**********************************/

typedef struct {
} nci_core_get_config_cmd_t;

typedef struct {
} nci_core_get_config_rsp_t;

/**********************************
*         Core Conn Create       *
**********************************/

typedef struct {
} nci_core_conn_create_cmd_t;

typedef struct {
} nci_core_conn_create_rsp_t;

/**********************************
*         Core Conn Close        *
**********************************/

typedef struct {
} nci_core_conn_close_cmd_t;

typedef struct {
} nci_core_conn_close_rsp_t;

/**********************************
*        Core Conn Credits       *
**********************************/

typedef struct {
} nci_core_conn_credits_ntf_t;

/**********************************
*       Core Generic Error       *
**********************************/

typedef struct {
  nci_status_code_t status;
} nci_core_generic_error_ntf_t;

/**********************************
*      Core Interface Error      *
**********************************/

typedef struct {
  nci_status_code_t status;
  uint8_t           conn_id;
} nci_core_interface_error_ntf_t;

/**********************************
*         RF Discover Map        *
**********************************/

typedef enum {
  nci_rf_mapping_mode_poll                    = 0x01,
  nci_rf_mapping_mode_listen                  = 0x02
} nci_rf_mapping_mode_t;

typedef struct {
  nci_rf_protocol_t       rf_protocol;
  nci_rf_mapping_mode_t   mode;
  nci_rf_interface_t      rf_interface;
} nci_rf_mapping_config_t;

typedef struct {
  uint8_t                  num_of_mapping_config;
  nci_rf_mapping_config_t *mapping_config;
} nci_rf_discover_map_cmd_t;

typedef struct {
  nci_status_code_t status;
} nci_rf_discover_map_rsp_t;

/**********************************
*   RF Set Listen Mode Routing   *
**********************************/

typedef enum {
  tech_based_routing_entry        = 0x00,
  protocol_based_routing_entry    = 0x01,
  aid_based_routing_entry         = 0x02
} nci_rf_listen_mode_routing_entry_t;

typedef struct {
  uint8_t                 route;
  uint8_t                 power_state;
  nci_nfc_rf_technology_t tech;
} nci_rf_tech_based_routing_value_t;

typedef struct {
  uint8_t             route;
  uint8_t             power_state;
  nci_rf_protocol_t   protocol;
} nci_rf_protocol_based_routing_value_t;

typedef struct {
  uint8_t     route;
  uint8_t     power_state;
  uint8_t *aid;
} nci_rf_aid_based_routing_value_t;

typedef union {
  nci_rf_tech_based_routing_value_t      tech_based_routing_value;
  nci_rf_protocol_based_routing_value_t  protocol_based_routing_value;
  nci_rf_aid_based_routing_value_t       aid_based_routing_value;
} nci_rf_listen_mode_routing_value_t;

typedef struct {
  nci_rf_listen_mode_routing_entry_t  type;
  uint8_t                             length;
  nci_rf_listen_mode_routing_value_t  value;
} nci_rf_listen_mode_routing_tlv_t;

typedef struct {
  bool                               more_messages;
  uint8_t                            num_of_routing_entries;
  nci_rf_listen_mode_routing_tlv_t *routing_entries;
} nci_rf_set_listen_mode_routing_cmd_t;

typedef struct {
  nci_status_code_t status;
} nci_rf_set_listen_mode_routing_rsp_t;

/**********************************
*   RF Get Listen Mode Routing   *
**********************************/

typedef struct {
  nci_status_code_t status;
} nci_rf_get_listen_mode_routing_rsp_t;

typedef struct {
} nci_rf_get_listen_mode_routing_ntf_t;

/**********************************
*          RF Discover           *
**********************************/
typedef struct {
  uint8_t  num_of_config;
  uint8_t *configurations;
} nci_rf_discover_cmd_t;

typedef struct {
  uint8_t status;
} nci_rf_discover_rsp_t;

typedef enum {
  nci_rf_disc_ntf_type_last_ntf                           = 0x00,
  nci_rf_disc_ntf_type_last_ntf_nfcc_limit                = 0x01,
  nci_rf_disc_ntf_type_more_ntf                           = 0x02
} nci_rf_discover_ntf_type_t;

typedef struct {
  uint8_t                                 rf_disc_id;
  nci_rf_protocol_t                       rf_protocol;
  nci_nfc_rf_tech_and_mode_t              rf_tech_and_mode;
  uint8_t                                 rf_tech_param_len;
  nci_rf_discover_ntf_rf_tech_param_t *rf_tech_param;
  nci_rf_discover_ntf_type_t              ntf_type;
} nci_rf_discover_ntf_t;

/**********************************
*       RF Discover Select       *
**********************************/

typedef struct {
} nci_rf_discover_select_cmd_t;

typedef struct {
  nci_status_code_t stauts;
} nci_rf_discover_select_rsp_t;

/**********************************
*        RF Intf Activated       *
**********************************/

typedef struct {
  uint8_t                             rf_disc_id;
  nci_rf_interface_t                  rf_interface;
  nci_rf_protocol_t                   rf_protocol;
  nci_nfc_rf_tech_and_mode_t          act_rf_tech_and_mode;
  uint8_t                             max_data_packet_payload_size;
  uint8_t                             init_num_of_credits;
  uint8_t                             len_rf_tech_params;
  nci_rf_discover_ntf_rf_tech_param_t rf_tech_params;
  nci_nfc_rf_tech_and_mode_t          data_exchange_rf_tech_and_mode;
  nci_nfc_bit_rate_t                  data_exchange_transmit_bit_rate;
  nci_nfc_bit_rate_t                  data_exchange_receive_bit_rate;
  uint8_t                             len_act_params;
  // activation parameters to add
} nci_rf_intf_activated_ntf_t;

/**********************************
*         RF Deactivate          *
**********************************/

typedef enum {
  nci_rf_deact_type_idle_mode                             = 0x00,
  nci_rf_deact_type_sleep_mode                            = 0x01,
  nci_rf_deact_type_sleep_af_mode                         = 0x02,
  nci_rf_deact_type_discovery_mode                        = 0x03
} nci_rf_deactivate_type_t;

typedef enum {
  nci_rf_deact_reason_dh_request                          = 0x00,
  nci_rf_deact_reason_endpoint_request                    = 0x01,
  nci_rf_deact_reason_rf_link_loss                        = 0x02,
  nci_rf_deact_reason_nfc_b_bad_afi                       = 0x03
} nci_rf_deactivate_reason_t;

typedef struct {
  nci_rf_deactivate_type_t deactivate_type;
} nci_rf_deactivate_cmd_t;

typedef struct {
  nci_status_code_t status;
} nci_rf_deactivate_rsp_t;

typedef struct {
  nci_rf_deactivate_type_t    deactivate_type;
  nci_rf_deactivate_reason_t  deactivate_reason;
} nci_rf_deactivate_ntf_t;

/**********************************
*         RF Field Info          *
**********************************/

typedef struct {
} nci_rf_field_info_ntf_t;

/**********************************
*         RF T3T Polling         *
**********************************/

typedef struct {
} nci_rf_t3t_polling_cmd_t;

typedef struct {
  nci_status_code_t status;
} nci_rf_t3t_polling_rsp_t;

typedef struct {
  nci_status_code_t status;
  uint8_t           num_of_res;
} nci_rf_t3t_polling_ntf_t;

/**********************************
*         RF NFCEE Action        *
**********************************/

typedef struct {
} nci_rf_nfcee_action_ntf_t;

/**********************************
*     RF NFCEE Discovery Req     *
**********************************/

typedef struct {
} nci_rf_nfcee_discovery_req_ntf_t;

/**********************************
*       RF Parameter Update      *
**********************************/

typedef struct {
  uint8_t num_of_params;
} nci_rf_parameter_update_cmd_t;

typedef struct {
  nci_status_code_t status;
} nci_rf_parameter_update_rsp_t;

/**********************************
*         NFCEE Discover         *
**********************************/

typedef struct {
} nci_nfcee_discover_cmd_t;

typedef struct {
} nci_nfcee_discover_rsp_t;

typedef struct {
} nci_nfcee_discover_ntf_t;

/**********************************
*         NFCEE Mode Set         *
**********************************/

typedef struct {
} nci_nfcee_mode_set_cmd_t;

typedef struct {
  nci_status_code_t status;
} nci_nfcee_mode_set_rsp_t;

/**********************************
*         NCI Event Data         *
**********************************/

typedef union {
  nci_rec_data_packet_t                       rec_data_packet;

  /* NCI Core */
  nci_core_reset_rsp_t                        core_reset_rsp;
  nci_core_reset_ntf_t                        core_reset_ntf;
  nci_core_init_rsp_t                         core_init_rsp;
  nci_core_set_config_rsp_t                   core_set_config_rsp;
  nci_core_get_config_rsp_t                   core_get_config_rsp;
  nci_core_conn_create_rsp_t                  core_conn_create_rsp;
  nci_core_conn_close_rsp_t                   core_conn_close_rsp;
  nci_core_conn_credits_ntf_t                 core_conn_credits_ntf;
  nci_core_generic_error_ntf_t                core_generic_error_ntf;
  nci_core_interface_error_ntf_t              core_interface_error_ntf;

  /* RF Management */
  nci_rf_discover_map_rsp_t                   rf_discover_map_rsp;
  nci_rf_set_listen_mode_routing_rsp_t        rf_set_listen_mode_routing_rsp;
  nci_rf_get_listen_mode_routing_rsp_t        rf_get_listen_mode_routing_rsp;
  nci_rf_get_listen_mode_routing_ntf_t        rf_get_listen_mode_routing_ntf;
  nci_rf_discover_rsp_t                       rf_discover_rsp;
  nci_rf_discover_ntf_t                       rf_discover_ntf;
  nci_rf_discover_select_rsp_t                rf_discover_select_rsp;
  nci_rf_intf_activated_ntf_t                 rf_intf_activated_ntf;
  nci_rf_deactivate_rsp_t                     rf_deactivate_rsp;
  nci_rf_deactivate_ntf_t                     rf_deactivate_ntf;
  nci_rf_field_info_ntf_t                     rf_field_info_ntf;
  nci_rf_t3t_polling_rsp_t                    rf_t3t_polling_rsp;
  nci_rf_t3t_polling_ntf_t                    rf_t3t_polling_ntf;
  nci_rf_nfcee_action_ntf_t                   rf_nfcee_action_ntf;
  nci_rf_nfcee_discovery_req_ntf_t            rf_nfcee_discovery_req_ntf;
  nci_rf_parameter_update_rsp_t               rf_parameter_update_rsp;

  /* NFCEE Management */
  nci_nfcee_discover_rsp_t                    nfcee_discover_rsp;
  nci_nfcee_discover_ntf_t                    nfcee_discover_ntf;
  nci_nfcee_mode_set_rsp_t                    nfcee_mode_set_rsp;
} nci_data_t;

typedef struct {
  uint8_t                                     len;
  bool                                        is_terminator_packet;
  union {
    nci_data_t                              nci_data;
    nci_proprietary_data_t                  proprietary_data;
    uint8_t *payload;
  } payload;
} nci_evt_data_t;

typedef struct {
  nci_evt_header_t header;
  nci_evt_data_t   data;
} nci_evt_t;

/****************************************************
*                                                  *
*                      APIs                        *
*                                                  *
****************************************************/

/***************************************************************************//**
 * @brief
 *  Initialize NCI.
 ******************************************************************************/
void nci_init(void);

/***************************************************************************//**
 * @brief
 *  Get current NCI event.
 *
 * @returns
 *  Pointer to current NCI event.
 ******************************************************************************/
nci_evt_t * nci_get_event(void);

/***************************************************************************//**
 * @brief
 *  Calling this function would notify the NCI stack that a packet is incoming.
 *  Should be called in the NFCC interrupt ISR.
 ******************************************************************************/
void nci_notify_incoming_packet(void);

/***************************************************************************//**
 * @brief
 *  Check if there is any incoming packet waiting to be read.
 *
 * @returns
 *  true  - if there is an incoming packet to be read.
 *  false - if there is no incoming packet to be read.
 ******************************************************************************/
bool nci_check_incoming_packet(void);

/***************************************************************************//**
 * @brief
 *  Encode and send a NCI data packet.
 *
 * @param[in] packet
 *  NCI data packet to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_data_packet_send(nci_data_packet_t *packet);

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_RESET_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_RESET command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_reset(nci_core_reset_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_INIT_CMD.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_init(void);

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_SET_CONFIG_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_SET_CONFIG command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_set_config(nci_core_set_config_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_GET_CONFIG_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_GET_CONFIG command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_get_config(nci_core_get_config_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_CONN_CREATE_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_CONN_CREATE command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_conn_create(nci_core_conn_create_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_CONN_CLOSE_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_CONN_CLOSE command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_conn_close(nci_core_conn_close_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send RF_DISCOVER_MAP_CMD.
 *
 * @param[in] cmd
 *  NCI RF_DISCOVER_MAP command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_discover_map(nci_rf_discover_map_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send RF_SET_LISTEN_MODE_ROUTING_CMD.
 *
 * @param[in] cmd
 *  NCI RF_SET_LISTEN_MODE_ROUTING command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_set_listen_mode_routing(
  nci_rf_set_listen_mode_routing_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send RF_GET_LISTEN_MODE_ROUTING_CMD.
 *
 * @param[in] cmd
 *  NCI RF_GET_LISTEN_MODE_ROUTING command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_get_listen_mode_routing(void);

/***************************************************************************//**
 * @brief
 *  Encode and send RF_DISCOVER_CMD.
 *
 * @param[in] cmd
 *  NCI RF_DISCOVER command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_discover(nci_rf_discover_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send RF_DISCOVER_SELECT_CMD.
 *
 * @param[in] cmd
 *  NCI RF_DISCOVER_SELECT command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_discover_select(nci_rf_discover_select_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send RF_DEACTIVATE_CMD.
 *
 * @param[in] cmd
 *  NCI RF_DEACTIVATE command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_deactivate(nci_rf_deactivate_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send RF_T3T_POLLING_CMD.
 *
 * @param[in] cmd
 *  NCI RF_T3T_POLLING command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_t3t_polling(nci_rf_t3t_polling_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send RF_PARAMETER_UPDATE_CMD.
 *
 * @param[in] cmd
 *  NCI RF_PARAMETER_UPDATE command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_parameter_update(nci_rf_parameter_update_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send NFCEE_DISCOVER_CMD.
 *
 * @param[in] cmd
 *  NCI NFCEE_DISCOVER command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_nfcee_discover(nci_nfcee_discover_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Encode and send NFCEE_MODE_SET_CMD.
 *
 * @param[in] cmd
 *  NCI NFCEE_MODE_SET command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_nfcee_mode_set(nci_nfcee_mode_set_cmd_t *cmd);

/***************************************************************************//**
 * @brief
 *  Parse the incoming packet header and send to corresponding processing
 *  functions.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_incoming_packet_process(void);

/** @} (end addtogroup NFC Library) */

#ifdef __cplusplus
}
#endif
#endif
