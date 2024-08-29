/***************************************************************************//**
 * @file gnss_max_m10s_ubx.c
 * @brief MAX_M10S GNSS receiver driver external APIs implementation for UBX
 * sentence data
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
#include <stdlib.h>
#include "gnss_max_m10s_ubx.h"

/**************************************************************************//**
 * @brief initializes packet for UBXNAVPVT packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavpvt(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVSTATUS packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavstatus(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVSAT packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavsat(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVVELNED packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavvelned(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVSIG packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavsig(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVEOE packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavepoch(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVCLOCK packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavclock(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVDOP packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavdop(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVTIMELS packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavtimels(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVPOSLLH packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavposllh(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief initializes packet for UBXNAVTIMEUTC packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_ubxnavtimeutc(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Given a spot in the payload array, extract four bytes and build a long
 * @param[in] msg : pointer to the payload
 * @param[in] spot_to_start : starting spot for the extraction.
 * @return long type data extracted from the payload.
 *****************************************************************************/
static uint32_t gnss_max_m10s_extract_long(sl_max_m10s_ubx_packet_t *msg,
                                           uint16_t spot_to_start);

/**************************************************************************//**
 * @brief Given a spot, extract a signed 8-bit value from the payload
 * @param[in] msg : pointer to the payload
 * @param[in] spot_to_start : starting spot for the extraction.
 * @return char type data extracted from the payload.
 *****************************************************************************/
static int8_t gnss_max_m10s_extract_signed_char(sl_max_m10s_ubx_packet_t *msg,
                                                uint16_t spot_to_start);

/**************************************************************************//**
 * @brief Just so there is no ambiguity about whether a uint32_t will cast to
 *        a int32_t correctly
 * @param[in] msg : pointer to the payload
 * @param[in] spot_to_start : starting spot for the extraction.
 * @return signed long type data extracted from the payload.
 *****************************************************************************/
static int32_t gnss_max_m10s_extract_signed_long(sl_max_m10s_ubx_packet_t *msg,
                                                 uint16_t spot_to_start);

/**************************************************************************//**
 * @brief Given a spot in the payload array, extract two bytes and build an int
 * @param[in] msg : pointer to the payload
 * @param[in] spot_to_start : starting spot for the extraction.
 * @return integer byte extracted from the payload.
 *****************************************************************************/
static uint16_t gnss_max_m10s_extract_int(sl_max_m10s_ubx_packet_t *msg,
                                          uint16_t spot_to_start);

/**************************************************************************//**
 * @brief check for the ambiguity about whether a uint16_t will cast to
 *        a int16_t correctly
 * @param[in] msg : pointer to the payload
 * @param[in] spot_to_start : starting spot for the extraction.
 * @return signed integer byte extracted from the payload.
 *****************************************************************************/
static int16_t gnss_max_m10s_extract_signed_int(sl_max_m10s_ubx_packet_t *msg,
                                                uint16_t spot_to_start);

/**************************************************************************//**
 * @brief Get the latest Position/Velocity/Time solution.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_pvt(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the information about the upcoming leap second event
 *        if one is scheduled.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_timels(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the information about the UTC time.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_timeutc(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Get geodetic position solution of the UBX data.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_posllh(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the latest receiver navigation status.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_status(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the satellite information tracked by GNSS receiver.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_sat(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the signal information currently tracked by GNSS receiver.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_sig(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the dilution of precision UBX data.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_dop(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the navigation clock solution UBX data.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_clock(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the velocity solution in NED frame UBX data.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_velned(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the navigation end of epoch UBX data.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_get_nav_epoch(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

void gnss_max_m10s_process_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                               uint8_t incoming,
                               uint8_t requested_class,
                               uint8_t requested_id)
{
  sl_max_m10s_ubx_packet_t *incomingUBX = NULL;
  uint16_t maximum_payload_size;
  bool overrun;

  if (gnss_cfg_data->active_packet_buffer
      == SL_MAX_M10S_UBLOX_PACKET_PACKETCFG) {
    maximum_payload_size = gnss_cfg_data->msg_data.packet_cfg_payloadsize;
    incomingUBX = &(gnss_cfg_data->packet_cfg);
  } else if (gnss_cfg_data->active_packet_buffer
             == SL_MAX_M10S_UBLOX_PACKET_PACKETAUTO) {
    incomingUBX = &(gnss_cfg_data->packet_auto);

    bool log_because_auto =
      gnss_max_m10s_auto_lookup(gnss_cfg_data,
                                &maximum_payload_size);

    if ((!log_because_auto)) {
      maximum_payload_size = UBX_MAX_LENGTH;
    }
  } else if (gnss_cfg_data->active_packet_buffer
             == SL_MAX_M10S_UBLOX_PACKET_PACKETACK) {
    incomingUBX = &(gnss_cfg_data->packet_ack);
    maximum_payload_size = 2;
  } else {
    if (gnss_cfg_data->active_packet_buffer
        == SL_MAX_M10S_UBLOX_PACKET_PACKETBUF) {
      incomingUBX = &(gnss_cfg_data->packet_buf);
    } else if (gnss_cfg_data->active_packet_buffer
               == SL_MAX_M10S_UBLOX_PACKET_PACKETACK) {
      incomingUBX = &(gnss_cfg_data->packet_ack);
    }
    maximum_payload_size = 2;
  }

  overrun = false;

  if (incomingUBX->counter < (incomingUBX->len + 4)) {
    gnss_max_m10s_add_to_checksum(gnss_cfg_data, incoming);
  }

  /// for UBX packet class
  if (incomingUBX->counter == 0) {
    incomingUBX->cls = incoming;
  } else if (incomingUBX->counter == 1) { /// for UBX packet ID
    incomingUBX->id = incoming;
  } else if (incomingUBX->counter == 2) { /// for LSB of data length
    incomingUBX->len = incoming;
  } else if (incomingUBX->counter == 3) { /// for MSB of data length
    incomingUBX->len |= incoming << 8;
  } else if (incomingUBX->counter == incomingUBX->len + 4) { /// for checksum_a
    incomingUBX->checksum_a = incoming;
  } else if (incomingUBX->counter == incomingUBX->len + 5) { /// for checksum_b
    incomingUBX->checksum_b = incoming;
    gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE;

    if ((incomingUBX->checksum_a == gnss_cfg_data->rolling_checksum_a)
        && (incomingUBX->checksum_b == gnss_cfg_data->rolling_checksum_b)) {
      incomingUBX->valid = SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID;
      gnss_cfg_data->signs_of_life = true;

      if ((incomingUBX->cls == requested_class)
          && (incomingUBX->id == requested_id)) {
        incomingUBX->class_and_id_match =
          SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID;
      } else if ((incomingUBX->cls == UBX_CLASS_ACK)
                 && (incomingUBX->id == UBX_ACK_ACK)
                 && (incomingUBX->payload[0] == requested_class)
                 && (incomingUBX->payload[1] == requested_id)) {
        incomingUBX->class_and_id_match =
          SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID;
      } else if ((incomingUBX->cls == UBX_CLASS_ACK)
                 && (incomingUBX->id == UBX_CLASS_NACK)
                 && (incomingUBX->payload[0] == requested_class)
                 && (incomingUBX->payload[1] == requested_id)) {
        incomingUBX->class_and_id_match =
          SL_MAX_M10S_UBLOX_PACKET_NOTACKNOWLEDGED;
      } else if ((gnss_max_m10s_auto_lookup(gnss_cfg_data,
                                            NULL))) {
        /// leave incomingUBX->class_and_id_match _unchanged
      }

      if (gnss_cfg_data->ignore_this_payload == false) {
        gnss_max_m10s_process_ubx_packet(gnss_cfg_data, incomingUBX);
      }
    } else {
      incomingUBX->valid = SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID;

      if ((incomingUBX->cls == requested_class)
          && (incomingUBX->id == requested_id)) {
        incomingUBX->class_and_id_match =
          SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID;
      } else if ((incomingUBX->cls == UBX_CLASS_ACK)
                 && (incomingUBX->payload[0] == requested_class)
                 && (incomingUBX->payload[1] == requested_id)) {
        incomingUBX->class_and_id_match =
          SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID;
      }
    }

    if (gnss_cfg_data->active_packet_buffer
        == SL_MAX_M10S_UBLOX_PACKET_PACKETAUTO) {
      free(gnss_cfg_data->msg_data.payload_auto);

      gnss_cfg_data->msg_data.payload_auto = NULL;
      gnss_cfg_data->packet_auto.payload = gnss_cfg_data->msg_data.payload_auto;
    }
  } else {
    uint16_t starting_spot = incomingUBX->starting_spot;

    if (gnss_max_m10s_auto_lookup(gnss_cfg_data, false)) {
      starting_spot = 0;
    }

    /// Check if this is payload data which should be ignored
    if (gnss_cfg_data->ignore_this_payload == false) {
      if ((incomingUBX->counter - 4) >= starting_spot) {
        if (((incomingUBX->counter - 4) - starting_spot)
            < maximum_payload_size) {
          incomingUBX->payload[(incomingUBX->counter - 4)
                               - starting_spot] = incoming; /// Store this byte into payload array
        } else {
          overrun = true;
        }
      }
    }
  }

  if (overrun
      || ((incomingUBX->counter == maximum_payload_size + 6)
          && (gnss_cfg_data->ignore_this_payload == false))) {
    gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE;
  }

  incomingUBX->counter++;
}

void gnss_max_m10s_process_ubx_packet(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      sl_max_m10s_ubx_packet_t *msg)
{
  if ((msg->id == UBX_NAV_PVT) && (msg->len == UBX_NAV_PVT_LEN)) {
    sl_max_m10s_ubx_navpvt_process(gnss_cfg_data, msg);
  } else if ((msg->id == UBX_NAV_STATUS) && (msg->len == UBX_NAV_STATUS_LEN)) {
    sl_max_m10s_ubx_navstatus_process(gnss_cfg_data, msg);
  } else if ((msg->id == UBX_NAV_DOP) && (msg->len == UBX_NAV_DOP_LEN)) {
    sl_max_m10s_ubx_navdop_process(gnss_cfg_data, msg);
  } else if ((msg->id == UBX_NAV_CLOCK) && (msg->len == UBX_NAV_CLOCK_LEN)) {
    sl_max_m10s_ubx_navclock_process(gnss_cfg_data, msg);
  } else if ((msg->id == UBX_NAV_EOE) && (msg->len == UBX_NAV_EOE_LEN)) {
    sl_max_m10s_ubx_navepoch_process(gnss_cfg_data, msg);
  } else if (msg->id == UBX_NAV_SAT) {
    sl_max_m10s_ubx_navsat_process(gnss_cfg_data, msg);
  } else if ((msg->id == UBX_NAV_TIMELS) && (msg->len == UBX_NAV_TIMELS_LEN)) {
    sl_max_m10s_ubx_navtimels_process(gnss_cfg_data, msg);
  } else if ((msg->id == UBX_NAV_TIMEUTC)
             && (msg->len == UBX_NAV_TIMEUTC_LEN)) {
    sl_max_m10s_ubx_navtimeutc_process(gnss_cfg_data, msg);
  } else if (msg->id == UBX_NAV_SIG) {
    sl_max_m10s_ubx_navsig_process(gnss_cfg_data, msg);
  } else if ((msg->id == UBX_NAV_POSLLH) && (msg->len == UBX_NAV_POSLLH_LEN)) {
    sl_max_m10s_ubx_navposllh_process(gnss_cfg_data, msg);
  } else if ((msg->id == UBX_NAV_VELNED) && (msg->len == UBX_NAV_VELNED_LEN)) {
    sl_max_m10s_ubx_navvelned_process(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navstatus_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVSTATUS != NULL) {
    gnss_cfg_data->packetUBXNAVSTATUS->data.iTOW = gnss_max_m10s_extract_long(
      msg,
      0);
    gnss_cfg_data->packetUBXNAVSTATUS->data.gps_fix =
      gnss_max_m10s_extract_byte(msg, 4);
    gnss_cfg_data->packetUBXNAVSTATUS->data.flags.all =
      gnss_max_m10s_extract_byte(msg, 5);
    gnss_cfg_data->packetUBXNAVSTATUS->data.fix_status.all =
      gnss_max_m10s_extract_byte(msg, 6);
    gnss_cfg_data->packetUBXNAVSTATUS->data.flags_2.all =
      gnss_max_m10s_extract_byte(msg, 7);
    gnss_cfg_data->packetUBXNAVSTATUS->data.time_to_first_fix =
      gnss_max_m10s_extract_long(msg, 8);
    gnss_cfg_data->packetUBXNAVSTATUS->data.milli_sec_since_startup =
      gnss_max_m10s_extract_long(msg, 12);

    if ((gnss_cfg_data->packetUBXNAVSTATUS->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVSTATUS->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVSTATUS->callback_data->iTOW,
             &gnss_cfg_data->packetUBXNAVSTATUS->data.iTOW,
             sizeof(sl_max_m10s_ubx_nav_status_data_t));

      gnss_cfg_data->packetUBXNAVSTATUS->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVSTATUS->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navdop_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVDOP != NULL) {
    gnss_cfg_data->packetUBXNAVDOP->data.iTOW = gnss_max_m10s_extract_long(msg,
                                                                           0);
    gnss_cfg_data->packetUBXNAVDOP->data.geometric = gnss_max_m10s_extract_int(
      msg,
      4);
    gnss_cfg_data->packetUBXNAVDOP->data.position = gnss_max_m10s_extract_int(
      msg,
      6);
    gnss_cfg_data->packetUBXNAVDOP->data.time =
      gnss_max_m10s_extract_int(msg, 8);
    gnss_cfg_data->packetUBXNAVDOP->data.vertical = gnss_max_m10s_extract_int(
      msg,
      10);
    gnss_cfg_data->packetUBXNAVDOP->data.horizontal = gnss_max_m10s_extract_int(
      msg,
      12);
    gnss_cfg_data->packetUBXNAVDOP->data.north = gnss_max_m10s_extract_int(msg,
                                                                           14);
    gnss_cfg_data->packetUBXNAVDOP->data.east = gnss_max_m10s_extract_int(msg,
                                                                          16);

    if ((gnss_cfg_data->packetUBXNAVDOP->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVDOP->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVDOP->callback_data->iTOW,
             &gnss_cfg_data->packetUBXNAVDOP->data.iTOW,
             sizeof(sl_max_m10s_ubx_nav_dop_data_t));

      gnss_cfg_data->packetUBXNAVDOP->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVDOP->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navclock_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVCLOCK != NULL) {
    gnss_cfg_data->packetUBXNAVCLOCK->data.iTOW = gnss_max_m10s_extract_long(
      msg,
      0);
    gnss_cfg_data->packetUBXNAVCLOCK->data.clock_bias =
      gnss_max_m10s_extract_signed_long(msg, 4);
    gnss_cfg_data->packetUBXNAVCLOCK->data.clock_drift =
      gnss_max_m10s_extract_signed_long(msg, 8);
    gnss_cfg_data->packetUBXNAVCLOCK->data.time_accuracy =
      gnss_max_m10s_extract_long(msg, 12);
    gnss_cfg_data->packetUBXNAVCLOCK->data.freq_accuracy =
      gnss_max_m10s_extract_long(msg, 16);

    if ((gnss_cfg_data->packetUBXNAVCLOCK->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVCLOCK->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVCLOCK->callback_data->iTOW,
             &gnss_cfg_data->packetUBXNAVCLOCK->data.iTOW,
             sizeof(sl_max_m10s_ubx_nav_clock_data_t));

      gnss_cfg_data->packetUBXNAVCLOCK->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVCLOCK->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navepoch_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVEOE != NULL) {
    gnss_cfg_data->packetUBXNAVEOE->data.iTOW = gnss_max_m10s_extract_long(msg,
                                                                           0);
    if ((gnss_cfg_data->packetUBXNAVEOE->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVEOE->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVEOE->callback_data->iTOW,
             &gnss_cfg_data->packetUBXNAVEOE->data.iTOW,
             sizeof(sl_max_m10s_ubx_nav_epoch_data_t));

      gnss_cfg_data->packetUBXNAVEOE->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVEOE->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navsat_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVSAT != NULL) {
    gnss_cfg_data->packetUBXNAVSAT->data.header.iTOW =
      gnss_max_m10s_extract_long(msg, 0);
    gnss_cfg_data->packetUBXNAVSAT->data.header.version =
      gnss_max_m10s_extract_byte(msg, 4);
    gnss_cfg_data->packetUBXNAVSAT->data.header.num_sv =
      gnss_max_m10s_extract_byte(msg, 5);

    /// The NAV SAT message could contain data for 255 SVs max.
    for (uint16_t i = 0; (i < 255)
         && (i < ((uint16_t)gnss_cfg_data->packetUBXNAVSAT->data.header.num_sv))
         && ((i * 12) < (msg->len - 8)); i++) {
      uint16_t offset = (i * 12) + 8;

      gnss_cfg_data->packetUBXNAVSAT->data.blocks[i].gnss_id =
        gnss_max_m10s_extract_byte(msg, offset + 0);
      gnss_cfg_data->packetUBXNAVSAT->data.blocks[i].sv_id =
        gnss_max_m10s_extract_byte(msg, offset + 1);
      gnss_cfg_data->packetUBXNAVSAT->data.blocks[i].carrier_noise_ratio =
        gnss_max_m10s_extract_byte(msg, offset + 2);
      gnss_cfg_data->packetUBXNAVSAT->data.blocks[i].elevation =
        gnss_max_m10s_extract_signed_char(msg, offset + 3);
      gnss_cfg_data->packetUBXNAVSAT->data.blocks[i].azimuth =
        gnss_max_m10s_extract_signed_int(msg, offset + 4);
      gnss_cfg_data->packetUBXNAVSAT->data.blocks[i].pseudo_range =
        gnss_max_m10s_extract_signed_int(msg, offset + 6);
      gnss_cfg_data->packetUBXNAVSAT->data.blocks[i].flags.all =
        gnss_max_m10s_extract_long(msg, offset + 8);
    }

    if ((gnss_cfg_data->packetUBXNAVSAT->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVSAT->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVSAT->callback_data->header.iTOW,
             &gnss_cfg_data->packetUBXNAVSAT->data.header.iTOW,
             sizeof(sl_max_m10s_ubx_navsat_data_t));

      gnss_cfg_data->packetUBXNAVSAT->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVSAT->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navsig_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVSIG != NULL) {
    gnss_cfg_data->packetUBXNAVSIG->data.header.iTOW =
      gnss_max_m10s_extract_long(msg, 0);
    gnss_cfg_data->packetUBXNAVSIG->data.header.version =
      gnss_max_m10s_extract_byte(msg, 4);
    gnss_cfg_data->packetUBXNAVSIG->data.header.num_signals =
      gnss_max_m10s_extract_byte(msg, 5);

    /// The NAV SIG message could contain data for 92 signals max.
    for (uint16_t i = 0; (i < 92)
         && (i
             < ((uint16_t)gnss_cfg_data->packetUBXNAVSIG->data.header.
                num_signals))
         && ((i * 16) < (msg->len - 8)); i++) {
      uint16_t offset = (i * 16) + 8;

      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].gnss_id =
        gnss_max_m10s_extract_byte(msg, offset + 0);
      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].sv_id =
        gnss_max_m10s_extract_byte(msg, offset + 1);
      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].sig_id =
        gnss_max_m10s_extract_byte(msg, offset + 2);
      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].freq_id =
        gnss_max_m10s_extract_byte(msg, offset + 3);
      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].psuedo_range =
        gnss_max_m10s_extract_signed_int(msg, offset + 4);
      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].carrier_noise_ratio =
        gnss_max_m10s_extract_byte(msg, offset + 6);
      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].quality_ind =
        gnss_max_m10s_extract_byte(msg, offset + 7);
      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].corr_source =
        gnss_max_m10s_extract_byte(msg, offset + 8);
      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].iono_model =
        gnss_max_m10s_extract_byte(msg, offset + 9);
      gnss_cfg_data->packetUBXNAVSIG->data.blocks[i].sig_flags.all =
        gnss_max_m10s_extract_int(msg, offset + 10);
    }

    if ((gnss_cfg_data->packetUBXNAVSIG->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVSIG->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVSIG->callback_data->header.iTOW,
             &gnss_cfg_data->packetUBXNAVSIG->data.header.iTOW,
             sizeof(sl_max_m10s_ubx_navsig_data_t));

      gnss_cfg_data->packetUBXNAVSIG->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVSIG->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navtimels_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVTIMELS != NULL) {
    gnss_cfg_data->packetUBXNAVTIMELS->data.iTOW = gnss_max_m10s_extract_long(
      msg,
      0);
    gnss_cfg_data->packetUBXNAVTIMELS->data.version =
      gnss_max_m10s_extract_byte(msg, 4);
    gnss_cfg_data->packetUBXNAVTIMELS->data.src_of_current_leap =
      gnss_max_m10s_extract_byte(msg, 8);
    gnss_cfg_data->packetUBXNAVTIMELS->data.current_leap_sec =
      gnss_max_m10s_extract_signed_char(msg, 9);
    gnss_cfg_data->packetUBXNAVTIMELS->data.src_of_leap_sec_change =
      gnss_max_m10s_extract_byte(msg, 10);
    gnss_cfg_data->packetUBXNAVTIMELS->data.leap_sec_change =
      gnss_max_m10s_extract_signed_char(msg, 11);
    gnss_cfg_data->packetUBXNAVTIMELS->data.time_to_leap_sec_event =
      gnss_max_m10s_extract_signed_long(msg, 12);
    gnss_cfg_data->packetUBXNAVTIMELS->data.date_of_leap_sec_gps_week =
      gnss_max_m10s_extract_int(msg, 16);
    gnss_cfg_data->packetUBXNAVTIMELS->data.day_of_leap_sec_gps =
      gnss_max_m10s_extract_int(msg, 18);
    gnss_cfg_data->packetUBXNAVTIMELS->data.valid.all =
      gnss_max_m10s_extract_signed_char(msg, 23);

    if ((gnss_cfg_data->packetUBXNAVTIMELS->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVTIMELS->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVTIMELS->callback_data->iTOW,
             &gnss_cfg_data->packetUBXNAVTIMELS->data.iTOW,
             sizeof(sl_max_m10s_ubx_nav_timels_data_t));

      gnss_cfg_data->packetUBXNAVTIMELS->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVTIMELS->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navtimeutc_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVTIMEUTC != NULL) {
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.iTOW = gnss_max_m10s_extract_long(
      msg,
      0);
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.time_accuracy =
      gnss_max_m10s_extract_long(msg, 4);
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.nano =
      gnss_max_m10s_extract_signed_long(msg, 8);
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.year = gnss_max_m10s_extract_int(
      msg,
      12);
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.month = gnss_max_m10s_extract_byte(
      msg,
      14);
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.day = gnss_max_m10s_extract_byte(
      msg,
      15);
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.hour = gnss_max_m10s_extract_byte(
      msg,
      16);
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.min = gnss_max_m10s_extract_byte(
      msg,
      17);
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.sec = gnss_max_m10s_extract_byte(
      msg,
      18);
    gnss_cfg_data->packetUBXNAVTIMEUTC->data.valid.all =
      gnss_max_m10s_extract_byte(msg, 19);

    if ((gnss_cfg_data->packetUBXNAVTIMEUTC->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVTIMEUTC->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVTIMEUTC->callback_data->iTOW,
             &gnss_cfg_data->packetUBXNAVTIMEUTC->data.iTOW,
             sizeof(sl_max_m10s_ubx_nav_timeutc_data_t));

      gnss_cfg_data->packetUBXNAVTIMEUTC->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVTIMEUTC->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navposllh_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVPOSLLH != NULL) {
    gnss_cfg_data->packetUBXNAVPOSLLH->data.iTOW = gnss_max_m10s_extract_long(
      msg,
      0);
    gnss_cfg_data->packetUBXNAVPOSLLH->data.lon =
      gnss_max_m10s_extract_signed_long(msg, 4);
    gnss_cfg_data->packetUBXNAVPOSLLH->data.lat =
      gnss_max_m10s_extract_signed_long(msg, 8);
    gnss_cfg_data->packetUBXNAVPOSLLH->data.height =
      gnss_max_m10s_extract_signed_long(msg, 12);
    gnss_cfg_data->packetUBXNAVPOSLLH->data.hMSL =
      gnss_max_m10s_extract_signed_long(msg, 16);
    gnss_cfg_data->packetUBXNAVPOSLLH->data.h_acc = gnss_max_m10s_extract_long(
      msg,
      20);
    gnss_cfg_data->packetUBXNAVPOSLLH->data.v_acc = gnss_max_m10s_extract_long(
      msg,
      24);

    if ((gnss_cfg_data->packetUBXNAVPOSLLH->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVPOSLLH->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVPOSLLH->callback_data->iTOW,
             &gnss_cfg_data->packetUBXNAVPOSLLH->data.iTOW,
             sizeof(sl_max_m10s_ubx_nav_posllh_data_t));

      gnss_cfg_data->packetUBXNAVPOSLLH->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVPOSLLH->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navvelned_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVVELNED != NULL) {
    gnss_cfg_data->packetUBXNAVVELNED->data.iTOW = gnss_max_m10s_extract_long(
      msg,
      0);
    gnss_cfg_data->packetUBXNAVVELNED->data.velN =
      gnss_max_m10s_extract_signed_long(msg, 4);
    gnss_cfg_data->packetUBXNAVVELNED->data.velE =
      gnss_max_m10s_extract_signed_long(msg, 8);
    gnss_cfg_data->packetUBXNAVVELNED->data.velD =
      gnss_max_m10s_extract_signed_long(msg, 12);
    gnss_cfg_data->packetUBXNAVVELNED->data.speed = gnss_max_m10s_extract_long(
      msg,
      16);
    gnss_cfg_data->packetUBXNAVVELNED->data.ground_speed =
      gnss_max_m10s_extract_long(msg, 20);
    gnss_cfg_data->packetUBXNAVVELNED->data.heading =
      gnss_max_m10s_extract_signed_long(msg, 24);
    gnss_cfg_data->packetUBXNAVVELNED->data.speed_acc =
      gnss_max_m10s_extract_long(msg, 28);
    gnss_cfg_data->packetUBXNAVVELNED->data.course_acc =
      gnss_max_m10s_extract_long(msg, 32);

    if ((gnss_cfg_data->packetUBXNAVVELNED->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVVELNED->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVVELNED->callback_data->iTOW,
             &gnss_cfg_data->packetUBXNAVVELNED->data.iTOW,
             sizeof(sl_max_m10s_ubx_nav_posllh_data_t));

      gnss_cfg_data->packetUBXNAVVELNED->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVVELNED->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

void sl_max_m10s_ubx_navpvt_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    sl_max_m10s_ubx_packet_t *msg)
{
  bool added_to_file_buffer = false;

  if (gnss_cfg_data->packetUBXNAVPVT != NULL) {
    gnss_cfg_data->packetUBXNAVPVT->data.fixType = gnss_max_m10s_extract_byte(
      msg,
      20);
    gnss_cfg_data->packetUBXNAVPVT->data.numSV = gnss_max_m10s_extract_byte(msg,
                                                                            23);
    gnss_cfg_data->packetUBXNAVPVT->data.lon =
      gnss_max_m10s_extract_signed_long(msg, 24);
    gnss_cfg_data->packetUBXNAVPVT->data.lat =
      gnss_max_m10s_extract_signed_long(msg, 28);
    gnss_cfg_data->packetUBXNAVPVT->data.height =
      gnss_max_m10s_extract_signed_long(msg, 32);
    gnss_cfg_data->packetUBXNAVPVT->data.hMSL =
      gnss_max_m10s_extract_signed_long(msg, 36);
    gnss_cfg_data->packetUBXNAVPVT->data.hour = gnss_max_m10s_extract_byte(msg,
                                                                           8);
    gnss_cfg_data->packetUBXNAVPVT->data.min =
      gnss_max_m10s_extract_byte(msg, 9);
    gnss_cfg_data->packetUBXNAVPVT->data.sec = gnss_max_m10s_extract_byte(msg,
                                                                          10);
    gnss_cfg_data->packetUBXNAVPVT->data.nano =
      gnss_max_m10s_extract_signed_long(msg, 16);
    gnss_cfg_data->packetUBXNAVPVT->data.tAcc = gnss_max_m10s_extract_byte(msg,
                                                                           12);
    gnss_cfg_data->packetUBXNAVPVT->data.year =
      gnss_max_m10s_extract_int(msg, 4);
    gnss_cfg_data->packetUBXNAVPVT->data.month = gnss_max_m10s_extract_byte(msg,
                                                                            6);
    gnss_cfg_data->packetUBXNAVPVT->data.day =
      gnss_max_m10s_extract_byte(msg, 7);
    gnss_cfg_data->packetUBXNAVPVT->data.hAcc = gnss_max_m10s_extract_long(msg,
                                                                           40);
    gnss_cfg_data->packetUBXNAVPVT->data.vAcc = gnss_max_m10s_extract_long(msg,
                                                                           42);
    gnss_cfg_data->packetUBXNAVPVT->data.sAcc = gnss_max_m10s_extract_long(msg,
                                                                           68);
    gnss_cfg_data->packetUBXNAVPVT->data.headAcc = gnss_max_m10s_extract_long(
      msg,
      72);
    gnss_cfg_data->packetUBXNAVPVT->data.velN =
      gnss_max_m10s_extract_signed_long(msg, 48);
    gnss_cfg_data->packetUBXNAVPVT->data.velE =
      gnss_max_m10s_extract_signed_long(msg, 52);
    gnss_cfg_data->packetUBXNAVPVT->data.velD =
      gnss_max_m10s_extract_signed_long(msg, 56);
    gnss_cfg_data->packetUBXNAVPVT->data.gSpeed =
      gnss_max_m10s_extract_signed_long(msg, 60);
    gnss_cfg_data->packetUBXNAVPVT->data.headMot =
      gnss_max_m10s_extract_signed_long(msg, 68);
    gnss_cfg_data->packetUBXNAVPVT->data.headVeh =
      gnss_max_m10s_extract_signed_long(msg, 84);
    gnss_cfg_data->packetUBXNAVPVT->data.magDec =
      gnss_max_m10s_extract_signed_int(msg, 88);
    gnss_cfg_data->packetUBXNAVPVT->data.magAcc = gnss_max_m10s_extract_int(msg,
                                                                            90);
    gnss_cfg_data->packetUBXNAVPVT->data.pDOP = gnss_max_m10s_extract_int(msg,
                                                                          76);
    gnss_cfg_data->packetUBXNAVPVT->data.valid.all = gnss_max_m10s_extract_byte(
      msg,
      11);
    gnss_cfg_data->packetUBXNAVPVT->data.confirm_flags.all =
      gnss_max_m10s_extract_byte(msg, 22);
    gnss_cfg_data->packetUBXNAVPVT->data.solution_flags.all =
      gnss_max_m10s_extract_byte(msg, 21);
    gnss_cfg_data->packetUBXNAVPVT->data.correction_flags.all =
      gnss_max_m10s_extract_byte(msg, 78);

    if ((gnss_cfg_data->packetUBXNAVPVT->callback_data != NULL)
        && (gnss_cfg_data->packetUBXNAVPVT->automatic_flags.flags.bits.
            callbackCopyValid == false)) {
      memcpy(&gnss_cfg_data->packetUBXNAVPVT->callback_data->iTOW,
             &gnss_cfg_data->packetUBXNAVPVT->data.iTOW,
             sizeof(sl_max_m10s_ubx_nav_pvt_data_t));

      gnss_cfg_data->packetUBXNAVPVT->automatic_flags.flags.bits.
      callbackCopyValid = true;
    }

    if (gnss_cfg_data->packetUBXNAVPVT->automatic_flags.flags.bits.
        addToFileBuffer) {
      added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
    }
  }

  if (!added_to_file_buffer) {
    added_to_file_buffer = gnss_max_m10s_store_packet(gnss_cfg_data, msg);
  }
}

sl_status_t gnss_max_m10s_get_unique_id(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait)
{
  gnss_cfg_data->packet_cfg.cls = UBX_CLASS_SEC;
  gnss_cfg_data->packet_cfg.id = UBX_SEC_UNIQID;
  gnss_cfg_data->packet_cfg.len = 0;
  gnss_cfg_data->packet_cfg.starting_spot = 0;

  sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(gnss_cfg_data,
                                                                  max_wait,
                                                                  false);

  if (ret_val != SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
    return SL_STATUS_FAIL;
  }

  /// Extract the data
  gnss_cfg_data->packetUBXUNIQID->version =
    gnss_max_m10s_extract_byte(&(gnss_cfg_data->packet_cfg),
                               0);

  /// For M10s version the unique id is 6 bytes, for F9 and M9 it will be 5 bytes.
  for (uint8_t i = 0; i < 6; i++) {
    gnss_cfg_data->packetUBXUNIQID->unique_id[i] =
      gnss_max_m10s_extract_byte(&(gnss_cfg_data->packet_cfg),
                                 i
                                 + 4);
  }

  return SL_STATUS_OK;
}

sl_status_t gnss_max_m10s_get_navpvt_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVPVT == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavpvt(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_pvt(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navepoch_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVEOE == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavepoch(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_epoch(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navtimels_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVTIMELS == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavtimels(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_timels(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navposllh_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVPOSLLH == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavposllh(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_posllh(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navtimeutc_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVTIMEUTC == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavtimeutc(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_timeutc(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navstatus_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVSTATUS == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavstatus(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_status(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navsat_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVSAT == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavsat(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_sat(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navsig_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVSIG == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavsig(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_sig(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navdop_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVDOP == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavdop(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_dop(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navclock_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVCLOCK == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavclock(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_clock(gnss_cfg_data, max_wait);

  return status;
}

sl_status_t gnss_max_m10s_get_navvelned_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVVELNED == NULL) {
    status = gnss_max_m10s_init_packet_ubxnavvelned(gnss_cfg_data);

    if (status != SL_STATUS_OK) {
      return status;
    }
  }

  status = gnss_max_m10s_get_nav_velned(gnss_cfg_data, max_wait);

  return status;
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavpvt(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVPVT =
    (sl_max_m10s_ubx_nav_pvt_t *)malloc(sizeof(sl_max_m10s_ubx_nav_pvt_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVPVT == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVPVT->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVPVT->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVPVT->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_get_pvt(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVPVT == NULL) {
    gnss_max_m10s_init_packet_ubxnavpvt(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVPVT == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVPVT->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVPVT->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVPVT->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVPVT->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_PVT;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavtimels(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVTIMELS = (sl_max_m10s_ubx_nav_timels_t *)malloc(
    sizeof(sl_max_m10s_ubx_nav_timels_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVTIMELS == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVTIMELS->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVTIMELS->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVTIMELS->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavposllh(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVPOSLLH = (sl_max_m10s_ubx_nav_posllh_t *)malloc(
    sizeof(sl_max_m10s_ubx_nav_posllh_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVPOSLLH == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVPOSLLH->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVPOSLLH->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVPOSLLH->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_get_nav_posllh(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVPOSLLH == NULL) {
    gnss_max_m10s_init_packet_ubxnavposllh(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVPOSLLH == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVPOSLLH->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVPOSLLH->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVPOSLLH->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVPOSLLH->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_POSLLH;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_get_nav_timels(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVTIMELS == NULL) {
    gnss_max_m10s_init_packet_ubxnavtimels(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVTIMELS == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVTIMELS->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVTIMELS->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVTIMELS->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVTIMELS->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_TIMELS;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavtimeutc(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVTIMEUTC = (sl_max_m10s_ubx_nav_timeutc_t *)malloc(
    sizeof(sl_max_m10s_ubx_nav_timeutc_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVTIMEUTC == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVTIMEUTC->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVTIMEUTC->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVTIMEUTC->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_get_nav_timeutc(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVTIMEUTC == NULL) {
    gnss_max_m10s_init_packet_ubxnavtimels(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVTIMEUTC == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVTIMEUTC->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVTIMEUTC->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVTIMEUTC->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVTIMEUTC->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_TIMEUTC;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavstatus(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVSTATUS = (sl_max_m10s_ubx_nav_status_t *)malloc(
    sizeof(sl_max_m10s_ubx_nav_status_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVSTATUS == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVSTATUS->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVSTATUS->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVSTATUS->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavsat(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVSAT =
    (sl_max_m10s_ubx_nav_sat_t *)malloc(sizeof(sl_max_m10s_ubx_nav_sat_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVSAT == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVSAT->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVSAT->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVSAT->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavsig(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVSIG =
    (sl_max_m10s_ubx_navsig_t *)malloc(sizeof(sl_max_m10s_ubx_navsig_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVSIG == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVSIG->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVSIG->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVSIG->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavdop(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVDOP =
    (sl_max_m10s_ubx_nav_dop_t *)malloc(sizeof(sl_max_m10s_ubx_nav_dop_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVDOP == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVDOP->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVDOP->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVDOP->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_get_nav_dop(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVDOP == NULL) {
    gnss_max_m10s_init_packet_ubxnavdop(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVDOP == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVDOP->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVDOP->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVDOP->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVDOP->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_DOP;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavclock(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVCLOCK = (sl_max_m10s_ubx_nav_clock_t *)malloc(
    sizeof(sl_max_m10s_ubx_nav_clock_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVCLOCK == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVCLOCK->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVCLOCK->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVCLOCK->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_get_nav_clock(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVCLOCK == NULL) {
    gnss_max_m10s_init_packet_ubxnavclock(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVCLOCK == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVCLOCK->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVCLOCK->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVCLOCK->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVCLOCK->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_CLOCK;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavvelned(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVVELNED = (sl_max_m10s_ubx_nav_velned_t *)malloc(
    sizeof(sl_max_m10s_ubx_nav_velned_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVVELNED == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVVELNED->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVVELNED->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVVELNED->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_get_nav_velned(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVVELNED == NULL) {
    gnss_max_m10s_init_packet_ubxnavclock(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVVELNED == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVVELNED->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVVELNED->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVVELNED->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVVELNED->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_VELNED;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_init_packet_ubxnavepoch(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packetUBXNAVEOE =
    (sl_max_m10s_ubx_nav_epoch_t *)malloc(sizeof(sl_max_m10s_ubx_nav_epoch_t)); /// Allocate RAM for the main struct

  if (gnss_cfg_data->packetUBXNAVEOE == NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->packetUBXNAVEOE->automatic_flags.flags.all = 0;
  gnss_cfg_data->packetUBXNAVEOE->callback_pointer_ptr = NULL;
  gnss_cfg_data->packetUBXNAVEOE->callback_data = NULL;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_get_nav_epoch(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVEOE == NULL) {
    gnss_max_m10s_init_packet_ubxnavepoch(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVEOE == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVEOE->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVEOE->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVEOE->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVEOE->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_EOE;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_get_nav_status(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVSTATUS == NULL) {
    gnss_max_m10s_init_packet_ubxnavstatus(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVSTATUS == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVSTATUS->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVSTATUS->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVSTATUS->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVSTATUS->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_STATUS;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_get_nav_sat(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVSAT == NULL) {
    gnss_max_m10s_init_packet_ubxnavsat(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVSAT == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVSAT->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVSAT->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVSAT->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVSAT->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_SAT;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static sl_status_t gnss_max_m10s_get_nav_sig(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  if (gnss_cfg_data->packetUBXNAVSIG == NULL) {
    gnss_max_m10s_init_packet_ubxnavsig(gnss_cfg_data);
  }

  if (gnss_cfg_data->packetUBXNAVSIG == NULL) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->packetUBXNAVSIG->automatic_flags.flags.bits.automatic
      && gnss_cfg_data->packetUBXNAVSIG->automatic_flags.flags.bits.
      implicitUpdate) {
    return gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                              &(gnss_cfg_data->packet_cfg), 0,
                                              0);
  } else if (gnss_cfg_data->packetUBXNAVSIG->automatic_flags.flags.bits.
             automatic
             && !gnss_cfg_data->packetUBXNAVSIG->automatic_flags.flags.bits.
             implicitUpdate) {
    return SL_STATUS_FAIL;
  } else {
    gnss_cfg_data->packet_cfg.cls = UBX_CLASS_NAV;
    gnss_cfg_data->packet_cfg.id = UBX_NAV_SIG;
    gnss_cfg_data->packet_cfg.len = 0;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

    sl_max_m10s_ublox_status_e ret_val = gnss_max_m10s_send_command(
      gnss_cfg_data,
      max_wait,
      false);

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED) {
      return SL_STATUS_OK;
    }

    if (ret_val == SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN) {
      return SL_STATUS_OK;
    }

    return SL_STATUS_FAIL;
  }
}

static uint32_t gnss_max_m10s_extract_long(sl_max_m10s_ubx_packet_t *msg,
                                           uint16_t spot_to_start)
{
  uint32_t val = 0;

  for (uint8_t i = 0; i < 4; i++) {
    val |= (uint32_t)msg->payload[spot_to_start + i] << (8 * i);
  }

  return val;
}

int8_t gnss_max_m10s_extract_signed_char(sl_max_m10s_ubx_packet_t *msg,
                                         uint16_t spot_to_start)
{
  sl_max_m10s_unsigned_signed_8_t converter;
  converter.unsigned8 = gnss_max_m10s_extract_byte(msg, spot_to_start);
  return (converter.signed8);
}

static int32_t gnss_max_m10s_extract_signed_long(sl_max_m10s_ubx_packet_t *msg,
                                                 uint16_t spot_to_start)
{
  sl_max_m10s_unsigned_signed_32_t converter;

  converter.unsigned32 = gnss_max_m10s_extract_long(msg, spot_to_start);

  return converter.signed32;
}

static uint16_t gnss_max_m10s_extract_int(sl_max_m10s_ubx_packet_t *msg,
                                          uint16_t spot_to_start)
{
  uint16_t val = (uint16_t)msg->payload[spot_to_start + 0] << 8 * 0;

  val |= (uint16_t)msg->payload[spot_to_start + 1] << 8 * 1;

  return val;
}

static int16_t gnss_max_m10s_extract_signed_int(sl_max_m10s_ubx_packet_t *msg,
                                                uint16_t spot_to_start)
{
  sl_max_m10s_unsigned_signed_16_t converter;

  converter.unsigned16 = gnss_max_m10s_extract_int(msg, spot_to_start);

  return converter.signed16;
}

uint8_t gnss_max_m10s_extract_byte(sl_max_m10s_ubx_packet_t *msg,
                                   uint16_t spot_to_start)
{
  return (msg->payload[spot_to_start]);
}
