/***************************************************************************//**
 * @file   t4t.h
 * @brief  Type 4 Tag related definitions and APIs.
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

#ifndef __T4T_H__
#define __T4T_H__
#include <stdint.h>

#define T4T_NDEF_FILE_CTRL_TLV_TYPE                  (0x04)
#define T4T_PROPRIETARY_FILE_CTRL_TLV_TYPE           (0x05)
#define T4T_ENDEF_FILE_CTRL_TLV_TYPE                 (0x06)
#define T4T_EPROPRIETARY_FILE_CTRL_TLV_TYPE          (0x07)

#define T4T_NDEF_FILE_CTRL_TLV_LENGTH                (6)

///
typedef struct {
  uint16_t fid;
  uint16_t size;
  uint8_t  read_access;
  uint8_t  write_access;
} t4t_ndef_file_ctrl_t;

///
typedef struct {
  uint16_t fid;
  uint32_t size;
  uint8_t  read_access;
  uint8_t  write_access;
} t4t_endef_file_ctrl_t;

///
typedef struct {
  uint16_t fid;
  uint16_t size;
  uint8_t  read_access;
  uint8_t  write_access;
} t4t_proprietary_file_ctrl_t;

///
typedef struct {
  uint16_t fid;
  uint32_t size;
  uint8_t  read_access;
  uint8_t  write_access;
} t4t_eproprietary_file_ctrl_t;

///
typedef struct {
  uint8_t                       t4t_vno;
  uint16_t                      mle;
  uint16_t                      mlc;
  t4t_ndef_file_ctrl_t          ndef_file_ctrl;
  t4t_endef_file_ctrl_t         endef_file_ctrl;
  t4t_proprietary_file_ctrl_t  *prop_file_ctrls;
  t4t_eproprietary_file_ctrl_t *eprop_file_ctrls;
} t4t_cc_file_t;

#endif
