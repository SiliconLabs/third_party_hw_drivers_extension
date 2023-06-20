/***************************************************************************//**
 * @file
 * @brief Top level application functions
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
#include <string.h>
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "gpiointerrupt.h"
#include "app_log.h"
#include "app_assert.h"

#include "t2t.h"
#include "nci.h"
#include "ndef_message.h"
#include "nfc_tlv.h"

#include "mikroe_pn7150_config.h"
#include "mikroe_pn7150.h"

static void app_int_callback(uint8_t intNo);

/*
 * T2T Write Process
 * -----------------------------------------------------------------------------
 * ----- Preparation Sequence ------
 * DH --> Core Reset CMD                                                --> NFCC
 * DH <-- Core Reset RSP                                                <-- NFCC
 * DH --> Core Init CMD                                                 --> NFCC
 * DH <-- Core Init RSP                                                 <-- NFCC
 * DH --> NXP Proprietary Act CMD                                       --> NFCC
 * DH <-- NXP Proprietary Act RSP                                       <-- NFCC
 * ----- RF Discover Phase ------
 * DH --> RF Discover Map CMD
 * (RF Prot. = PROTOCOL_T2T, Mode = Poll, RF Intf. = Frame RF)          --> NFCC
 * DH <-- RF Discover Map RSP                                           <-- NFCC
 * DH --> RF Discover CMD (NFC_A_PASSIVE_POLL_MODE)                     --> NFCC
 * DH <-- RF Discover RSP                                               <-- NFCC
 * DH <-- RF Intf Activated NTF (Prot = PROTOCOL_T2T, Intf = Frame RF)  <-- NFCC
 * ----- Data Exhange Phase (REPEAT) ------
 * DH --> NCI Data Message (WRITE Command, Block Address, Data)         <-- NFCC
 * DH <-- Core Conn Credits NTF                                         <-- NFCC
 * DH <-- NCI Data Message (ACK or NACK Response)                       <-- NFCC
 * ----- RF Deactivate Phase ------
 * DH --> RF Deactivate CMD (Discovery)                                 <-- NFCC
 * DH <-- RF Deactivate RSP                                             <-- NFCC
 * DH --> RF Deactivate NTF                                             <-- NFCC
 * ----- Go Back to RF Discover Phase ------
 */

#define TLV_BUFFER_SIZE             100
#define PAYLOAD_BUFFER_SIZE         100

char *content = "silabs.com/wireless/bluetooth";
uint8_t payload[PAYLOAD_BUFFER_SIZE];

ndef_record_t record = {
  .header = {
    .mb = 1,
    .me = 1,
    .cf = 0,
    .sr = 1,
    .il = 0,
    .tnf = ndefTnfWellKnown
  },
  .type_length = 1,
  .type = (uint8_t *) NDEF_RTD_WKT_GLOBAL_URI
};

ndef_record_t ndef_message[1];

uint8_t ndef_message_buff[TLV_BUFFER_SIZE - 3];
uint8_t tlv_buff[TLV_BUFFER_SIZE];

//    uint8_t conn_credits = 0;
uint16_t write_index = 0;
uint32_t write_size;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  // Initialize PN71x0 I2C communication.
  if (SL_STATUS_OK != mikroe_pn7150_init(sl_i2cspm_mikroe)) {
    app_log("> PN7150 - NFC 2 Click board driver init failed.\n");
  }

  GPIO_ExtIntConfig(MIKROE_PN7150_INT_PORT,
                    MIKROE_PN7150_INT_PIN,
                    MIKROE_PN7150_INT_PIN,
                    1,
                    0,
                    1);
  GPIOINT_CallbackRegister(MIKROE_PN7150_INT_PIN, app_int_callback);
  GPIO_IntEnable(MIKROE_PN7150_INT_PIN);
  app_log("        HW Reset       \r\n");
  mikroe_pn7150_hw_reset();

  // Initialize NCI.
  nci_init();

  /* Print project name. */
  app_log("\r\n******************************\r\n*\r\n");
  app_log("* NCI T2T Write Demo\r\n");
  app_log("*\r\n******************************\r\n");

  app_log("Write content: %s\r\n", content);

  record.payload = payload;
  // prefix + content
  record.payload_length = 1 + strlen(content);

  ndef_message[0] = record;

  payload[0] = NDEF_RTD_WKT_URI_PREFIX_HTTPS;

  memcpy(&payload[1], content, strlen(content));

  // encode ndef message
  ndef_message_encode_result_t ndef_message_encode_result;
  ndef_message_encode_result = ndef_message_encode(ndef_message_buff,
                                                   ndef_message);
  if (ndef_message_encode_result.err == ndefMessageEncodeFail) {
    app_log("NDEF message encode failed:(\r\n");
    return;
  }

  write_size = ndef_message_encode_result.size;

  // encode ndef tlv
  if (tlv_encode(tlv_buff,
                 TLV_BUFFER_SIZE,
                 NFC_T2T_NDEF_MESSAGE_TLV,           // T
                 ndef_message_encode_result.size,    // L
                 ndef_message_buff                   // V
                 ) != tlvEncodeCompleted) {
    app_log("NDEF TLV encode failed:(\r\n");
    return;
  }

  write_size += 2;

  if (tlv_encode(&tlv_buff[write_size],
                 (TLV_BUFFER_SIZE - write_size),
                 NFC_T2T_TERMINATOR_TLV,        // T
                 0,                             // L
                 NULL                           // V
                 ) != tlvEncodeCompleted) {
    app_log("Terminator TLV encode failed:(\r\n");
    return;
  }

  write_size++;
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  /* Get NCI event. */
  nci_evt_t *nci_evt = nci_get_event();

  if (nci_evt->header == nci_evt_none) {
    return;
  }

  /* Log NCI event, if debug enabled. */
  nci_evt_log(nci_evt->header);

  switch (nci_evt->header) {
    /* Start up event, enter init sequence. */
    case nci_evt_startup: {
      /* Step 1 in init sequence: core reset. */
      nci_core_reset_cmd_t core_reset_cmd;

      /* Keep configurations. */
      core_reset_cmd.reset_type = nci_core_reset_keep_config;

      /* Send command and check for error/ */
      nci_err_t nci_err = nci_core_reset(&core_reset_cmd);
      app_assert(nci_err == nci_err_none,
                 "NCI core reset error: %x \r\n",
                 nci_err);
      break;
    }

    /* Event generated by reception of core reset response. */
    case nci_evt_core_reset_rsp_rec: {
      /* Get core reset response data: NCI version of NFCC. */
      uint8_t nci_version =
        nci_evt->data.payload.nci_data.core_reset_rsp.nci_version;

      /* Get core reset response data: configuration status. */
      uint8_t config_status =
        nci_evt->data.payload.nci_data.core_reset_rsp.config_status;

      /* Log NCI version of NFCC. */
      app_log("NFCC's NCI Version is %x.%x\r\n",
              ((nci_version & 0xF0) >> 4),
              (nci_version & 0x0F));

      /* Log configuration status. */
      if (config_status == nci_core_reset_keep_config) {
        nci_log_ln("NCI RF Configuration has been kept.");
      } else if (config_status == nci_core_reset_reset_conig) {
        nci_log_ln("NCI RF Configuration has been reset. ");
      }

      /* Step 2 in init sequence: core init.
       * Send command and check for error.
       */
      nci_err_t nci_err = nci_core_init();
      app_assert(nci_err == nci_err_none,
                 "NCI core init error: %x \r\n",
                 nci_err);
      break;
    }

    /* Event generated by reception of core init response. */
    case nci_evt_core_init_rsp_rec: {
      uint8_t *manu_spec_info =
        nci_evt->data.payload.nci_data.core_init_rsp.manu_spec_info;

      app_log("NFCC's Firmware Version is %02x.%02x\r\n",
              manu_spec_info[2],
              manu_spec_info[3]);

      /* Activate NXP proprietary extensions,
       * send command and check for error.
       */
      nci_err_t nci_err = nci_proprietary_nxp_act();
      app_assert(nci_err == nci_err_none,
                 "NCI core init error: %x \r\n",
                 nci_err);
      break;
    }

    /* Event generated by reception of proprietary nxp act response. */
    case nci_evt_proprietary_nxp_act_rsp_rec: {
      nci_rf_mapping_config_t mapping_config_1 = {
        .rf_protocol = nci_protocol_t2t,
        .mode = nci_rf_mapping_mode_poll,
        .rf_interface = nci_rf_interface_frame
      };

      nci_rf_mapping_config_t mapping_config[1] = { mapping_config_1 };

      nci_rf_discover_map_cmd_t cmd = {
        .num_of_mapping_config = 1,
        .mapping_config = mapping_config
      };

      nci_err_t nci_err = nci_rf_discover_map(&cmd);
      app_assert(nci_err == nci_err_none,
                 "NCI RF discovery map error: %x \r\n",
                 nci_err);
      break;
    }

    case nci_evt_rf_discover_map_rsp_rec: {
      if (nci_evt->data.payload.nci_data.rf_discover_map_rsp.status
          != nci_status_ok) {
        nci_log_ln("RF Discover Map Response status not ok.");
        return;
      }

      uint8_t config[] = { nci_nfc_a_passive_poll_mode, 1 };

      nci_rf_discover_cmd_t cmd = {
        .num_of_config = 1,
        .configurations = config
      };

      nci_err_t nci_err = nci_rf_discover(&cmd);
      app_assert(nci_err == nci_err_none,
                 "NCI RF discovery error: %x \r\n",
                 nci_err);
      break;
    }

    case nci_evt_rf_discover_rsp_rec: {
      if (nci_evt->data.payload.nci_data.rf_discover_rsp.status
          != nci_status_ok) {
        nci_log_ln("RF Discover Response status not ok.");
        return;
      }

      /* Wait for NTF */
      break;
    }

    case nci_evt_rf_intf_activated_ntf_rec: {
      uint8_t t2t_cmd_buff[] = {
        T2T_CMD_WRITE,
        write_index + 4,
        tlv_buff[write_index * 4],
        tlv_buff[write_index * 4 + 1],
        tlv_buff[write_index * 4 + 2],
        tlv_buff[write_index * 4 + 3]
      };

      nci_data_packet_t pakcet = {
        .pbf = nci_pbf_complete_msg,
        .conn_id = 0,
        .payload_len = 6,
        .payload = t2t_cmd_buff
      };

      nci_err_t nci_err = nci_data_packet_send(&pakcet);
      app_assert(nci_err == nci_err_none,
                 "NCI RF discovery map error: %x \r\n",
                 nci_err);
      break;
    }
    case nci_evt_data_packet_rec: {
      if (nci_evt->data.payload.nci_data.rec_data_packet.payload[0]
          != T2T_RSP_ACK) {
        app_log("T2T Write NACK response. \r\n");
      }

      write_index++;

      int num_bytes_to_write = write_size - (write_index * 4);

      uint8_t t2t_cmd_buff[6] = { 0 };

      if (num_bytes_to_write > 0) {
        t2t_cmd_buff[0] = T2T_CMD_WRITE;
        t2t_cmd_buff[1] = write_index + 4;
        t2t_cmd_buff[2] = tlv_buff[write_index * 4];
        if (num_bytes_to_write >= 4) {
          t2t_cmd_buff[3] = tlv_buff[write_index * 4 + 1];
          t2t_cmd_buff[4] = tlv_buff[write_index * 4 + 2];
          t2t_cmd_buff[5] = tlv_buff[write_index * 4 + 3];
        } else if (num_bytes_to_write == 3) {
          t2t_cmd_buff[3] = tlv_buff[write_index * 4 + 1];
          t2t_cmd_buff[4] = tlv_buff[write_index * 4 + 2];
        } else if (num_bytes_to_write == 2) {
          t2t_cmd_buff[3] = tlv_buff[write_index * 4 + 1];
        }
        nci_data_packet_t pakcet = {
          .pbf = nci_pbf_complete_msg,
          .conn_id = 0,
          .payload_len = 6,
          .payload = t2t_cmd_buff
        };
        nci_data_packet_send(&pakcet);
      } else {
        app_log("T2T write completed.\r\n");
        write_index = 0;
        nci_rf_deactivate_cmd_t cmd = {
          .deactivate_type = nci_rf_deact_type_discovery_mode
        };

        nci_err_t nci_err = nci_rf_deactivate(&cmd);
        app_assert(nci_err == nci_err_none,
                   "NCI rf deactive error: %x \r\n",
                   nci_err);
      }

      break;
    }
    case nci_evt_core_conn_credits_ntf_rec: {
      break;
    }
    case nci_evt_rf_deactivate_rsp_rec: {
      break;
    }
    case nci_evt_rf_deactivate_ntf_rec: {
      break;
    }

    /* Meh */
    default:
      break;
  }
}

static void app_int_callback(uint8_t intNo)
{
  (void) intNo;
  nci_notify_incoming_packet();
}
