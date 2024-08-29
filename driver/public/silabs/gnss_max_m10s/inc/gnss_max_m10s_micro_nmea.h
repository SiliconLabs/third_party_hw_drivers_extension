/***************************************************************************//**
 * @file gnss_max_m10s_micro_nmea.h
 * @brief MAX_M10S GNSS receiver driver micro-NMEA related external APIs.
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

#ifndef GNSS_MAX_M10S_MICRO_NMEA_H_
#define GNSS_MAX_M10S_MICRO_NMEA_H_

#ifdef __cplusplus
extern "C" {
#endif

/// Includes
#include <limits.h>
#include "gnss_max_m10s_nmea_struct.h"
#include "gnss_max_m10s_nmea.h"

/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/

/*******************************************************************************
 *************************   ENUMS and Structures  *****************************
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Prototypes

/**************************************************************************//**
 * @brief initializes buffer allocated to micro nmea library.
 * @param[in] gnss_cfg_data : pointer to the GNSS cfg data structure.
 *****************************************************************************/
void gnss_max_m10s_init_micro_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Clear all information related to NMEA data.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 *****************************************************************************/
void gnss_max_m10s_clear_nmea_data(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the horizontal dilution of precision.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return horizontal dilution of precision value.
 *****************************************************************************/
uint8_t gnss_max_m10s_nmea_get_hdop(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief check if latest fix is valid.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return true if valid.
 *         false if not valid.
 *****************************************************************************/
bool gnss_max_m10s_nmea_is_valid(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the navigation system in use.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return navigation system in use.
 *****************************************************************************/
char gnss_max_m10s_nmea_get_nav_system(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the number of satellites from NMEA sentence.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return number of satellites.
 *****************************************************************************/
uint8_t gnss_max_m10s_nmea_get_num_satellites(
  sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the latitude from NMEA sentence.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return current latitude in millionth of degree.
 *****************************************************************************/
int32_t gnss_max_m10s_nmea_get_latitude(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the longitude from NMEA sentence.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return current longitude in millionth of degree.
 *****************************************************************************/
int32_t gnss_max_m10s_nmea_get_longitude(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the altitude from mean sea level from NMEA sentence.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @param[out] alt : pointer to value of altitude in mm.
 * @return true altitude is valid
 *         false altitude not valid
 *****************************************************************************/
bool gnss_max_m10s_nmea_get_altitude_msl(int32_t *alt,
                                         sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the height above WGS84 Geoid in mm.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @param[out] alt : pointer to value of altitude in mm.
 * @return true altitude is valid
 *         false altitude not valid
 *****************************************************************************/
bool gnss_max_m10s_nmea_get_geoid_height(int32_t *alt,
                                         sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the current year.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return current year in UTC format.
 *****************************************************************************/
uint16_t gnss_max_m10s_nmea_get_year(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the current month of year.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return current month of year in UTC format.
 *****************************************************************************/
uint8_t gnss_max_m10s_nmea_get_month(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the current day.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return current day in UTC format.
 *****************************************************************************/
uint8_t gnss_max_m10s_nmea_get_day(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the current hour.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return current hour in UTC format.
 *****************************************************************************/
uint8_t gnss_max_m10s_nmea_get_hour(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the current minute.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return current minute in UTC format.
 *****************************************************************************/
uint8_t gnss_max_m10s_nmea_get_minute(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the current second.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return current second in UTC format.
 *****************************************************************************/
uint8_t gnss_max_m10s_nmea_get_second(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the speed over ground.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return speed over ground in knots.
 *****************************************************************************/
int32_t gnss_max_m10s_nmea_get_speed(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief Get the course over ground.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @return course over ground in degrees.
 *****************************************************************************/
int32_t gnss_max_m10s_nmea_get_course(sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief To process a character.
 * @param[in] nmea_data : pointer to the NMEA data structure.
 * @param[in] c : Character to process
 * @return true A complete non-empty sentence has been processed
 *         false End of sentence not detected
 *****************************************************************************/
bool gnss_max_m10s_process_nmea(char c, sl_max_m10s_nmea_data_t *nmea_data);

#ifdef __cplusplus
}
#endif

#endif // GNSS_MAX_M10S_MICRO_NMEA_H_

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
