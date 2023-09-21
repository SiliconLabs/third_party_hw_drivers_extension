/***************************************************************************//**
 * @file touch_screen.h
 * @brief Touchscreen Header File
 * @version 1.0.0
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#ifndef TOUCH_SCREEN_H_
#define TOUCH_SCREEN_H_

// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include "sl_status.h"
#include "em_cmu.h"
#include "sl_udelay.h"
#include "em_gpio.h"
#include "em_iadc.h"
#include "touch_screen_config.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
//                       Typedefs
// -----------------------------------------------------------------------------
typedef struct
{
  GPIO_Port_TypeDef port;
  unsigned int pin;
}touch_adc_pin_t;

// Touch point properties struct
typedef struct
{
  int x; /* X coordinate of touch point */
  int y; /* Y coordinate of touch point */
  int z; /* Pressure value when touch point detected */
} touch_point_t;

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  Initialize Touch Screen feature.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t touch_screen_init(void);

/***************************************************************************//**
 * @brief
 *  Get touch points whenever a touch is detected on the screen.
 *
 * @param[in] rxplate
 *  Resistance threshold value.
 * @param[out] ts_point
 *  Touch point detected coordinate.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t touch_screen_get_point(int rxplate, touch_point_t *ts_point);

#ifdef __cplusplus
extern "C"
}
#endif

#endif /* TOUCH_SCREEN_H_ */
