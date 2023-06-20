/***************************************************************************//**
 * @file   nfc_serial.c
 * @brief  Implementation for custom serial output.
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

#include <stdio.h>
#include "nfc_serial.h"

/***************************************************************************//**
 * @brief
 *  Prints an array of hexadecimal value.
 *
 * @param[in] data
 *  Pointer to the data
 *
 * @param[in] length
 *  Data length in bytes
 ******************************************************************************/
void serial_put_hex(const uint8_t *data, const uint32_t length)
{
  for (uint8_t i = 0; i < length; i++) {
    printf(" %02x", data[i]);
  }
}

/***************************************************************************//**
 * @brief
 *  Prints an array of hexadecimal value in plain characters.
 *
 * @param[in] data
 *  Pointer to the data
 *
 * @param[in] length
 *  Data length in bytes
 ******************************************************************************/
void serial_put_ascii(const uint8_t *data, const uint32_t length)
{
  for (uint8_t i = 0; i < length; i++) {
    char c = data[i];
    if ((c <= 0x1f) || (c > 0x7f)) {
      printf(".");
    } else {
      printf("%c", c);
    }
  }
}

/***************************************************************************//**
 * @brief
 *  Prints an array of hexadecimal value in plain characters, along with
 *  the char equivalents in the following format:
 *
 *  00 00 00 00 00 00  ......
 *
 * @param[in] data
 *  Pointer to the data
 *
 * @param[in] length
 *  Data length in bytes
 ******************************************************************************/
void serial_put_hex_and_ascii(const uint8_t *data, const uint32_t length)
{
  serial_put_hex(data, length);
  printf("    ");
  serial_put_ascii(data, length);
}
