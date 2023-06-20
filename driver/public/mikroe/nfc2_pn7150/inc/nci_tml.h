#ifndef __NCI_TML_H__
#define __NCI_TML_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup NFC Library
 * @brief Types and definitions for NCI TML.
 * @{
 ******************************************************************************/

typedef enum {
  nci_tml_err_none            = 0x00,
  nci_tml_err_comm_bus        = 0x01
} nci_tml_err_t;

/**************************************************************************//**
 * @brief
 *  NCI TML transceive function wrapper for PN7150 I2C.
 *
 * @param[in] packet
 *  Packet buffer to hold the packet to be sent.
 *
 * @returns
 *  Any error code.
 *****************************************************************************/
nci_tml_err_t nci_tml_transceive(uint8_t *packet);

/**************************************************************************//**
 * @brief
 *  NCI TML receive function wrapper for PN7150 I2C.
 *
 * @param[out] packet
 *  Packet buffer to hold the received packet.
 *
 * @returns
 *  Any error code.
 *****************************************************************************/
nci_tml_err_t nci_tml_receive(uint8_t *packet);

/** @} (end addtogroup NFC Library) */

#ifdef __cplusplus
}
#endif
#endif
