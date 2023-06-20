/***************************************************************************//**
 * @file   nfc_tlv.c
 * @brief  Implementation for using Type Length Value.
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
#include <string.h>
#include <stdbool.h>
#include "nfc_tlv.h"

/**************************************************************************//**
 * @brief
 *  Encode a TLV.
 *
 * @param[out] tlv_buff
 *  Buffer to hold encoded TLV
 *
 * @param[in] tlv_buff_size
 *  Size of TLV buffer
 *
 * @param[in] type
 *  Type field
 *
 * @param[in] length
 *  Length field
 *
 * @param[in] value
 *  Value field
 *
 * @returns
 *  Any error code.
 *****************************************************************************/
tlv_encode_error_t tlv_encode(uint8_t *tlv_buff,
                              uint32_t tlv_buff_size,
                              uint8_t  type,
                              uint32_t length,
                              uint8_t *value)
{
  // check if TLV buffer is big enough
  if (tlv_buff_size < (length + 2)) {
    return tlvEncodeBufferOverflow;
  }
  // encode NDEF TLV Type
  tlv_buff[0] = type;
  // encode NDEF TLV length
  tlv_buff[1] = length;
  // encode NDEF TLV value (NDEF message)
  memcpy(&tlv_buff[2], value, length);
  // return completion code
  return tlvEncodeCompleted;
}
