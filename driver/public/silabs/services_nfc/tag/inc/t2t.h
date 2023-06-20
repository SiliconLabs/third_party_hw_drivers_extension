/***************************************************************************//**
 * @file   t2t.h
 * @brief  Type 2 Tag related definitions.
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

#ifndef __T2T_H__
#define __T2T_H__

#define T2T_CMD_READ                                (0x30)
#define T2T_CMD_WRITE                               (0xA2)
#define T2T_CMD_SEC_SEL                             (0xC2)

#define T2T_RSP_ACK                                 (0x0A)

#define T2T_CC0                                     (0xE1)
#define T2T_CC1_VERSION_1_0                         (0x10)

#define NFC_T2T_NULL_TLV                            (0x00)
#define NFC_T2T_LOCK_CONTROL_TLV                    (0x01)
#define NFC_T2T_MEMORY_CONTRO_TLV                   (0x02)
#define NFC_T2T_NDEF_MESSAGE_TLV                    (0x03)
#define NFC_T2T_PROPRIETARY_TLV                     (0xFD)
#define NFC_T2T_TERMINATOR_TLV                      (0xFE)

#endif
