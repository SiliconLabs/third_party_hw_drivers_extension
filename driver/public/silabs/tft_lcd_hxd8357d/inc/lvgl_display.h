/***************************************************************************//**
 * @file lvgl_display.h
 * @brief LVGL Driver API interface
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

#ifndef LVGL_DISPLAY_H_
#define LVGL_DISPLAY_H_

#include "adafruit_hxd8357d.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LVGL_TFT_WIDTH  HXD8357D_TFTWIDTH
#define LVGL_TFT_HEIGHT HXD8357D_TFTHEIGHT

typedef struct lvgl_display_driver_api {
  sl_status_t (*init)(void);
  sl_status_t (*flush_area_rgb565)(int16_t x1, int16_t y1,
                                   int16_t x2, int16_t y2,
                                   uint8_t *pcolor,
                                   bool color_swap,
                                   void (*callback)(void *arg),
                                   void *callback_arg);
} lvgl_display_driver_api_t;

/**
 * General memory LCD data structure.
 */
typedef struct lvgl_display {
  unsigned short width;       ///< Display pixel width
  unsigned short height;      ///< Display pixel height
  const lvgl_display_driver_api_t *driver;
} lvgl_display_t;

sl_status_t lvgl_display_init(void);
lvgl_display_t *lvgl_display_get(void);

#ifdef __cplusplus
}
#endif

#endif /* LVGL_DISPLAY_H_ */
