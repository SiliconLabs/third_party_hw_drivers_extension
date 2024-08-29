/***************************************************************************//**
 * @file gnss_max_m10s_ubx.h
 * @brief MAX_M10S GNSS receiver driver external APIs for UBX sentence data.
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

#ifndef GNSS_MAX_M10S_UBX_H_
#define GNSS_MAX_M10S_UBX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gnss_max_m10s_driver.h"

/******************************************************************************/

/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Prototypes

/**************************************************************************//**
 * @brief To process all the data from all UBX packets.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void gnss_max_m10s_process_ubx_packet(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_STATUS packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navstatus_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_DOP packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navdop_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_CLOCK packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navclock_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_EOE packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navepoch_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_SAT packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navsat_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_SIG packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navsig_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_TIMELS packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navtimels_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_TIMEUTC packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navtimeutc_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_POSLLH packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navposllh_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_VELNED packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navvelned_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To process all the data from the UBX_NAV_PVT packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_ubx_navpvt_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief Get the unique ID from UBX_SEC_UNIQID packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] msg : pointer to the packet payload.
 *****************************************************************************/
void sl_max_m10s_get_unique_id(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                               sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief To get all the data from the UBX_NAV_PVT packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navpvt_data(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                          uint16_t max_wait);

/**************************************************************************//**
 * @brief Given a spot, extract a byte from the payload
 * @param[in] msg : pointer to the payload
 * @param[in] spot_to_start : starting spot for the extraction.
 * @return byte extracted from the payload.
 *****************************************************************************/
uint8_t gnss_max_m10s_extract_byte(sl_max_m10s_ubx_packet_t *msg,
                                   uint16_t spot_to_start);

/**************************************************************************//**
 * @brief Given a character, file it away into the ubx packet structure and
 *        set valid to VALID or NOT_VALID once sentence is completely received
 *        and passes or fails CRC
 * @param[in] gnss_cfg_data : pointer to the structure containing
 *                            GNSS configuration data.
 * @param[in] incoming : character to check
 * @param[in] requested_class : class of the UBX packet.
 * @param[in] requested_id : ID for the UBX packet.
 *****************************************************************************/
void gnss_max_m10s_process_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                               uint8_t incoming,
                               uint8_t requested_class,
                               uint8_t requested_id);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_PVT packet and to get all the data
 *        from the UBX_NAV_PVT packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navpvt_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_EOE packet and to get all the data
 *        from the UBX_NAV_EOE packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navepoch_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_SAT packet and to get all the data
 *        from the UBX_NAV_SAT packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navsat_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_SIG packet and to get all the data
 *        from the UBX_NAV_SIG packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navsig_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_STATUS packet and to get all the data
 *        from the UBX_NAV_STATUS packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navstatus_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_TIMELS packet and to get all the data
 *        from the UBX_NAV_TIMELS packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navtimels_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_TIMEUTC packet and to get all the data
 *        from the UBX_NAV_TIMEUTC packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navtimeutc_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_DOP packet and to get all the data
 *        from the UBX_NAV_DOP packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navdop_ubx(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_POSLLH packet and to get all the data
 *        from the UBX_NAV_POSLLH packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navposllh_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_CLOCK packet and to get all the data
 *        from the UBX_NAV_CLOCK packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navclock_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief To Initialize the UBX_NAV_VELNED packet and to get all the data
 *        from the UBX_NAV_VELNED packet.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navvelned_ubx(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

#ifdef __cplusplus
}
#endif

#endif // GNSS_MAX_M10S_UBX_H_

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
