/***************************************************************************//**
 * @file gnss_max_m10s_ubx_struct.h
 * @brief MAX_M10S GNSS receiver UBX packet structures
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

#ifndef GNSS_MAX_M10S_UBX_STRUCT_H_
#define GNSS_MAX_M10S_UBX_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/// @brief Flags to check the UBX packet update
typedef struct sl_max_m10s_ubx_automatic_flags
{
  union
  {
    uint8_t all;                      ///< Used to check all the data type of the UBX packet
    struct
    {
      uint8_t automatic : 1;          ///< Used to check if this message be delivered and parsed "automatically" (without polling)
      uint8_t implicitUpdate : 1;     ///< Used to check if the update triggered by accessing stale data (=true) or by a call to checkUblox (=false)
      uint8_t addToFileBuffer : 1;    ///< Used to check if the raw UBX data be added to the file buffer
      uint8_t callbackCopyValid : 1;  ///< Used to check if the copy of the data struct used by the callback valid/fresh
    } bits;                           ///< bits to represent different data
  } flags;                            ///< To store the flags to check the UBX packet update
}sl_max_m10s_ubx_automatic_flags_t;

/// @brief structure to store the data for UBX-NAV-PVT
typedef struct sl_max_m10s_ubx_nav_pvt_data
{
  uint32_t iTOW;                     ///< GPS time of week of the navigation epoch: ms
  uint8_t fixType;                   ///< GNSSfix Type
  uint8_t numSV;                     ///< Number of satellites used in navigation Solution
  int32_t lon;                       ///< Longitude: deg * 1e-7
  int32_t lat;                       ///< Latitude: deg * 1e-7
  int32_t height;                    ///< Height above ellipsoid: mm
  int32_t hMSL;                      ///< Height above mean sea level: mm
  uint8_t hour;                      ///< Hour of day, range 0..23 (UTC)
  uint8_t min;                       ///< Minute of hour, range 0..59 (UTC)
  uint8_t sec;                       ///< Seconds of minute, range 0..60 (UTC)
  uint8_t nano;                      ///< nano seconds of time (UTC)
  uint8_t tAcc;                      ///< time accuracy estimate (nano seconds)
  uint16_t year;                     ///< Year (UTC)
  uint8_t month;                     ///< Month, range 1..12 (UTC)
  uint8_t day;                       ///< Day of month, range 1..31 (UTC)
  uint32_t hAcc;                     ///< Horizontal accuracy estimate: mm
  uint32_t vAcc;                     ///< Vertical accuracy estimate: mm
  int32_t velN;                      ///< NED north velocity: mm/s
  int32_t velE;                      ///< NED east velocity: mm/s
  int32_t velD;                      ///< NED down velocity: mm/s
  int32_t gSpeed;                    ///< Ground Speed (2-D): mm/s
  int32_t headMot;                   ///< Heading of motion (2-D): deg * 1e-5
  uint32_t sAcc;                     ///< Speed accuracy estimate: mm/s
  uint32_t headAcc;                  ///< Heading accuracy estimate (both motion and vehicle): deg * 1e-5
  uint16_t pDOP;                     ///< Position DOP * 0.01
  uint8_t reserved1[5];              ///< Reserved data
  int32_t headVeh;                   ///< Heading of vehicle (2-D): deg * 1e-5
  int16_t magDec;                    ///< Magnetic declination: deg * 1e-2
  uint16_t magAcc;                   ///< Magnetic declination accuracy: deg * 1e-2
  union
  {
    uint8_t all;                     ///< To store different data represented in bits.
    struct
    {
      uint8_t valid_date : 1;        ///< 1 = valid UTC Date
      uint8_t valid_time : 1;        ///< 1 = valid UTC time of day
      uint8_t fullyResolved : 1;     ///< 1 = UTC time of day has been fully resolved (no seconds uncertainty).
      uint8_t validMag : 1;          ///< 1 = valid magnetic declination
    } bits;                          ///< bits to represent different data
  } valid;                           ///< Validity flags

  union
  {
    uint8_t all;                            ///< To store different data represented in bits.
    struct
    {
      uint8_t reserved : 5;                 ///< Reserved bits
      uint8_t confirmed_available : 1;      ///< 1 = information about UTC Date and Time of Day validity confirmation is available
      uint8_t confirmed_date : 1;           ///< 1 = UTC Date validity could be confirmed
      uint8_t confirmed_time : 1;           ///< 1 = UTC Time of Day could be confirmed
    }confirm_data;                          ///< Confirmation data
  }confirm_flags;                           ///< Confirmation flags
  union
  {
    uint8_t all;                        ///< To store different data represented in bits.
    struct
    {
      uint8_t invalidLlh : 1;           ///< 1 = Invalid lon, lat, height and hMSL
      uint8_t lastCorrectionAge : 4;    ///< Age of the most recently received differential correction
    } bits;                             ///< bits to represent different data
  } correction_flags;                   ///< Correction data flags
  union
  {
    uint8_t all;
    struct
    {
      uint8_t gnssFixOK : 1;            ///< 1 = valid fix (i.e within DOP & accuracy masks)
      uint8_t diffSoln : 1;             ///< 1 = differential corrections were applied
      uint8_t psmState : 3;             ///< Power save mode state
      uint8_t headVehValid : 1;         ///< 1 = heading of vehicle is valid, only set if the receiver is in sensor fusion mode
      uint8_t carrSoln : 2;             ///< Carrier phase range solution status
    } bits;                             ///< bits to represent different data
  } solution_flags;                     ///< Solution flags
} sl_max_m10s_ubx_nav_pvt_data_t;

/// @brief To store the UBX packet with data, flags and updated values.
typedef struct sl_max_m10s_ubx_nav_pvt
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;                ///< To store automatic flags for the UBX packet
  sl_max_m10s_ubx_nav_pvt_data_t data;                              ///< To store the data for the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_nav_pvt_data_t *);   ///< To store the callback pointer for UBX packet
  sl_max_m10s_ubx_nav_pvt_data_t *callback_data;                    ///< To get the callback data
} sl_max_m10s_ubx_nav_pvt_t;

/// @brief structure to store the data for UBX-NAV-STATUS
typedef struct sl_max_m10s_ubx_nav_status_data
{
  uint32_t iTOW;                        ///< GPS time of week of the navigation epoch: ms
  uint8_t gps_fix;                      ///< GPS fix Type
  union
  {
    uint8_t all;                           ///< To store different data represented in bits.
    struct
    {
      uint8_t gps_fix_ok : 1;              ///< 1 = position and velocity valid and within DOP and ACC Masks.
      uint8_t differential_solution : 1;   ///< 1 = differential corrections were applied
      uint8_t week_number : 1;             ///< 1 = Week Number valid (see Time Validity section for details)
      uint8_t time_of_week : 1;            ///< 1 = Time of Week valid (see Time Validity section for details)
    } bits;                                ///< bits to represent different data
  } flags;                                 ///< Fix status flags
  union
  {
    uint8_t all;                              ///< To store different data represented in bits.
    struct
    {
      uint8_t differential_correction : 1;    ///< 1 = differential corrections available
      uint8_t correction_valid : 1;           ///< 1 = valid carrSoln
      uint8_t reserved : 4;                   ///< reserved bits
      uint8_t map_matching : 2;               ///< map matching status
    } bits;                        ///< bits to represent different data
  } fix_status;                    ///< Fix Status Information
  union
  {
    uint8_t all;                   ///< To store different data represented in bits.
    struct
    {
      uint8_t psm_state : 2;       ///< power save mode stateE
      uint8_t reserved_1 : 1;      ///< reserved bit
      uint8_t spoof_state : 2;     ///< Spoofing detection state
      uint8_t reserved_2 : 1;      ///< reserved bits
      uint8_t carrier_solution : 2; ///< Carrier phase range solution status
    } bits;                         ///< bits to represent different data
  } flags_2;                        ///< Additional flags
  uint32_t time_to_first_fix;       ///< Time to first fix (millisecond time tag): ms
  uint32_t milli_sec_since_startup; ///< Milliseconds since Startup / Reset: ms
} sl_max_m10s_ubx_nav_status_data_t;

/// @brief structure to store the UBX-NAV-STATUS packet
typedef struct
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;                  ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_nav_status_data_t data;                             ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_nav_status_data_t *);  ///< Callback pointer
  sl_max_m10s_ubx_nav_status_data_t *callback_data;                   ///< Pointer to callback data
} sl_max_m10s_ubx_nav_status_t;

/// @brief structure to store the data for UBX-NAV-DOP
typedef struct
{
  uint32_t iTOW;        ///< GPS time of week of the navigation epoch: ms
  uint16_t geometric;   ///< Geometric DOP: * 0.01
  uint16_t position;    ///< Position DOP: * 0.01
  uint16_t time;        ///< Time DOP: * 0.01
  uint16_t vertical;    ///< Vertical DOP: * 0.01
  uint16_t horizontal;  ///< Horizontal DOP: * 0.01
  uint16_t north;       ///< Northing DOP: * 0.01
  uint16_t east;        ///<  Easting DOP: * 0.01
} sl_max_m10s_ubx_nav_dop_data_t;

/// @brief structure to store the UBX-NAV-DOP packet
typedef struct sl_max_m10s_ubx_nav_dop
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;              ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_nav_dop_data_t data;                            ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_nav_dop_data_t *); ///< Callback pointer
  sl_max_m10s_ubx_nav_dop_data_t *callback_data;                  ///< Pointer to callback data
} sl_max_m10s_ubx_nav_dop_t;

/// @brief structure to store the data for UBX-NAV-CLOCK
typedef struct
{
  uint32_t iTOW;          ///< GPS time of week of the navigation epoch: ms
  int32_t clock_bias;     ///< Clock bias: ns
  int32_t clock_drift;    ///< Clock drift: ns/s
  uint32_t time_accuracy; ///< Time accuracy estimate: ns
  uint32_t freq_accuracy; ///< Frequency accuracy estimate: ps/s
} sl_max_m10s_ubx_nav_clock_data_t;

/// @brief structure to store the UBX-NAV-CLOCK packet
typedef struct sl_max_m10s_ubx_nav_clock
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;                ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_nav_clock_data_t data;                            ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_nav_clock_data_t *); ///< Callback pointer
  sl_max_m10s_ubx_nav_clock_data_t *callback_data;                  ///< Pointer to callback data
} sl_max_m10s_ubx_nav_clock_t;

/// @brief structure to store the data for UBX-NAV-EOE
typedef struct
{
  uint32_t iTOW;           ///< GPS time of week of the navigation epoch: ms
} sl_max_m10s_ubx_nav_epoch_data_t;

/// @brief structure to store the UBX-NAV-EOE packet
typedef struct sl_max_m10s_ubx_nav_epoch
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;                ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_nav_epoch_data_t data;                            ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_nav_epoch_data_t *); ///< Callback pointer
  sl_max_m10s_ubx_nav_epoch_data_t *callback_data;                  ///< Pointer to callback data
} sl_max_m10s_ubx_nav_epoch_t;

/// @brief structure to store common data for UBX-NAV-SAT packet
typedef struct
{
  uint32_t iTOW;   ///< GPS time of week
  uint8_t version; ///< Message version (0x01 for this version)
  uint8_t num_sv;  ///< Number of satellites
  uint8_t reserved1[2]; ///< Reserved bits
} sl_max_m10s_ubx_navsat_header_t;

/// @brief structure to store the data specific to subset of signal identifier for UBX-NAV-SAT
typedef struct
{
  uint8_t gnss_id;                ///< GNSS identifier
  uint8_t sv_id;                  ///< Satellite identifier
  uint8_t carrier_noise_ratio;    ///< Carrier-to-noise density ratio: dB-Hz
  int8_t elevation;               ///< Elevation (range: +/-90): deg
  int16_t azimuth;                ///< Azimuth (range 0-360): deg
  int16_t pseudo_range;           ///< Pseudorange residual: m * 0.1
  union
  {
    uint32_t all;                      ///< Used to check all the data type of the UBX packet
    struct
    {
      uint32_t quality_indicator : 3;  ///< Signal quality indicator: 0: no signal
      uint32_t sv_used : 1;            ///< 1 = Signal in the subset specified in Signal Identifiers is currently being used for navigation
      uint32_t health : 2;             ///< Signal health flag: 0: unknown  1: healthy  2: unhealthy
      uint32_t diff_correction : 1;    ///< 1 = differential correction data is available for this SV
      uint32_t smoothed : 1;           ///< 1 =  carrier smoothed pseudorange used
      uint32_t orbit_source : 3;       ///< Orbit source
      uint32_t ephemeris_avail : 1;    ///< 1 = ephemeris is available for this SV
      uint32_t almanac_avail : 1;      ///< 1 = almanac is available for this SV
      uint32_t assist_now_avail : 1;   ///< 1 = AssistNow Offline data is available for this SV
      uint32_t assist_now_autonomous_avail : 1;  ///< 1 = AssistNow Autonomous data is available for this SV
      uint32_t reserved_1 : 1;                   ///< reserved bit
      uint32_t sbas_correction_used : 1;         ///< 1 = SBAS corrections have been used for a signal in the subset specified in Signal Identifiers
      uint32_t rtcm_correction_used : 1;         ///< 1 = RTCM corrections have been used for a signal in the subset specified in Signal Identifiers
      uint32_t slas_correction_used : 1;         ///< 1 = QZSS SLAS corrections have been used for a signal in the subset specified in Signal Identifiers
      uint32_t spartn_correction_used : 1;       ///< 1 = SPARTN corrections have been used for a signal in the subset specified in Signal Identifiers
      uint32_t psuedorange_correction_used : 1;  ///< 1 = Pseudorange corrections have been used for a signal in the subset specified in Signal Identifiers
      uint32_t carrier_range_correction_used : 1; ///< 1 = Carrier range corrections have been used for a signal in the subset specified in Signal Identifiers
      uint32_t doppler_correction_used : 1;       ///< 1 = Range rate (Doppler) corrections have been used for a signal in the subset specified in Signal Identifiers
      uint32_t reserved_2 : 9;                    ///< reserved bits
    } bits;                                       ///< bits to represent different data
  } flags;                                        ///< Bitmask for different data
} sl_max_m10s_ubx_navsat_block_t;

/// @brief structure to store the whole data of UBX-NAV-SAT packet
typedef struct
{
  sl_max_m10s_ubx_navsat_header_t header;     ///< Data common for all satellites
  sl_max_m10s_ubx_navsat_block_t blocks[255]; ///< Data for specific satellite
} sl_max_m10s_ubx_navsat_data_t;

/// @brief structure to store the data for UBX-NAV-SAT
typedef struct sl_max_m10s_ubx_nav_sat
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;              ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_navsat_data_t data;                             ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_navsat_data_t *);  ///< Callback pointer
  sl_max_m10s_ubx_navsat_data_t *callback_data;                   ///< Pointer to callback data
} sl_max_m10s_ubx_nav_sat_t;

/// @brief structure to store the data of UBX-NAV-TIMLS packet.
typedef struct
{
  uint32_t iTOW;                      ///< GPS time of week of the navigation epoch: ms
  uint8_t version;                    ///< Message version (0x00 for this version)
  uint8_t reserved_1[3];              ///< reserved
  uint8_t src_of_current_leap;        ///< Information source for the current number of leap seconds
  int8_t current_leap_sec;            ///< Current number of leap seconds since start of GPS (Jan 6, 1980), s
  uint8_t src_of_leap_sec_change;     ///< Information source for the future leap second event
  int8_t leap_sec_change;             ///< Future leap second change if one is scheduled, +1, 0, -1s
  int32_t time_to_leap_sec_event;     ///< Number of seconds until the next or from the last leap second, s
  uint16_t date_of_leap_sec_gps_week; ///< GPS week num (WN) of the next or the last leap second event
  uint16_t day_of_leap_sec_gps;       ///< GPS day of week num (DN) for the next or last leap second event
  uint8_t reserved_2[3];              ///< Reserved bits
  union
  {
    uint8_t all;                                 ///< To store different data represented in bits.
    struct
    {
      uint8_t valid_current_leap_sec : 1;        ///< 1 = Valid current number of leap seconds value
      uint8_t valid_time_to_leap_sec_event : 1;  ///< 1 = Valid time to next leap second event or from the last leap second event if no future event scheduled
    } bits;                                      ///< bits to represent different data
  } valid;                                       ///< Validity Flags
} sl_max_m10s_ubx_nav_timels_data_t;

/// @brief To store the UBX-NAV-TIMELS packet.
typedef struct sl_max_m10s_ubx_nav_timels
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;                ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_nav_timels_data_t data;                           ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_nav_timels_data_t *); ///< Callback pointer
  sl_max_m10s_ubx_nav_timels_data_t *callback_data;                 ///< Pointer to callback data
} sl_max_m10s_ubx_nav_timels_t;

/// @brief To store the UBX-NAV-TIMEUTC packet data.
typedef struct
{
  uint32_t iTOW;            ///< GPS time of week of the navigation epoch: ms
  uint32_t time_accuracy;   ///< Time accuracy estimate (UTC): ns
  int32_t nano;             ///< Fraction of second, range -1e9 .. 1e9 (UTC): ns
  uint16_t year;            ///< Year (UTC)
  uint8_t month;            ///< Month, range 1..12 (UTC)
  uint8_t day;              ///< Day of month, range 1..31 (UTC)
  uint8_t hour;             ///< Hour of day, range 0..23 (UTC)
  uint8_t min;              ///< Minute of hour, range 0..59 (UTC)
  uint8_t sec;              ///< Seconds of minute, range 0..60 (UTC)
  union
  {
    uint8_t all;                        ///< To store different data represented in bits.
    struct
    {
      uint8_t valid_tow : 1;            ///< 1 = Valid Time of Week
      uint8_t valid_week_number : 1;    ///< 1 = Valid Week Number
      uint8_t valid_utc : 1;            ///< 1 = Valid UTC Time
      uint8_t reserved : 1;             ///< reserved bit
      uint8_t utc_standard : 4;         ///< UTC standard identifier
    } bits;                 ///< bits to represent different data
  } valid;                  ///< Validity flags
} sl_max_m10s_ubx_nav_timeutc_data_t;

/// @brief To store the UBX-NAV-TIMEUTC packet.
typedef struct
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;                  ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_nav_timeutc_data_t data;                            ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_nav_timeutc_data_t *); ///< Callback pointer
  sl_max_m10s_ubx_nav_timeutc_data_t *callback_data;                  ///< Pointer to callback data
} sl_max_m10s_ubx_nav_timeutc_t;

/// @brief To store the UBX-NAV-SIG packet data common for all signals.
typedef struct
{
  uint32_t iTOW;        ///< GPS time of week
  uint8_t version;      ///< Message version (0x00 for this version)
  uint8_t num_signals;  ///< Number of signals
  uint8_t reserved0[2]; ///< reserved
} sl_max_m10s_ubx_navsig_header_t;

/// @brief To store the UBX-NAV-SIG packet data for a specific signal.
typedef struct
{
  uint8_t gnss_id;                ///< GNSS identifier
  uint8_t sv_id;                  ///< Satellite identifier
  uint8_t sig_id;                 ///< New signal identifier
  uint8_t freq_id;                ///< GLONASS frequency slot
  int16_t psuedo_range;           ///< Pseudorange residual: m * 0.1
  uint8_t carrier_noise_ratio;    ///< Carrier-to-noise density ratio: dB-Hz
  uint8_t quality_ind;            ///< Signal quality indicator:
  uint8_t corr_source;            ///< Correction source:
  uint8_t iono_model;             ///< Ionospheric model used:
  union
  {
    uint16_t all;                           ///< To store different data represented in bits.
    struct
    {
      uint16_t health : 1;                  ///< Signal health flag: 0 = unknown; 1 = healthy; 2 = unhealthy
      uint16_t pr_smoothed : 1;             ///< 1 = Pseudorange has been smoothed
      uint16_t pr_used : 1;                 ///< 1 = Pseudorange has been used for this signal
      uint16_t cr_used : 1;                 ///< 1 = Carrier range has been used for this signal
      uint16_t doppler_used : 1;            ///< 1 = Range rate (Doppler) has been used for this signal
      uint16_t pr_corr_used : 1;            ///< 1 = Pseudorange corrections have been used for this signal
      uint16_t carrier_corr_used : 1;       ///< 1 = Carrier range corrections have been used for this signal
      uint16_t doppler_corr_used : 1;       ///< 1 = Range rate (Doppler) corrections have been usedfor this signal
      uint16_t authentication_status : 1;   ///< Authentication status of the navigation data: 0 = Unknown; 1 = Authenticated
    } bits;                         ///< bits to represent different data
  } sig_flags;                      ///< Signal related flags
  uint8_t reserved1[4];             ///< Reserved bytes
} sl_max_m10s_ubx_navsig_block_t;

/// @brief To store the UBX-NAV-SIG packet whole data.
typedef struct
{
  sl_max_m10s_ubx_navsig_header_t header;     ///< Data common to all signals
  sl_max_m10s_ubx_navsig_block_t blocks[92];  ///< Data for specific signal
} sl_max_m10s_ubx_navsig_data_t;

/// @brief To store the UBX-NAV-SIG packet.
typedef struct
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;             ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_navsig_data_t data;                            ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_navsig_data_t *); ///< Callback pointer
  sl_max_m10s_ubx_navsig_data_t *callback_data;                  ///< Pointer to callback data
} sl_max_m10s_ubx_navsig_t;

/// @brief To store the UBX-NAV-POSLLH packet data.
typedef struct
{
  uint32_t iTOW;    ///< GPS time of week of the navigation epoch: ms
  int32_t lon;      ///< Longitude: Degrees * 1e-7
  int32_t lat;      ///< Latitude: Degrees * 1e-7
  int32_t height;   ///< Height above ellipsoid: mm
  int32_t hMSL;     ///< Height above mean sea level: mm
  uint32_t h_acc;   ///< Horizontal Accuracy Estimate: mm
  uint32_t v_acc;   ///< Vertical Accuracy Estimate: mm
} sl_max_m10s_ubx_nav_posllh_data_t;

/// @brief To store the UBX-NAV-POSLLH packet.
typedef struct
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;                  ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_nav_posllh_data_t data;                             ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_nav_posllh_data_t *);  ///< Callback pointer
  sl_max_m10s_ubx_nav_posllh_data_t *callback_data;                   ///< Pointer to callback data
} sl_max_m10s_ubx_nav_posllh_t;

/// @brief To store the UBX-SEC_UNIQID packet.
typedef struct
{
  uint8_t version;        ///< GNSS receiver module version
  uint8_t reserved0[3];   ///< reserved
  uint8_t unique_id[6];   ///< Unique id of the GNSS module
} sl_max_m10s_ubx_unique_id_t;

/// @brief To store the UBX-NAV-VELNED packet data.
typedef struct
{
  uint32_t iTOW;          ///< GPS time of week of the navigation epoch: ms
  int32_t velN;           ///< North velocity component: cm/s
  int32_t velE;           ///< East velocity component: cm/s
  int32_t velD;           ///< Down velocity component: cm/s
  uint32_t speed;         ///< Speed (3-D): cm/s
  uint32_t ground_speed;  ///< Ground Speed (2-D): cm/s
  int32_t heading;        ///< Heading of motion 2-D: Degrees * 1e-5
  uint32_t speed_acc;     ///< Speed accuracy estimate: cm/s
  uint32_t course_acc;    ///< Course/Heading accuracy estimate: Degrees * 1e-5
} sl_max_m10s_ubx_nav_velned_data_t;

/// @brief To store the UBX-NAV-VELNED packet.
typedef struct sl_max_m10s_ubx_nav_velned
{
  sl_max_m10s_ubx_automatic_flags_t automatic_flags;                  ///< Automatic flags for UBX packet
  sl_max_m10s_ubx_nav_velned_data_t data;                             ///< Data from the UBX packet
  void (*callback_pointer_ptr)(sl_max_m10s_ubx_nav_velned_data_t *);  ///< Callback pointer
  sl_max_m10s_ubx_nav_velned_data_t *callback_data;                   ///< Pointer to callback data
} sl_max_m10s_ubx_nav_velned_t;

#ifdef __cplusplus
}
#endif

#endif // GNSS_MAX_M10S_UBX_STRUCT_H_

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
