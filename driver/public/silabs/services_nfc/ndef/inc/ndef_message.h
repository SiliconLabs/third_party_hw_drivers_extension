/***************************************************************************//**
 * @file   ndef_message.h
 * @brief  Definitions, types and APIs for NDEF Message.
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

#ifndef __NDEF_MESSAGE_H__
#define __NDEF_MESSAGE_H__

#include <stdint.h>
#include <stdbool.h>
#include "ndef_record.h"

/***************************************************************************//**
 * @addtogroup NFC Library
 * @brief Definitions, types and APIs for NDEF Message.
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/// NDEF message validation error type.
typedef enum {
  ndefMessageValid = 0
} ndef_message_validate_error_t;

/// NDEF message encode error type.
typedef enum {
  ndefMessageEncodeSuccess = 0,
  ndefMessageEncodeFail    = -1
} ndef_message_encode_error_t;

/// NDEF message encode result type.
typedef struct {
  uint32_t                    size;
  ndef_message_encode_error_t err;
}ndef_message_encode_result_t;

/// NDEF message validation result type.
typedef struct {
  uint32_t                      record_count;
  ndef_message_validate_error_t err;
} ndef_message_validate_result_t;

ndef_message_encode_result_t ndef_message_encode (uint8_t *message_buff,
                                                  ndef_record_t *message);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup NFC Library) */
#endif
