/***************************************************************************//**
 * @file gnss_max_m10s_nmea.h
 * @brief MAX_M10S GNSS receiver driver external APIs for NMEA sentence data
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

#ifndef GNSS_MAX_M10S_NMEA_H_
#define GNSS_MAX_M10S_NMEA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gnss_max_m10s_driver.h"
#include "gnss_max_m10s_micro_nmea.h"
#include "gnss_max_m10s_nmea_struct.h"

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
 * @brief To initialize the NMEA buffer and NMEA data structure.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return the following values are returned:
 *         true for success.
 *         false for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_nmea_init(sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief To check if the header of the NMEA sentence is valid.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return the following values are returned:
 *         true for success.
 *         false for failure.
 *****************************************************************************/
bool gnss_max_m10s_is_nmea_header_valid(sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief To check if we should pass this NMEA message
 *        to process from micro nmea library.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return true for success and fail for failure.
 *****************************************************************************/
bool gnss_max_m10s_process_current_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief  Get a pointer to the automatic NMEA flags.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return pointer to the automatic NMEA flags.
 *****************************************************************************/
sl_max_m10s_nmea_automatic_flags_t * gnss_max_m10s_get_nmea_flags_ptr(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief  Get a pointer to the complete copy NMEA data.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return pointer to the complete copy of the NMEA data structure.
 *****************************************************************************/
uint8_t * gnss_max_m10s_get_complete_copy_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief  Get a pointer to the complete copy length.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return pointer to the complete copy length of the NMEA data structure.
 ****************************************************************************/
uint8_t * gnss_max_m10s_get_nmea_complete_length_ptr(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief  Get the maximum length of given NMEA message.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return the maximum length of the NMEA message.
 ****************************************************************************/
uint8_t gnss_max_m10s_get_nmea_max_length(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief  Get a pointer to the working copy NMEA data.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return pointer to working copy of NMEA data.
 ****************************************************************************/
uint8_t * gnss_max_m10s_get_nmea_working_ptr(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief  Get a pointer to the working copy length of NMEA data.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return pointer to the working copy length of NMEA data.
 ****************************************************************************/
uint8_t * gnss_max_m10s_get_nmea_working_length_ptr(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief  Check if the NMEA message is "auto" and has dedicated RAM allocated for it.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_is_nmea_auto(sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief  Get the latitude value from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] latitude : value of latitude in degree minutes format.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_latitude_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  int32_t *latitude,
  uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the longitude value from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] longitude : value of longitude in degree minutes format.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_longitude_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  int32_t *longitude,
  uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the altitude from mean sea level value from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] altitude_msl : value of altitude from mean sea level in mm.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_altitude_msl_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  int32_t *altitude_msl,
  uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the geoid separation from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] geoid : geoid separation in mm.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_geoid_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         int32_t *geoid,
                                         uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the quality of fix from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] quality : value of quality of fix.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_quality_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t *quality,
  uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the number of satellites from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] num_sat : number of satellites.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_satellite_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t *num_sat,
  uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the current day from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] day : current day.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_day_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint8_t *day,
                                       uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the current month value from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] month : current month.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_month_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint8_t *month,
                                         uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the current year from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] year : current year.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_year_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t *year,
                                        uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the hour in UTC from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] hour : current hour in UTC format.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_hour_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint8_t *hour,
                                        uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the current minute of hour from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] min : minute of hour in UTC format.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_min_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint8_t *min,
                                       uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the current second in UTC format.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] second : value of current second in UTC format.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_second_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                          uint8_t *second,
                                          uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the horizontal dilution of precision value from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] hdop : value of horizontal dilution of precision.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_horizontal_dilution_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t *hdop,
  uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the speed over ground value from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] speed : value of speed over ground in knots.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_speed_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         int32_t *speed,
                                         uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the course over ground value from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] course : value of course over ground in degree.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_ground_course_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  int32_t *course,
  uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the horizontal dilution of precision value from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] hdop : value of horizontal dilution of precision.
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_horizontal_dilution_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t *hdop,
  uint16_t max_time);

/**************************************************************************//**
 * @brief  Get the navigation system from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[out] nav_system : value of horizontal dilution of precision.
 * @details `N` = GNSS, `P` = GPS, `L` = GLONASS, `A` = Galileo, `\0` = none
 * @param[in] max_time : max timeout value.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_nav_system_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  char *nav_system,
  uint16_t max_time);

#ifdef __cplusplus
}
#endif

#endif // GNSS_MAX_M10S_NMEA_H_

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
