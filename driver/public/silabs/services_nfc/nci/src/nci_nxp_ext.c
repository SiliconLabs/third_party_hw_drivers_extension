/***************************************************************************//**
 * @file   nci_nxp_ext.c
 * @brief  Implementation for NXP proprietary extension for NCI.
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

#include <string.h>
#include <stdint.h>
#include "nci.h"

/// Enum to hold values for all NXP NCI Proprietary Opcode Identifier (OID).
typedef enum {
  nci_oid_nxp_act                         = 0x02,
  nci_oid_nxp_rf_pres_check               = 0x11,
  nci_oid_nxp_rf_t4t_sblock               = 0x10,
  nci_oid_nxp_rf_tag_detector_trace       = 0x13,
  nci_oid_nxp_core_set_power_mode         = 0x00,
  nci_oid_nxp_rf_get_transition           = 0x14,
  nci_oid_nxp_test_prbs                   = 0x30,
  nci_oid_nxp_test_antenna                = 0x3D,
  nci_oid_nxp_test_get_register           = 0x33
} nci_oid_nxp_t;

extern nci_err_t nci_control_packet_send (nci_control_packet_t *packet);

// -------------------------------
// NCI_PROPRIETARY_ACT

/**************************************************************************//**
 * @brief
 *  Encode and send NCI_PROPRIETARY_ACT_CMD.
 *
 * @returns
 *  Any error code.
 *****************************************************************************/
nci_err_t nci_proprietary_nxp_act(void)
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_proprietary;
  packet.oid = nci_oid_nxp_act;
  packet.payload_len = 0;

  return nci_control_packet_send(&packet);
}

/**************************************************************************//**
 * @brief
 *  Process and load NCI_PROPRIETARY_ACT_RSP to nci event data.
 *****************************************************************************/
static void nci_proprietary_nxp_act_rsp_process(uint8_t *packet_buff)
{
  nci_evt_t *nci_evt = nci_get_event();

  nci_evt->header = nci_evt_proprietary_nxp_act_rsp_rec;
  nci_evt->data.is_terminator_packet = true;
  nci_evt->data.len = 5;

  uint16_t index = NCI_PACKET_PAYLOAD_START_INDEX;

  nci_evt->data.payload.proprietary_data.proprietary_nxp_act_rsp.status
    = packet_buff[index++];
  nci_evt->data.payload.proprietary_data.proprietary_nxp_act_rsp.fw_build_num
    = &packet_buff[index];
}

// -------------------------------
// RF_PRES_CHECK

nci_err_t nci_proprietary_nxp_rf_pres_check(void)
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_proprietary;
  packet.oid = nci_oid_nxp_rf_pres_check;
  packet.payload_len = 0;

  return nci_control_packet_send(&packet);
}

// -------------------------------
// RF_T4T_SBLOCK

nci_err_t nci_proprietary_nxp_rf_t4t_sblock()
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_proprietary;
  packet.oid = nci_oid_nxp_rf_t4t_sblock;
//    packet.payload_len = 1;

  // payload config

  return nci_control_packet_send(&packet);
}

// -------------------------------
// CORE_SET_POWER_MODE

nci_err_t nci_proprietary_nxp_core_set_power_mode()
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_proprietary;
  packet.oid = nci_oid_nxp_core_set_power_mode;
  packet.payload_len = 1;

  // payload config

  return nci_control_packet_send(&packet);
}

// -------------------------------
// RF_GET_TRANSITION

nci_err_t nci_proprietary_nxp_rf_get_transition()
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_proprietary;
  packet.oid = nci_oid_nxp_rf_get_transition;
  packet.payload_len = 2;

  // payload config

  return nci_control_packet_send(&packet);
}

// -------------------------------
// TEST_PRBS

nci_err_t nci_proprietary_nxp_test_prbs()
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_proprietary;
  packet.oid = nci_oid_nxp_test_prbs;
  packet.payload_len = 7;

  // payload config

  return nci_control_packet_send(&packet);
}

// -------------------------------
// TEST_ANTENNA

nci_err_t nci_proprietary_nxp_test_antenna()
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_proprietary;
  packet.oid = nci_oid_nxp_test_antenna;
//    packet.payload_len = ;

  // payload config

  return nci_control_packet_send(&packet);
}

// -------------------------------
// TEST_GET_REGISTER

nci_err_t nci_proprietary_nxp_test_get_register(void)
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_proprietary;
  packet.oid = nci_oid_nxp_test_get_register;
  packet.payload_len = 0;

  return nci_control_packet_send(&packet);
}

// -----------------------------------------------------------------------------
// Incoming Packet Parser

nci_err_t nci_incoming_proprietary_packet_rsp_process(uint8_t *packet_buff)
{
  switch ((packet_buff[NCI_PACKET_OID_INDEX] & NCI_PACKET_OID_M)
          >> NCI_PACKET_OID_SHIFT) {
    case nci_oid_nxp_act: {
      nci_proprietary_nxp_act_rsp_process(packet_buff);
      break;
    }
    case nci_oid_nxp_rf_pres_check: {
      break;
    }
    case nci_oid_nxp_rf_t4t_sblock: {
      break;
    }
    case nci_oid_nxp_core_set_power_mode: {
      break;
    }
    case nci_oid_nxp_rf_get_transition: {
      break;
    }
    case nci_oid_nxp_test_prbs: {
      break;
    }
    case nci_oid_nxp_test_antenna: {
      break;
    }
    case nci_oid_nxp_test_get_register: {
      break;
    }
    default:
      /* error */
      break;
  }
  return nci_err_none;
}

nci_err_t nci_incoming_proprietary_packet_ntf_process(uint8_t *packet_buff)
{
  switch ((packet_buff[NCI_PACKET_OID_INDEX] & NCI_PACKET_OID_M)
          >> NCI_PACKET_OID_SHIFT) {
    case nci_oid_nxp_rf_pres_check: {
      break;
    }
    case nci_oid_nxp_rf_t4t_sblock: {
      break;
    }
    case nci_oid_nxp_rf_tag_detector_trace: {
      break;
    }
    default:
      /* error */
      break;
  }
  return nci_err_none;
}
