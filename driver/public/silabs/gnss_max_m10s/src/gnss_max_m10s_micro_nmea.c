/***************************************************************************//**
 * @file gnss_max_m10s_micro_nmea.c
 * @brief MAX_M10S GNSS receiver driver micro-NMEA related external APIs
 * implementation.
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
#include <ctype.h>
#include "gnss_max_m10s_micro_nmea.h"

/**************************************************************************//**
 * @brief To calculate the 10 raise to the power of input.
 * @param[in] b : input digit for which the exponential value is calculated.
 * @return long type calculated value.
 *****************************************************************************/
static long gnss_max_m10s_exp_10(uint8_t b);

/**************************************************************************//**
 * @brief To convert the input value in hexadecimal.
 * @param[in] nibble : input value.
 * @return hexadecimal value.
 *****************************************************************************/
static char gnss_max_m10s_to_hex(uint8_t nibble);

/**************************************************************************//**
 * @brief To skip a field from NMEA sentence.
 * @param[in] s : pointer to NMEA filed.
 * @return pointer to the next field of the NMEA sentence after the comma.
 *****************************************************************************/
static const char * gnss_max_m10s_skip_field(const char *s);

/**************************************************************************//**
 * @brief To parse an unsigned int type data from NMEA sentence.
 * @param[in] s : pointer to NMEA filed.
 * @param[in] len : length of the parsed data.
 * @return unsigned int type data from the sentence.
 *****************************************************************************/
static unsigned int gnss_max_m10s_parse_unsigned_int(const char *s,
                                                     uint8_t len);

/**************************************************************************//**
 * @brief To parse an float type data from NMEA sentence.
 * @param[in] s : pointer to NMEA field.
 * @param[in] log_10_multiplier : value of multiplier.
 * @param[in] eptr : pointer to return the position in the input string where parsing stopped .
 * @param[in] result_valid : pointer to the valid result check.
 * @return long type NMEA data.
 *****************************************************************************/
static long gnss_max_m10s_parse_float(const unsigned char *s,
                                      uint8_t log_10_multiplier,
                                      const char **eptr,
                                      bool *result_valid);

/**************************************************************************//**
 * @brief To parse data from NMEA sentence in degree minute formate.
 * @param[in] s : pointer to NMEA field.
 * @param[in] deg_width : width of degree.
 * @param[in] eptr : pointer to return the position in the input string where parsing stopped .
 * @return data in degree minute format.
 *****************************************************************************/
static long gnss_max_m10s_parse_degree_minute(const char *s,
                                              uint8_t deg_width,
                                              const char **eptr);

/**************************************************************************//**
 * @brief To parse data from NMEA sentence.
 * @param[in] s : pointer to NMEA field.
 * @param[in] result : pointer to the field parsed.
 * @param[in] len : length of the field.
 * @return pointer to the field parsed.
 *****************************************************************************/
static const char * gnss_max_m10s_parse_field(const char *s,
                                              char *result,
                                              int len);

/**************************************************************************//**
 * @brief To generate checksum for NMEA sentence.
 * @param[in] s : pointer to NMEA sentence.
 * @param[out] checksum : To store the calculated checksum value.
 * @return a pointer to the end of the string.
 *****************************************************************************/
static const char * gnss_max_m10s_generate_checksum(const char *s,
                                                    char *checksum);

/**************************************************************************//**
 * @brief To check generate checksum for NMEA sentence and calculated checksum.
 * @param[in] s : pointer to NMEA sentence.
 * @return true if checksum matches the calculated checksum.
 *****************************************************************************/
static bool gnss_max_m10s_test_checksum(const char *s);

/**************************************************************************//**
 * @brief To set the buffer allocated for micro nmea.
 * @param[in] gnss_cfg_data : pointer to GNSS cfg data structure.
 *****************************************************************************/
static void gnss_max_m10s_set_buffer(sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief To check the end of field from NMEA sentence.
 * @param[in] c : character from NMEA sentence.
 * @return true if end of field is found.
 *         false if end of field is not found.
 *****************************************************************************/
static bool gnss_max_m10s_is_end_of_fields(char c);

/**************************************************************************//**
 * @brief To process the RMC type NMEA sentence.
 * @param[in] s : character to be processed from NMEA sentence.
 * @param[in] nmea_cfg_data : pointer to NMEA data.
 * @return The following values are returned:
 * - \ref true on success.
 * - \ref false on failure.
 *****************************************************************************/
static bool gnss_max_m10s_process_rmc(const char *s,
                                      sl_max_m10s_nmea_data_t *nmea_data);

/**************************************************************************//**
 * @brief To process the GGA type NMEA sentence.
 * @param[in] s : character to be processed from NMEA sentence.
 * @param[in] nmea_cfg_data : pointer to NMEA data.
 * @return The following values are returned:
 * - \ref true on success.
 * - \ref false on failure.
 *****************************************************************************/
static bool gnss_max_m10s_process_gga(const char *s,
                                      sl_max_m10s_nmea_data_t *nmea_data);

static long gnss_max_m10s_exp_10(uint8_t b)
{
  long r = 1;
  while (b--) {
    r *= 10;
  }
  return r;
}

static char gnss_max_m10s_to_hex(uint8_t nibble)
{
  if (nibble >= 10) {
    return nibble + 'A' - 10;
  } else {
    return nibble + '0';
  }
}

static const char * gnss_max_m10s_skip_field(const char *s)
{
  if (s == NULL) {
    return NULL;
  }

  while (!gnss_max_m10s_is_end_of_fields(*s)) {
    if (*s == ',') {
      /// Check next character
      if (gnss_max_m10s_is_end_of_fields(*++s)) {
        break;
      } else {
        return s;
      }
    }
    ++s;
  }

  return NULL; /// End of string or valid sentence
}

static unsigned int gnss_max_m10s_parse_unsigned_int(const char *s,
                                                     uint8_t len)
{
  int r = 0;
  while (len--) {
    r = 10 * r + *s++ - '0';
  }
  return r;
}

static long gnss_max_m10s_parse_float(const unsigned char *s,
                                      uint8_t log_10_multiplier,
                                      const char **eptr,
                                      bool *result_valid)
{
  int8_t neg = 1;
  long r = 0;

  if (result_valid) {
    *result_valid = false;
  }

  while (isspace(*s)) {
    ++s;
  }

  if (*s == '-') {
    neg = -1;
    ++s;
  } else if (*s == '+') {
    ++s;
  }

  while (isdigit(*s)) {
    r = 10 * r + *s++ - '0';
    if (result_valid) {
      *result_valid = true;
    }
  }

  r *= gnss_max_m10s_exp_10(log_10_multiplier);

  if (*s == '.') {
    ++s;
    long frac = 0;
    while (isdigit(*s) && log_10_multiplier) {
      frac = 10 * frac + *s++ - '0';
      --log_10_multiplier;
    }
    frac *= gnss_max_m10s_exp_10(log_10_multiplier);
    r += frac;
  }

  r *= neg; /// Include effect of any minus sign

  if (eptr) {
    *eptr = gnss_max_m10s_skip_field((const char *)s);
  }

  return r;
}

static long gnss_max_m10s_parse_degree_minute(const char *s,
                                              uint8_t deg_width,
                                              const char **eptr)
{
  if (*s == ',') {
    if (eptr) {
      *eptr = gnss_max_m10s_skip_field(s);
    }
    return 0;
  }
  long r = gnss_max_m10s_parse_unsigned_int(s, deg_width) * 1000000L;
  s += deg_width;
  r += gnss_max_m10s_parse_float((unsigned char *)s, 6, eptr, NULL) / 60;
  return r;
}

static const char * gnss_max_m10s_parse_field(const char *s,
                                              char *result,
                                              int len)
{
  if (s == NULL) {
    return NULL;
  }

  int i = 0;
  while (*s != ',' && !gnss_max_m10s_is_end_of_fields(*s)) {
    if (result && (i++ < len)) {
      *result++ = *s;
    }
    ++s;
  }
  if (result && (i < len)) {
    *result = '\0';
  }

  if (*s == ',') {
    return ++s; /// Location of start of next field
  } else {
    return NULL; /// End of string or valid sentence
  }
}

static const char * gnss_max_m10s_generate_checksum(const char *s,
                                                    char *checksum)
{
  uint8_t c = 0;
  /// Initial $ is omitted from checksum
  if (*s == '$') {
    ++s;
  }

  while (*s != '\0' && *s != '*') {
    c ^= *s++;
  }

  if (checksum) {
    checksum[0] = gnss_max_m10s_to_hex(c / 16);
    checksum[1] = gnss_max_m10s_to_hex(c % 16);
  }
  return s;
}

static bool gnss_max_m10s_test_checksum(const char *s)
{
  char checksum[2];
  const char *p = gnss_max_m10s_generate_checksum(s, checksum);
  return *p == '*' && p[1] == checksum[0] && p[2] == checksum[1];
}

char gnss_max_m10s_nmea_get_nav_system(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->nav_system;
}

uint8_t gnss_max_m10s_nmea_get_num_satellites(
  sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->num_sat;
}

uint8_t gnss_max_m10s_nmea_get_hdop(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->hdop;
}

bool gnss_max_m10s_nmea_is_valid(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->is_valid;
}

int32_t gnss_max_m10s_nmea_get_latitude(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->latitude;
}

int32_t gnss_max_m10s_nmea_get_longitude(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->longitude;
}

bool gnss_max_m10s_nmea_get_altitude_msl(int32_t *alt,
                                         sl_max_m10s_nmea_data_t *nmea_data)
{
  if (nmea_data->altitude_valid) {
    *alt = nmea_data->altitude;
  }
  return nmea_data->altitude_valid;
}

bool gnss_max_m10s_nmea_get_geoid_height(int32_t *alt,
                                         sl_max_m10s_nmea_data_t *nmea_data)
{
  if (nmea_data->geoid_height_valid) {
    *alt = nmea_data->geoid_height;
  }

  return nmea_data->geoid_height_valid;
}

uint16_t gnss_max_m10s_nmea_get_year(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->year;
}

uint8_t gnss_max_m10s_nmea_get_month(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->month;
}

uint8_t gnss_max_m10s_nmea_get_day(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->day;
}

uint8_t gnss_max_m10s_nmea_get_hour(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->hour;
}

uint8_t gnss_max_m10s_nmea_get_minute(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->minute;
}

uint8_t gnss_max_m10s_nmea_get_second(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->second;
}

int32_t gnss_max_m10s_nmea_get_speed(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->speed;
}

int32_t gnss_max_m10s_nmea_get_course(sl_max_m10s_nmea_data_t *nmea_data)
{
  return nmea_data->course;
}

static bool gnss_max_m10s_is_end_of_fields(char c)
{
  return c == '*' || c == '\0' || c == '\r' || c == '\n';
}

void gnss_max_m10s_init_micro_nmea(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->nmea_data->talker_id = '\0';
  memset(gnss_cfg_data->nmea_data->message_id, 0,
         sizeof(gnss_cfg_data->nmea_data->message_id));
  gnss_max_m10s_set_buffer(gnss_cfg_data);
  gnss_max_m10s_clear_nmea_data(gnss_cfg_data->nmea_data);
}

static void gnss_max_m10s_set_buffer(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->nmea_data->buffer_len = sizeof(gnss_cfg_data->nmea_buffer);
  gnss_cfg_data->nmea_data->buffer = (char *)gnss_cfg_data->nmea_buffer;
  gnss_cfg_data->nmea_data->ptr = gnss_cfg_data->nmea_data->buffer;

  if (gnss_cfg_data->nmea_data->buffer_len) {
    *gnss_cfg_data->nmea_data->ptr = '\0';
    gnss_cfg_data->nmea_data->buffer[gnss_cfg_data->nmea_data->buffer_len
                                     - 1] = '\0';
  }
}

void gnss_max_m10s_clear_nmea_data(sl_max_m10s_nmea_data_t *nmea_data)
{
  nmea_data->nav_system = '\0';
  nmea_data->num_sat = 0;
  nmea_data->hdop = 255;
  nmea_data->is_valid = false;
  nmea_data->latitude = 999000000L;
  nmea_data->longitude = 999000000L;
  nmea_data->altitude = LONG_MIN;
  nmea_data->speed = LONG_MIN;
  nmea_data->course = LONG_MIN;
  nmea_data->altitude_valid = false;
  nmea_data->geoid_height_valid = false;
  nmea_data->year = 0;
  nmea_data->month = 0;
  nmea_data->day = 0;
  nmea_data->hour = 99;
  nmea_data->minute = 99;
  nmea_data->second = 99;
}

bool gnss_max_m10s_process_nmea(char c, sl_max_m10s_nmea_data_t *nmea_data)
{
  if ((nmea_data->buffer == NULL) || (nmea_data->buffer_len == 0)) {
    return false;
  }

  if ((c == '\0') || (c == '\n') || (c == '\r')) {
    *(nmea_data->ptr) = '\0';
    nmea_data->ptr = nmea_data->buffer;

    if ((*(nmea_data->buffer) == '$')
        && gnss_max_m10s_test_checksum(nmea_data->buffer)) {
      /// Valid message
      const char *data;
      if (nmea_data->buffer[1] == 'G') {
        nmea_data->talker_id = nmea_data->buffer[2];
        data = gnss_max_m10s_parse_field(&nmea_data->buffer[3],
                                         &nmea_data->message_id[0],
                                         sizeof(nmea_data->message_id));
      } else {
        nmea_data->talker_id = '\0';
        data = gnss_max_m10s_parse_field(&nmea_data->buffer[1],
                                         &nmea_data->message_id[0],
                                         sizeof(nmea_data->message_id));
      }

      if ((data != NULL) && (strcmp(&nmea_data->message_id[0], "GGA") == 0)) {
        return gnss_max_m10s_process_gga(data, nmea_data);
      } else if ((data != NULL)
                 && (strcmp(&nmea_data->message_id[0], "RMC") == 0)) {
        return gnss_max_m10s_process_rmc(data, nmea_data);
      }
    }

    /// Return true for a complete, non-empty, sentence (even if not a valid one).
    return *(nmea_data->buffer) != '\0';
  } else {
    *(nmea_data->ptr) = c;
    if (nmea_data->ptr < &nmea_data->buffer[nmea_data->buffer_len - 1]) {
      ++nmea_data->ptr;
    }
  }

  return false;
}

static const char * gnss_max_m10s_parse_time(const char *s,
                                             sl_max_m10s_nmea_data_t *nmea_data)
{
  if (*s == ',') {
    return gnss_max_m10s_skip_field(s);
  }

  nmea_data->hour = gnss_max_m10s_parse_unsigned_int(s, 2);
  nmea_data->minute = gnss_max_m10s_parse_unsigned_int(s + 2, 2);
  nmea_data->second = gnss_max_m10s_parse_unsigned_int(s + 4, 2);

  return gnss_max_m10s_skip_field(s + 9);
}

static const char * gnss_max_m10s_parse_date(const char *s,
                                             sl_max_m10s_nmea_data_t *nmea_data)
{
  if (*s == ',') {
    return gnss_max_m10s_skip_field(s);
  }

  nmea_data->day = gnss_max_m10s_parse_unsigned_int(s, 2);
  nmea_data->month = gnss_max_m10s_parse_unsigned_int(s + 2, 2);
  nmea_data->year = gnss_max_m10s_parse_unsigned_int(s + 4, 2) + 2000;

  return gnss_max_m10s_skip_field(s + 6);
}

static bool gnss_max_m10s_process_gga(const char *s,
                                      sl_max_m10s_nmea_data_t *nmea_data)
{
  nmea_data->nav_system = nmea_data->talker_id;

  s = gnss_max_m10s_parse_time(s, nmea_data);

  if (s == NULL) {
    return false;
  }

  nmea_data->latitude = gnss_max_m10s_parse_degree_minute(s, 2, &s);
  if (s == NULL) {
    return false;
  }

  if (*s == ',') {
    ++s;
  } else {
    if (*s == 'S') {
      nmea_data->latitude *= -1;
    }
    s += 2; /// Skip N/S and comma
  }

  nmea_data->longitude = gnss_max_m10s_parse_degree_minute(s, 3, &s);
  if (s == NULL) {
    return false;
  }

  if (*s == ',') {
    ++s;
  } else {
    if (*s == 'W') {
      nmea_data->longitude *= -1;
    }
    s += 2; /// Skip E/W and comma
  }

  nmea_data->is_valid = (*s >= '1' && *s <= '5');

  long tmp = gnss_max_m10s_parse_float((unsigned char *)s, 0, &s, NULL);
  nmea_data->num_sat = (tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));

  if (s == NULL) {
    return false;
  }

  tmp = gnss_max_m10s_parse_float((unsigned char *)s, 1, &s, NULL);
  nmea_data->hdop = (tmp > 255 || tmp < 0 ? 255 : tmp);

  if (s == NULL) {
    return false;
  }

  bool result_valid;
  nmea_data->altitude = gnss_max_m10s_parse_float((unsigned char *)s,
                                                  3,
                                                  &s,
                                                  &result_valid);

  if (s == NULL) {
    return false;
  }

  if (result_valid) {
    nmea_data->altitude_valid = true;
  }
  s += 2; /// Skip M and comma
  nmea_data->geoid_height = gnss_max_m10s_parse_float((unsigned char *)s,
                                                      3,
                                                      &s,
                                                      &result_valid);

  if (s == NULL) {
    return false;
  }

  if (result_valid) {
    nmea_data->geoid_height_valid = true;
  }
  return true;
}

static bool gnss_max_m10s_process_rmc(const char *s,
                                      sl_max_m10s_nmea_data_t *nmea_data)
{
  nmea_data->nav_system = nmea_data->talker_id;

  s = gnss_max_m10s_parse_time(s, nmea_data);

  if (s == NULL) {
    return false;
  }

  nmea_data->is_valid = (*s == 'A');
  s += 2; /// Skip validity and comma

  nmea_data->latitude = gnss_max_m10s_parse_degree_minute(s, 2, &s);
  if (s == NULL) {
    return false;
  }

  if (*s == ',') {
    ++s;
  } else {
    if (*s == 'S') {
      nmea_data->latitude *= -1;
    }

    s += 2; /// Skip N/S and comma
  }

  nmea_data->longitude = gnss_max_m10s_parse_degree_minute(s, 3, &s);

  if (s == NULL) {
    return false;
  }

  if (*s == ',') {
    ++s;
  } else {
    if (*s == 'W') {
      nmea_data->longitude *= -1;
    }

    s += 2; /// Skip E/W and comma
  }

  nmea_data->speed = gnss_max_m10s_parse_float((unsigned char *)s, 3, &s, NULL);

  if (s == NULL) {
    return false;
  }

  nmea_data->course =
    gnss_max_m10s_parse_float((unsigned char *)s, 3, &s, NULL);

  if (s == NULL) {
    return false;
  }

  s = gnss_max_m10s_parse_date(s, nmea_data);
  return true;
}
