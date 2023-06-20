/***************************************************************************//**
 * @file   nci_dbg.h
 * @brief  Debug helper functions for NCI.
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

#ifndef __NCI_DBG_H__
#define __NCI_DBG_H__

#include "nci_config.h"

#if (NFC_NCI_DBG == 1)

#include <stdio.h>
#include "nfc_serial.h"

#define nci_log(...)                    printf(__VA_ARGS__)
#define nci_log_ln(...)                 { \
    printf(__VA_ARGS__);                  \
    printf("\r\n");                       \
}

#define nci_evt_log(header)             nci_evt_print(header)

void nci_evt_print (uint8_t header);

#if (NFC_NCI_TML_DBG == 1)

#define nci_tml_log(...)                printf(__VA_ARGS__)
#define nci_tml_log_ln(...)             { \
    printf(__VA_ARGS__);                  \
    printf("\r\n");                       \
}

#define nci_tml_packet_log(packet, len) serial_put_hex(packet, len)

#else

#define nci_tml_log(...)
#define nci_tml_log_ln(...)

#define nci_tml_packet_log(packet, len)

#endif

#else

#define nci_log(...)
#define nci_log_ln(...)

#define nci_evt_log(hdeaer)

#define nci_tml_log(...)
#define nci_tml_log_ln(...)

#define nci_tml_packet_log(packet, len)

#endif

#endif
