/***************************************************************************//**
 * @file enocean_proxy_api.h
 * @brief enocean_proxy_api header file.
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef ENOCEAN_PROXY_API_H
#define ENOCEAN_PROXY_API_H

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------
#include "sl_bt_api.h"

// Start listening for commissioning messages. If a valid commissioning
// message is received within one minute of calling this function, the sending
// switch is bound to the element given in the parameter.
sl_status_t enocean_proxy_enter_commissioning_mode(uint8_t element_index);

// A helper function which returns the element index of
// Returns 0xffff if no unused elements available
uint16_t enocean_proxy_get_lowest_unused_element_index(void);

#endif /* ENOCEAN_PROXY_API_H */
