/***************************************************************************//**
 * @file   ndef_record.h
 * @brief  APIs and defines for NDEF record.
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

#ifndef __NDEF_RECORD_H__
#define __NDEF_RECORD_H__

#include <stdint.h>
#include <stdbool.h>

/***************************************************************************//**
 * @addtogroup NFC Library
 * @brief APIs and defines for NDEF record.
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* some NFC forum global well-known types */
#define NDEF_RTD_WKT_GLOBAL_SMART_POSTER         "Sp" // urn:nfc:wkt:Sp
#define NDEF_RTD_WKT_GLOBAL_TEXT                 "T" // urn:nfc:wkt:T
#define NDEF_RTD_WKT_GLOBAL_URI                  "U" // urn:nfc:wkt:U
#define NDEF_RTD_WKT_GLOBAL_SIGNATURE            "Sig" // urn:nfc:wkt:Sig

/* URI Identifier Code */
#define NDEF_RTD_WKT_URI_PREFIX_NONE             (0x00)
#define NDEF_RTD_WKT_URI_PREFIX_HTTP_WWWDOT      (0x01)
#define NDEF_RTD_WKT_URI_PREFIX_HTTPS_WWWDOT     (0x02)
#define NDEF_RTD_WKT_URI_PREFIX_HTTP             (0x03)
#define NDEF_RTD_WKT_URI_PREFIX_HTTPS            (0x04)
#define NDEF_RTD_WKT_URI_PREFIX_TEL              (0x05)
#define NDEF_RTD_WKT_URI_PREFIX_MAILTO           (0x06)
#define NDEF_RTD_WKT_URI_PREFIX_FTP_ANONAT       (0x07)
#define NDEF_RTD_WKT_URI_PREFIX_FTP_FTPDOT       (0x08)
#define NDEF_RTD_WKT_URI_PREFIX_FTPS             (0x09)
#define NDEF_RTD_WKT_URI_PREFIX_SFTP             (0x0A)
#define NDEF_RTD_WKT_URI_PREFIX_SMB              (0x0B)
#define NDEF_RTD_WKT_URI_PREFIX_NFS              (0x0C)
#define NDEF_RTD_WKT_URI_PREFIX_FTP              (0x0D)
#define NDEF_RTD_WKT_URI_PREFIX_DAV              (0x0E)
#define NDEF_RTD_WKT_URI_PREFIX_NEWS             (0x0F)
#define NDEF_RTD_WKT_URI_PREFIX_TELNET           (0x10)
#define NDEF_RTD_WKT_URI_PREFIX_IMAP             (0x11)
#define NDEF_RTD_WKT_URI_PREFIX_RTSP             (0x12)
#define NDEF_RTD_WKT_URI_PREFIX_URN              (0x13)
#define NDEF_RTD_WKT_URI_PREFIX_POP              (0x14)
#define NDEF_RTD_WKT_URI_PREFIX_SIP              (0x15)
#define NDEF_RTD_WKT_URI_PREFIX_SIPS             (0x16)
#define NDEF_RTD_WKT_URI_PREFIX_TFTP             (0x17)
#define NDEF_RTD_WKT_URI_PREFIX_BTSPP            (0x18)
#define NDEF_RTD_WKT_URI_PREFIX_BTL2CAP          (0x19)
#define NDEF_RTD_WKT_URI_PREFIX_BTGOEP           (0x1A)
#define NDEF_RTD_WKT_URI_PREFIX_TCPOBEX          (0x1B)
#define NDEF_RTD_WKT_URI_PREFIX_IRDAOBEX         (0x1C)
#define NDEF_RTD_WKT_URI_PREFIX_FILE             (0x1D)
#define NDEF_RTD_WKT_URI_PREFIX_URN_EPC_ID       (0x1E)
#define NDEF_RTD_WKT_URI_PREFIX_URN_EPC_TAG      (0x1F)
#define NDEF_RTD_WKT_URI_PREFIX_URN_EPC_PAT      (0x20)
#define NDEF_RTD_WKT_URI_PREFIX_URN_EPC_RAW      (0x21)
#define NDEF_RTD_WKT_URI_PREFIX_URN_EPC          (0x22)
#define NDEF_RTD_WKT_URI_PREFIX_URN_NFC          (0x23)

#define NDEF_RECORD_MAX_LENGTH                   (1000)

#define NDEF_RECORD_HEADER_MB_SHIFT              (7)
#define NDEF_RECORD_HEADER_MB_M                  (0x01 << \
                                                  NDEF_RECORD_HEADER_MB_SHIFT)
#define NDEF_RECORD_HEADER_ME_SHIFT              (6)
#define NDEF_RECORD_HEADER_ME_M                  (0x01 << \
                                                  NDEF_RECORD_HEADER_ME_SHIFT)
#define NDEF_RECORD_HEADER_CF_SHIFT              (5)
#define NDEF_RECORD_HEADER_CF_M                  (0x01 << \
                                                  NDEF_RECORD_HEADER_CF_SHIFT)
#define NDEF_RECORD_HEADER_SR_SHIFT              (4)
#define NDEF_RECORD_HEADER_SR_M                  (0x01 << \
                                                  NDEF_RECORD_HEADER_SR_SHIFT)
#define NDEF_RECORD_HEADER_IL_SHIFT              (3)
#define NDEF_RECORD_HEADER_IL_M                  (0x01 << \
                                                  NDEF_RECORD_HEADER_IL_SHIFT)
#define NDEF_RECORD_HEADER_TNF_SHIFT             (0)
#define NDEF_RECORD_HEADER_TNF_M                 (0x07 << \
                                                  NDEF_RECORD_HEADER_TNF_SHIFT)

#define BIT_31_TO_24_M                           (0xFF000000)
#define GET_BIT_31_TO_24(num)                    ((num & BIT_31_TO_24_M) >> 24)
#define BIT_23_TO_16_M                           (0x00FF0000)
#define GET_BIT_23_TO_16(num)                    ((num & BIT_23_TO_16_M) >> 16)
#define BIT_15_TO_8_M                            (0x0000FF00)
#define GET_BIT_15_TO_8(num)                     ((num & BIT_15_TO_8_M) >> 8)
#define BIT_7_TO_0_M                             (0x000000FF)
#define GET_BIT_7_TO_0(num)                      ((num & BIT_7_TO_0_M) >> 0)

/*
 * @brief TNF type definition
 */
typedef enum {
  ndefTnfEmpty       = 0x00,
  ndefTnfWellKnown   = 0x01,
  ndefTnfMIMEMedia   = 0x02,
  ndefTnfAbsoluteURI = 0x03,
  ndefTnfExternal    = 0x04,
  ndefTnfUnknown     = 0x05,
  ndefTnfUnchanged   = 0x06,
  ndefTnfReserved    = 0x07
} ndef_tnf_t;

typedef enum {
  ndefRecordEncodeCompleted = 0,
  ndefRecordEncodeOverflow  = -1
} ndef_record_encode_error_t;

typedef struct {
  bool    mb;     /* Message Begin */
  bool    me;     /* Message End */
  bool    cf;     /* Chunk Flag */
  bool    sr;     /* Short Record */
  bool    il;     /* Id Length */
  uint8_t tnf;    /* Type Name Format */
} ndef_record_header_t;

/*
 *  Bit 7     6       5       4       3       2       1       0
 * ------  ------  ------  ------  ------  ------  ------  ------
 * [ MB ]  [ ME ]  [ CF ]  [ SR ]  [ IL ]  [        TNF         ]
 *
 * [                        TYPE LENGTH                         ]
 *
 * [                       PAYLOAD LENGTH 3                     ]
 *
 * [                       PAYLOAD LENGTH 2                     ]
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
 * [ MB ]  [ ME ]  [ CF ]  [ SR ]  [ IL ]  [        TNF         ]
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

typedef struct {
  /* header */
  ndef_record_header_t header;

  /* length */
  uint8_t type_length;
  uint32_t payload_length;
  uint8_t id_length;

  /* payload */
  uint8_t *type;
  uint8_t *id;
  uint8_t *payload;
} ndef_record_t;

typedef struct {
  uint32_t                    size;
  ndef_record_encode_error_t  err;
}ndef_record_encode_result_t;

ndef_record_encode_result_t ndef_record_encode (ndef_record_t record,
                                                uint8_t *record_buff);

void ndef_record_decode (ndef_record_t *record, uint8_t *record_buff);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup NFC Library) */
#endif
