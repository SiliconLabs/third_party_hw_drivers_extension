/****************************************************************************//*
 * @file gnss_max_m10s_nmea.c
 * @brief MAX_M10S GNSS receiver driver external APIs implementation for NMEA
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
#include "gnss_max_m10s_driver.h"
#include "gnss_max_m10s_nmea.h"

sl_status_t gnss_max_m10s_nmea_init(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->nmea_data = malloc(sizeof(sl_max_m10s_nmea_data_t));

  if (gnss_cfg_data->nmea_data != NULL) {
    memset(gnss_cfg_data->nmea_data, 0, sizeof(sl_max_m10s_nmea_data_t));
    gnss_cfg_data->processing_nmea.all = SL_MAX_M10S_FILTER_NMEA_ALL;
    gnss_max_m10s_init_micro_nmea(gnss_cfg_data);
    return SL_STATUS_OK;
  }

  return SL_STATUS_FAIL;
}

bool gnss_max_m10s_is_nmea_header_valid(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  if (gnss_cfg_data->nmea_address_field[0] != '*') {
    return (false);
  }
  if (gnss_cfg_data->nmea_address_field[1] != 'G') {
    return (false);
  }

  if (gnss_cfg_data->nmea_address_field[3] == 'G') {
    if ((gnss_cfg_data->nmea_address_field[4] == 'G')
        && (gnss_cfg_data->nmea_address_field[5] == 'A')) {
      return (true);
    }
  }

  if ((gnss_cfg_data->nmea_address_field[3] == 'R')
      && (gnss_cfg_data->nmea_address_field[4] == 'M')
      && (gnss_cfg_data->nmea_address_field[5] == 'C')) {
    return (true);
  }

  return (false);
}

bool gnss_max_m10s_process_current_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  if (gnss_cfg_data->processing_nmea.bits.all == 1) {
    return (true);
  }

  if (gnss_cfg_data->nmea_address_field[3] == 'G') {
    if ((gnss_cfg_data->nmea_address_field[4] == 'G')
        && (gnss_cfg_data->nmea_address_field[5] == 'A')
        && (gnss_cfg_data->processing_nmea.bits.UBX_NMEA_GGA == 1)) {
      return (true);
    }
  }

  if ((gnss_cfg_data->nmea_address_field[3] == 'R')
      && (gnss_cfg_data->nmea_address_field[4] == 'M')
      && (gnss_cfg_data->nmea_address_field[5] == 'C')
      && (gnss_cfg_data->processing_nmea.bits.UBX_NMEA_RMC == 1)) {
    return (true);
  }

  return (false);
}

uint8_t gnss_max_m10s_get_nmea_max_length(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  char nmea[] = "GPGGA";
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return SL_MAX_M10S_NMEA_GGA_MAX_LENGTH;
  }

  strcpy(nmea, "GNGGA");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return SL_MAX_M10S_NMEA_GGA_MAX_LENGTH;
  }

  strcpy(nmea, "GPRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return SL_MAX_M10S_NMEA_RMC_MAX_LENGTH;
  }

  strcpy(nmea, "GNRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return SL_MAX_M10S_NMEA_RMC_MAX_LENGTH;
  }

  return 0;
}

sl_max_m10s_nmea_automatic_flags_t * gnss_max_m10s_get_nmea_flags_ptr(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  char nmea[] = "GPGGA";
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPGGA->automatic_flags;
  }

  strcpy(nmea, "GNGGA");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNGGA->automatic_flags;
  }

  strcpy(nmea, "GPRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPRMC->automatic_flags;
  }

  strcpy(nmea, "GNRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNRMC->automatic_flags;
  }

  return NULL;
}

sl_status_t gnss_max_m10s_is_nmea_auto(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  char nmea[] = "GPGGA";
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    if (gnss_cfg_data->storageNMEAGPGGA != NULL) {
      return true;
    }
  }

  strcpy(nmea, "GNGGA");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    if (gnss_cfg_data->storageNMEAGNGGA != NULL) {
      return true;
    }
  }

  strcpy(nmea, "GPRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    if (gnss_cfg_data->storageNMEAGPRMC != NULL) {
      return true;
    }
  }

  strcpy(nmea, "GNRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    if (gnss_cfg_data->storageNMEAGNRMC != NULL) {
      return true;
    }
  }

  return false;
}

uint8_t * gnss_max_m10s_get_nmea_working_length_ptr(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  char nmea[] = "GPGGA";
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPGGA->working_copy.length;
  }

  strcpy(nmea, "GNGGA");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNGGA->working_copy.length;
  }

  strcpy(nmea, "GPRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPRMC->working_copy.length;
  }

  strcpy(nmea, "GNRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNRMC->working_copy.length;
  }

  return NULL;
}

uint8_t * gnss_max_m10s_get_nmea_working_ptr(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  char nmea[] = "GPGGA";
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPGGA->working_copy.nmea[0];
  }

  strcpy(nmea, "GNGGA");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNGGA->working_copy.nmea[0];
  }

  strcpy(nmea, "GPRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPRMC->working_copy.nmea[0];
  }

  strcpy(nmea, "GNRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNRMC->working_copy.nmea[0];
  }

  return NULL;
}

uint8_t * gnss_max_m10s_get_nmea_complete_length_ptr(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  char nmea[] = "GPGGA";
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPGGA->complete_copy.length;
  }

  strcpy(nmea, "GNGGA");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNGGA->complete_copy.length;
  }

  strcpy(nmea, "GPRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPRMC->complete_copy.length;
  }

  strcpy(nmea, "GNRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNRMC->complete_copy.length;
  }

  return NULL;
}

uint8_t * gnss_max_m10s_get_complete_copy_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  char nmea[] = "GPGGA";
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPGGA->complete_copy.nmea[0];
  }

  strcpy(nmea, "GNGGA");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNGGA->complete_copy.nmea[0];
  }

  strcpy(nmea, "GPRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGPRMC->complete_copy.nmea[0];
  }

  strcpy(nmea, "GNRMC");
  if (memcmp(nmea, &gnss_cfg_data->nmea_address_field[1], 5) == 0) {
    return &gnss_cfg_data->storageNMEAGNRMC->complete_copy.nmea[0];
  }

  return NULL;
}

sl_status_t gnss_max_m10s_get_latitude_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  int32_t *latitude,
  uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *latitude = gnss_max_m10s_nmea_get_latitude(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_longitude_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  int32_t *longitude,
  uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status = gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                                &(gnss_cfg_data->packet_cfg),
                                                0,
                                                0);
  }

  if (SL_STATUS_OK == status) {
    *longitude = gnss_max_m10s_nmea_get_longitude(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_altitude_msl_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  int32_t *altitude_msl,
  uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    if (false == gnss_max_m10s_nmea_get_altitude_msl(altitude_msl,
                                                     gnss_cfg_data->nmea_data))
    {
      return SL_STATUS_FAIL;
    }
  }

  return status;
}

sl_status_t gnss_max_m10s_get_geoid_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         int32_t *geoid,
                                         uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    if (false
        == gnss_max_m10s_nmea_get_geoid_height(geoid,
                                               gnss_cfg_data->nmea_data)) {
      return SL_STATUS_FAIL;
    }
  }
  return status;
}

sl_status_t gnss_max_m10s_get_quality_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t *quality,
  uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }
  if (SL_STATUS_OK == status) {
    *quality = gnss_max_m10s_nmea_is_valid(gnss_cfg_data->nmea_data);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_satellite_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t *num_sat,
  uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }
  if (SL_STATUS_OK == status) {
    *num_sat = gnss_max_m10s_nmea_get_num_satellites(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_day_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint8_t *day,
                                       uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *day = gnss_max_m10s_nmea_get_day(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_month_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint8_t *month,
                                         uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *month = gnss_max_m10s_nmea_get_month(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_year_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t *year,
                                        uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *year = gnss_max_m10s_nmea_get_year(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_hour_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint8_t *hour,
                                        uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *hour = gnss_max_m10s_nmea_get_hour(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_min_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint8_t *min,
                                       uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *min = gnss_max_m10s_nmea_get_minute(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_second_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                          uint8_t *second,
                                          uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *second = gnss_max_m10s_nmea_get_second(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_speed_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         int32_t *speed,
                                         uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *speed = gnss_max_m10s_nmea_get_speed(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_ground_course_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  int32_t *course,
  uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *course = gnss_max_m10s_nmea_get_course(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_horizontal_dilution_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t *hdop,
  uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *hdop = gnss_max_m10s_nmea_get_hdop(gnss_cfg_data->nmea_data);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_nav_system_nmea(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  char *nav_system,
  uint16_t max_time)
{
  sl_status_t status = SL_STATUS_OK;
  uint32_t start_time = 0;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    status =
      gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                         &(gnss_cfg_data->packet_cfg),
                                         0,
                                         0);
  }

  if (SL_STATUS_OK == status) {
    *nav_system = gnss_max_m10s_nmea_get_nav_system(gnss_cfg_data->nmea_data);
  }

  return status;
}
