/***************************************************************************//**
 * @file adafruit_is31fl3741_params.h
 * @brief Adafruit RGB LED parameters header file.
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

#ifndef ADAFRUIT_IS31FL3741_PARAMS_H_
#define ADAFRUIT_IS31FL3741_PARAMS_H_

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include "adafruit_is31fl3741_config.h"

// -----------------------------------------------------------------------------
//                               Macros
// -----------------------------------------------------------------------------

/* Size of EGB LED IS31FL3741 module */
#if (IS31FL3741_DISPLAY_LAYOUT == 0)
#define WIDTH                                   13
#define HEIGHT                                  9

#elif (IS31FL3741_DISPLAY_LAYOUT == 1)
#define WIDTH                                   26
#define HEIGHT                                  9

#elif (IS31FL3741_DISPLAY_LAYOUT == 2)
#define WIDTH                                   39
#define HEIGHT                                  9

#elif (IS31FL3741_DISPLAY_LAYOUT == 3)
#define WIDTH                                   42
#define HEIGHT                                  9

#elif (IS31FL3741_DISPLAY_LAYOUT == 4)
#define WIDTH                                   13
#define HEIGHT                                  18

#elif (IS31FL3741_DISPLAY_LAYOUT == 5)
#define WIDTH                                   26
#define HEIGHT                                  18

#endif

typedef enum {
  // Offset:     R          G          B
  IS3741_RGB = ((0 << 4) | (1 << 2) | (2)), // Encode as R,G,B
  IS3741_RBG = ((0 << 4) | (2 << 2) | (1)), // Encode as R,B,G
  IS3741_GRB = ((1 << 4) | (0 << 2) | (2)), // Encode as G,R,B
  IS3741_GBR = ((2 << 4) | (0 << 2) | (1)), // Encode as G,B,R
  IS3741_BRG = ((1 << 4) | (2 << 2) | (0)), // Encode as B,R,G
  IS3741_BGR = ((2 << 4) | (1 << 2) | (0)), // Encode as B,G,R
} IS3741_order;

#endif /* ADAFRUIT_IS31FL3741_PARAMS_H_ */
