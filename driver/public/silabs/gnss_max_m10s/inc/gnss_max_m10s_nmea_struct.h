/***************************************************************************//**
 * @file gnss_max_m10s_nmea_struct.h
 * @brief MAX_M10S GNSS receiver NMEA sentence structures
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

#ifndef GNSS_MAX_M10S_NMEA_STRUCT_H_
#define GNSS_MAX_M10S_NMEA_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/******************************************************************************/

/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/

#define SL_MAX_M10S_NMEA_GGA_MAX_LENGTH         100         ///< Maximum length for GGA type NMEA sentence
#define SL_MAX_M10S_NMEA_RMC_MAX_LENGTH         100         ///< Maximum length for RMC type NMEA sentence
#define SL_MAX_M10S_NMEA_BYTE_COUNT             SL_MAX_M10S_NMEA_GGA_MAX_LENGTH ///< Maximum byte counts for any NMEA sentence type

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/// @brief type of NMEA sentence to be processed
typedef enum
{
  SL_MAX_M10S_FILTER_NMEA_ALL = 0x01, ///< To allow all NMEA sentence to be processed.
  SL_MAX_M10S_FILTER_NMEA_GGA = 0x02, ///< To allow only GGA type NMEA sentence to be processed.
  SL_MAX_M10S_FILTER_NMEA_RMC = 0x04, ///< To allow only RMC type NMEA sentence to be processed.
} sl_max_m10s_filter_nmea_e;

/// @brief Additional flags and pointers that need to be stored with each NMEA sentence type
typedef struct sl_max_m10s_nmea_automatic_flags
{
  union
  {
    uint8_t all;
    struct
    {
      uint8_t complete_copy_valid : 1; ///< Check if the copy of the data struct valid/fresh
      uint8_t complete_copy_read : 1;  ///< Check if the complete copy been read
      uint8_t callback_copy_valid : 1; ///< Check if the copy of the data struct used by the callback valid/fresh
    } bits;
  } flags; ///< Automatic flags
} sl_max_m10s_nmea_automatic_flags_t;

/// @brief to store length and buffer for GGA type NMEA sentence
typedef struct
{
  uint8_t length;                                ///< The number of bytes in nmea
  uint8_t nmea[SL_MAX_M10S_NMEA_GGA_MAX_LENGTH]; ///< Buffer to store the GGA type NMEA sentence
} sl_max_m10s_nmea_gga_data_t;

/// @brief to store the GGA type NMEA sentence
typedef struct
{
  sl_max_m10s_nmea_automatic_flags_t automatic_flags; ///< Automatic flags
  sl_max_m10s_nmea_gga_data_t working_copy;  ///< Incoming data is added to the working copy
  sl_max_m10s_nmea_gga_data_t complete_copy; ///< The working copy is copied into the complete copy when all data has been received and the checksum is valid
  void (*callback_pointer_ptr)(sl_max_m10s_nmea_gga_data_t *); ///< Callback pointer
  sl_max_m10s_nmea_gga_data_t *callback_copy; ///< The callback gets its own preserved copy of the complete copy
} sl_max_m10s_nmea_gga_t;

/// @brief to store the length and buffer for RMC type NMEA sentence
typedef struct
{
  uint8_t length;                                ///< The number of bytes in nmea
  uint8_t nmea[SL_MAX_M10S_NMEA_RMC_MAX_LENGTH]; ///< Buffer to store RMC type NMEA sentence
} sl_max_m10s_nmea_rmc_data_t;

/// @brief struct to store RMC type NMEA sentence
typedef struct
{
  sl_max_m10s_nmea_automatic_flags_t automatic_flags; ///< Automatic flags
  sl_max_m10s_nmea_rmc_data_t working_copy;  ///< Incoming data is added to the working copy
  sl_max_m10s_nmea_rmc_data_t complete_copy; ///< The working copy is copied into the complete copy when all data has been received and the checksum is valid
  void (*callback_pointer_ptr)(sl_max_m10s_nmea_rmc_data_t *); ///< pointer to callback
  sl_max_m10s_nmea_rmc_data_t *callbackCopy; ///< The callback gets its own preserved copy of the complete copy
} sl_max_m10s_nmea_rmc_t;

/// @brief struct to allow selective logging / processing of NMEA messages
typedef struct
{
  union
  {
    uint8_t all;
    struct
    {
      uint8_t all : 1;
      uint8_t UBX_NMEA_GGA : 1;
      uint8_t UBX_NMEA_RMC : 1;
    } bits;
  };
} sl_max_m10s_nmea_filtering_t;

/// @brief to store the data used in micro-NMEA library and data parsed from NMEA sentence
typedef struct sl_max_m10s_nmea_data
{
  uint8_t buffer_len;     ///< NMEA sentence buffer length
  uint8_t month;          ///< Current month in UTC format
  uint8_t day;            ///< Current day in UTC format
  uint8_t hour;           ///< Current hour in UTC format
  uint8_t minute;         ///< Current minute in UTC format
  uint8_t second;         ///< Current second in UTC format
  uint8_t quality;        ///< Quality indicator for position fix
  uint8_t num_sat;        ///< number of satellites tracked
  uint8_t hdop;           ///< Horizontal dilution of precision.
  bool altitude_valid;    ///< Flag to check the validity of altitude
  bool geoid_height_valid;///< Flag to check the validity of geoid height
  bool is_valid;          ///< Flag to check the valid data
  char talker_id;         ///< Talker ID of NMEA sentence
  char nav_system;        ///< navigation system from the NMEA sentence
  uint16_t year;          ///< Current year in UTC format
  uint32_t geoid_height;  ///< the height above WGS84 Geoid in milli-metres
  int32_t latitude;       ///< Latitude of current position in degrees and minutes
  int32_t longitude;      ///< Longitude of current position in degrees and minutes
  int32_t altitude;       ///< Altitude from mean sea level in milli-metres
  int32_t speed;          ///< Speed over ground in knots
  int32_t course;         ///< Course over ground in degrees
  char *buffer;           ///< Pointer to the start of the buffer
  char *ptr;              ///< Pointer to track the current position in NMEA sentence buffer.
  char message_id[6];     ///< message ID of the NMEA sentence
} sl_max_m10s_nmea_data_t;

#ifdef __cplusplus
}
#endif

#endif // GNSS_MAX_M10S_NMEA_STRUCT_H_

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
