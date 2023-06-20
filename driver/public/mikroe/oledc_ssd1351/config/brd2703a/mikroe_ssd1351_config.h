/***************************************************************************//**
 * @file micro_oled_ssd1351_config.h
 * @brief Micro OLED SSD1351 Configuration
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
#ifndef MIKROE_SSD1351_CONFIG_H_
#define MIKROE_SSD1351_CONFIG_H_

#define SSD1351_DISPLAY_WIDTH                         96
#define SSD1351_DISPLAY_HEIGHT                        96
#define SSD1351_DISPLAY_COLOR                         16
// <<< sl:start pin_tool >>>

// <gpio optional=true> SSD1351_DC
// $[GPIO_SSD1351_DC]
#define SSD1351_DC_PORT                               gpioPortA
#define SSD1351_DC_PIN                                0
// [GPIO_SSD1351_DC]$

// <gpio optional=true> SSD1351_RST
// $[GPIO_SSD1351_RST]
#define SSD1351_RST_PORT                              gpioPortC
#define SSD1351_RST_PIN                               8
// [GPIO_SSD1351_RST]$

// <gpio optional=true> SSD1351_EN
// $[GPIO_SSD1351_EN]
#define SSD1351_EN_PORT                               gpioPortB
#define SSD1351_EN_PIN                                1
// [GPIO_SSD1351_EN]$

// <gpio optional=true> SSD1351_RW
// $[GPIO_SSD1351_RW]
#define SSD1351_RW_PORT                               gpioPortB
#define SSD1351_RW_PIN                                0
// [GPIO_SSD1351_RW]$
// <<< sl:end pin_tool >>>

#endif /* MIKROE_SSD1351_CONFIG_H_ */
