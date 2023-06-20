/***************************************************************************//**
 * @file nci_tml.c
 * @brief Implementation for using NCI Transport Mapping Layer
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

#include "sl_status.h"
#include "nci_tml.h"
#include "mikroe_pn7150.h"

/**************************************************************************//**
 * @brief
 *  NCI TML receive function wrapper for PN71x0 I2C.
 *
 * @param[out] packet
 *  Packet buffer to hold the received packet.
 *
 * @returns
 *  Any error code.
 *****************************************************************************/
nci_tml_err_t nci_tml_receive(uint8_t *packet)
{
  sl_status_t ret;

  ret = mikroe_pn7150_generic_read(packet, &packet[3]);
  if (ret != SL_STATUS_OK) {
    return nci_tml_err_comm_bus;
  }

  return nci_tml_err_none;
}

/**************************************************************************//**
 * @brief
 *  NCI TML transceive function wrapper for PN71x0 I2C.
 *
 * @param[in] packet
 *  Packet buffer to hold the packet to be sent.
 *
 * @returns
 *  Any error code.
 *****************************************************************************/
nci_tml_err_t nci_tml_transceive(uint8_t *packet)
{
  sl_status_t ret;

  ret = mikroe_pn7150_generic_write(packet, packet[2] + 3);
  if (ret != SL_STATUS_OK) {
    return nci_tml_err_comm_bus;
  }

  return nci_tml_err_none;
}
