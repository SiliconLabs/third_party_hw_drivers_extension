/***************************************************************************//**
 * @file adafruit_st7789_spi_usart_config.h
 * @brief Configuration file for Adafruit RGB LED Driver board.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#ifndef ADAFRUIT_ST7789_CONFIG_H_
#define ADAFRUIT_ST7789_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

// <<< Use Configuration Wizard in Context Menu >>>
// <h> ST7789 SPI settings

// <o ADAFRUIT_ST7789_SPI_BITRATE> ST7789 SPI bitrate
// <i> Default: 40000000
#define ADAFRUIT_ST7789_SPI_BITRATE                      40000000
// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <usart signal=TX,RX,CLK,(CS)> ADAFRUIT_ST7789
// $[USART_ADAFRUIT_ST7789]
#warning "ST7789 SPI Peripheral not configured"
// [USART_ADAFRUIT_ST7789]$

// <gpio optional=true> ST7789_SPI_DC
// $[GPIO_ST7789_SPI_DC]
#warning "ST7789 SPI DC Pin not configured"
// [GPIO_ST7789_SPI_DC]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
extern "C"
}
#endif

#endif /* ADAFRUIT_ST7789_CONFIG_H_ */
