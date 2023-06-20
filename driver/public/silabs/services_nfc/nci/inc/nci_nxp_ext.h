/***************************************************************************//**
 * @file   nci_nxp_ext.h
 * @brief  NXP proprietary extension for NCI.
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

#ifndef __NCI_NXP_EXT_H__
#define __NCI_NXP_EXT_H__

#include "nci_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup NFC Library
 * @brief NXP proprietary extension for NCI.
 * @{
 ******************************************************************************/

typedef enum {
  nci_nxp_rf_protocol_15693                           = 0x06,
  nci_nxp_rf_protocol_mifare_classic                  = 0x80,
  nci_nxp_rf_protocol_kovio                           = 0x8A
} nci_nxp_rf_protocol_t;

typedef struct {
  uint8_t  status;
  uint8_t *fw_build_num;
} nci_proprietary_nxp_act_rsp_t;

typedef union {
  nci_proprietary_nxp_act_rsp_t proprietary_nxp_act_rsp;
} nci_proprietary_data_t;

nci_err_t nci_proprietary_nxp_act (void);

nci_err_t nci_proprietary_nxp_rf_pres_check (void);

nci_err_t nci_proprietary_nxp_test_get_register (void);

/** @} (end addtogroup NFC Library) */

#ifdef __cplusplus
}
#endif
#endif
