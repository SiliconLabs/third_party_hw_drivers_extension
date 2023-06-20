/***************************************************************************//**
 * @file   ndef_message.c
 * @brief  Implementation of NDEF Message level functions.
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

#include <stdint.h>
#include <stdbool.h>
#include "ndef_message.h"
#include "ndef_record.h"

/**************************************************************************//**
 * @brief
 *  Validate an NDEF message.
 *
 * @param[in] message
 *  NDEF message to be validated
 *
 * @returns
 *  Result of validation
 *
 * @warning
 *  This function is not yet completed.
 *****************************************************************************/
ndef_message_validate_result_t ndef_message_validate(ndef_record_t *message)
{
  (void) message;
  ndef_message_validate_result_t result;

  // Todo

  result.err = ndefMessageValid;
  result.record_count = 1;

  return result;
}

/**************************************************************************//**
 * @brief
 *  Encode an NDEF message into raw data array.
 *
 * @param[out] message_buff
 *  Buffer to hold the encoded NDEF message
 *
 * @param[in] message
 *  NDEF message to be encoded
 *
 * @returns
 *  Result of the encoding, including error.
 *****************************************************************************/
ndef_message_encode_result_t ndef_message_encode(uint8_t *message_buff,
                                                 ndef_record_t *message)
{
  ndef_message_encode_result_t result = { .size = 0 };

  ndef_message_validate_result_t validate_result;

  /* Validate message layout and get a record count. */
  validate_result = ndef_message_validate(message);

  /* Check message validation result. */
  if (validate_result.err == ndefMessageValid) {
    uint32_t index;

    /* Encode records. */
    for (index = 0; index < validate_result.record_count; index++) {
      ndef_record_encode_result_t record_encode_result
        = ndef_record_encode(message[index], &message_buff[result.size]);

      /* Encode a record. */
      if (record_encode_result.err != ndefRecordEncodeCompleted) {
        /* Encode record failed. */
        result.err = ndefMessageEncodeFail;
        return result;
      }
      result.size += record_encode_result.size;
    }
  } else {
    /* Validate message failed. */
    result.err = ndefMessageEncodeFail;
    return result;
  }

  /* Complete encoding a message. */
  result.err = ndefMessageEncodeSuccess;
  return result;
}

/**************************************************************************//**
 * @brief
 *  Decode an NDEF message from raw data array.
 *
 * @param[in] message_buff
 *  Raw NDEF message data to be decoded
 *
 * @param[out] message
 *  Struct to hold the decoded NDEF message
 *
 * @returns
 *  Result of the decoding, including error.
 *
 * @warning
 *  This function is not yet completed.
 *****************************************************************************/
void ndef_message_decode(uint8_t *ndef_msg_buff, ndef_record_t *message)
{
  (void) ndef_msg_buff;
  (void) message;
  // Todo
}
