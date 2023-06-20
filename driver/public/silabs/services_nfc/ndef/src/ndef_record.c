/***************************************************************************//**
 * @file   ndef_record.c
 * @brief  Implementation of NDEF record level functions.
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
#include <stdbool.h>
#include "ndef_record.h"

/**************************************************************************//**
 * @brief
 *  Encode NDFE record MB, ME, CF, SR, IL and TNF fields..
 *
 * @param[in] header
 *  header field specific definitions
 *
 * @returns
 *  Encoded header byte.
 *****************************************************************************/
static uint8_t ndef_record_header_encode(ndef_record_header_t header)
{
  uint8_t header_byte = 0;

  /* MB(Message Begin) bit */
  if (header.mb) {
    header_byte |= NDEF_RECORD_HEADER_MB_M;
  }

  /* ME(Message End) bit */
  if (header.me) {
    header_byte |= NDEF_RECORD_HEADER_ME_M;
  }

  /* CF(Chunk Flag) bit */
  if (header.cf) {
    header_byte |= NDEF_RECORD_HEADER_CF_M;
  }

  /* SR(Short Record) bit */
  if (header.sr) {
    header_byte |= NDEF_RECORD_HEADER_SR_M;
  }

  /* IL(Id Length) bit */
  if (header.il) {
    header_byte |= NDEF_RECORD_HEADER_IL_M;
  }

  /* set TNF(Type Name Format) */
  header_byte |= (header.tnf & NDEF_RECORD_HEADER_TNF_M);

  return header_byte;
}

/**************************************************************************//**
 * @brief
 *  Decode raw NDEF header byte and store it into NDEF header struct.
 *
 * @param[out] header
 *  NDEF header struct to save the decoded header info
 *
 * @param[in] header_byte
 *  Raw header byte to be decoded
 *****************************************************************************/
static void ndef_record_header_decode(ndef_record_header_t *header,
                                      uint8_t header_byte)
{
  /* MB(Message Begin) bit */
  if ((header_byte & NDEF_RECORD_HEADER_MB_M)) {
    header->mb = true;
  }

  /* ME(Message End) bit */
  if ((header_byte & NDEF_RECORD_HEADER_ME_M)) {
    header->me = true;
  }

  /* CF(Chunk Flag) bit */
  if ((header_byte & NDEF_RECORD_HEADER_CF_M)) {
    header->cf = true;
  }

  /* SR(Short Record) bit */
  if ((header_byte & NDEF_RECORD_HEADER_SR_M)) {
    header->sr = true;
  }

  /* IL(Id Length) bit */
  if ((header_byte & NDEF_RECORD_HEADER_IL_M)) {
    header->il = true;
  }

  /* set TNF(Type Name Format) */
  header->tnf = (header_byte & NDEF_RECORD_HEADER_TNF_M);
}

/**************************************************************************//**
 * @brief
 *  Encode a NDEF record and store in a data buffer pointert.
 *
 * @param[in] record
 *  Record to be encoded
 *
 * @param[out] record_buffer
 *  Pointer to store the encoded NDEF record
 *
 * @returns
 *  Result of the record encode process.
 *****************************************************************************/
ndef_record_encode_result_t ndef_record_encode(ndef_record_t record,
                                               uint8_t *record_buff)
{
  /*
   *  Bit 7     6       5       4       3       2       1       0
   * ------  ------  ------  ------  ------  ------  ------  ------
   * [ MB ]  [ ME ]  [ CF ]  [ SR ]  [ IL ]  [        TNF         ] => HEADER
   *
   * [                        TYPE LENGTH                         ]
   *
   * [                       PAYLOAD LENGTH 3                     ]
   *
   * [                       PAYLOAD LENGTH 3                     ]
   *
   * [                       PAYLOAD LENGTH 1                     ]
   *
   * [                       PAYLOAD LENGTH 0                     ]
   *
   * [                         ID LENGTH                          ]
   *
   * [                           TYPE                             ]
   *
   * [                            ID                              ]
   *
   * [                          PAYLOAD                           ]
   */

  /*
   *  Bit 7     6       5       4       3       2       1       0
   * ------  ------  ------  ------  ------  ------  ------  ------
   * [ MB ]  [ ME ]  [ CF ]  [ SR ]  [ IL ]  [        TNF         ] => HEADER
   *
   * [                        TYPE LENGTH                         ]
   *
   * [                       PAYLOAD LENGTH                       ]
   *
   * [                         ID LENGTH                          ]
   *
   * [                           TYPE                             ]
   *
   * [                            ID                              ]
   *
   * [                          PAYLOAD                           ]
   */

  ndef_record_encode_result_t result;

  uint32_t index = 0;

  /* encode HEADER */
  record_buff[index++] = ndef_record_header_encode(record.header);

  /* encode TYPE LENGTH */
  record_buff[index++] = record.type_length;

  /* encode PAYLOAD LENGTH */
  /* Check if is short record. */
  if (record.header.sr) {
    result.size =
      (record.type_length + record.id_length + record.payload_length + 3);

    /* Check if NDEF record too long, the is a custom macro in case size needs
     *   to be constrained. */
    if (result.size > NDEF_RECORD_MAX_LENGTH) {
      result.err = ndefRecordEncodeOverflow;
      return result;
    }

    /* Write payload length into the record. */
    record_buff[index++] = (uint8_t) record.payload_length;
  } else {
    result.size =
      (record.type_length + record.id_length + record.payload_length + 6);

    /* Check if NDEF record too long, the is a custom macro in case size needs
     *   to be constrained. */
    if (result.size > NDEF_RECORD_MAX_LENGTH) {
      result.err = ndefRecordEncodeOverflow;
      return result;
    }

    /* Write payload length into the record. */
    record_buff[index++] = (uint8_t) GET_BIT_31_TO_24(record.payload_length);
    record_buff[index++] = (uint8_t) GET_BIT_23_TO_16(record.payload_length);
    record_buff[index++] = (uint8_t) GET_BIT_15_TO_8(record.payload_length);
    record_buff[index++] = (uint8_t) GET_BIT_7_TO_0(record.payload_length);
  }

  /* Encode ID LENGTH. */
  if (record.header.il) {
    result.size++;
    record_buff[index++] = record.id_length;
  }

  /* Encode TYPE. */
  memcpy(&record_buff[index], record.type, record.type_length);
  index += record.type_length;

  /* Encode ID. */
  memcpy(&record_buff[index], record.id, record.id_length);
  index += record.id_length;

  /* Encode PAYLOAD. */
  memcpy(&record_buff[index], record.payload, record.payload_length);

  /* Set error to encoded completed (no error). */
  result.err = ndefRecordEncodeCompleted;
  return result;
}

/**************************************************************************//**
 * @brief
 *  Decode a NDEF record from a raw buffer and put the result into NDEF struct.
 *
 * @param[out] record
 *  Record to be returned
 *
 * @param[in] record_buffer
 *  Pointer to location that stores the NDEF record to be decoded
 *****************************************************************************/
void ndef_record_decode(ndef_record_t *record, uint8_t *record_buff)
{
  /*
   *  Bit 7     6       5       4       3       2       1       0
   * ------  ------  ------  ------  ------  ------  ------  ------
   * [ MB ]  [ ME ]  [ CF ]  [ SR ]  [ IL ]  [        TNF         ] => HEADER
   *
   * [                        TYPE LENGTH                         ]
   *
   * [                       PAYLOAD LENGTH 3                     ]
   *
   * [                       PAYLOAD LENGTH 3                     ]
   *
   * [                       PAYLOAD LENGTH 1                     ]
   *
   * [                       PAYLOAD LENGTH 0                     ]
   *
   * [                         ID LENGTH                          ]
   *
   * [                           TYPE                             ]
   *
   * [                            ID                              ]
   *
   * [                          PAYLOAD                           ]
   */

  /*
   *  Bit 7     6       5       4       3       2       1       0
   * ------  ------  ------  ------  ------  ------  ------  ------
   * [ MB ]  [ ME ]  [ CF ]  [ SR ]  [ IL ]  [        TNF         ] => HEADER
   *
   * [                        TYPE LENGTH                         ]
   *
   * [                       PAYLOAD LENGTH                       ]
   *
   * [                         ID LENGTH                          ]
   *
   * [                           TYPE                             ]
   *
   * [                            ID                              ]
   *
   * [                          PAYLOAD                           ]
   */

  /* Initialize index for navigating through the record buffer. */
  uint32_t index = 0;

  /* Decode header. */
  ndef_record_header_decode(&record->header, record_buff[index++]);

  /* Set TYPE LENGTH. */
  record->type_length = record_buff[index++];

  /* Check if it's a short record. */
  if (record->header.sr) {
    /* Set PAYLOAD LENGTH. */
    record->payload_length = record_buff[index++];
  }

  /* Regular record. */
  else {
    /* Add PAYLOAD LENGTH 3 */
    record->payload_length += record_buff[index++] << 24;

    /* Add PAYLOAD LENGTH 2 */
    record->payload_length += record_buff[index++] << 16;

    /* Add PAYLOAD LENGTH 1 */
    record->payload_length += record_buff[index++] << 8;

    /* Add PAYLOAD LENGTH 0 */
    record->payload_length += record_buff[index++];
  }

  /* Set ID LENGTH. */
  record->id_length = record_buff[index++];

  /* Check TYPE LENGTH and set TYPE if necessary.  */
  if (record->type_length > 0) {
    /* Set TYPE. */
    memcpy(record->type, &record_buff[index], record->type_length);

    /* Increment index base on TYPE LENGTH. */
    index += record->type_length;
  }

  /* Check ID LENGTH and set ID if necessary.  */
  if (record->id_length > 0) {
    /* Set ID. */
    memcpy(record->id, &record_buff[index], record->id_length);

    /* Increment index base on ID LENGTH. */
    index += record->id_length;
  }

  /* Check PAYLOAD LENGTH and set PAYLOAD if necessary.  */
  if (record->payload_length > 0) {
    /* Set PAYLOAD. */
    memcpy(record->payload, &record_buff[index], record->payload_length);
  }
}
