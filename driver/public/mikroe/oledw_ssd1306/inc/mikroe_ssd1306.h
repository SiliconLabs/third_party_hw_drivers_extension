/***************************************************************************//**
 * @file mikroe_ssd1306.h
 * @brief SCL SSD1306 Prototypes
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

#ifndef MIKROE_SSD1306_H_
#define MIKROE_SSD1306_H_

#include "sl_status.h"
#include "spidrv.h"
#include "oledw.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup ssd1306 SSD1306 - OLED W Click
 * @brief  Driver for the Mikroe OLED W Click
 *
 *    @n @section ssd1306_example SSD1306 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "sl_spidrv_instances.h"
 * #include "mikroe_ssd1306.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize sensor and set mode
 *      mikroe_ssd1306_init(sl_spidrv_mikroe_handle);
 *      mikroe_ssd1306_default_cfg();
 *
 *      while (true) {
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
* @addtogroup ssd1306_registers Register Addresses
* @brief Register definitions
* @{
*******************************************************************************/
#define MIKROE_SSD1306_LCDWIDTH                              96
#define MIKROE_SSD1306_LCDHEIGHT                             39
#define MIKROE_SSD1306_DISPLAYOFF                            0xAE
#define MIKROE_SSD1306_SETDISPLAYCLOCKDIV                    0xD5
#define MIKROE_SSD1306_SETMULTIPLEX                          0xA8
#define MIKROE_SSD1306_SETDISPLAYOFFSET                      0xD3
#define MIKROE_SSD1306_SETSTARTLINE                          0x40
#define MIKROE_SSD1306_CHARGEPUMP                            0x8D
#define MIKROE_SSD1306_SETSEGMENTREMAP                       0xA1
#define MIKROE_SSD1306_SEGREMAP                              0xA0
#define MIKROE_SSD1306_COMSCANDEC                            0xC8
#define MIKROE_SSD1306_SETCOMPINS                            0xDA
#define MIKROE_SSD1306_SETCONTRAST                           0x81
#define MIKROE_SSD1306_SETPRECHARGE                          0xD9
#define MIKROE_SSD1306_SETVCOMDETECT                         0xDB
#define MIKROE_SSD1306_DISPLAYALLON_RESUME                   0xA4
#define MIKROE_SSD1306_NORMALDISPLAY                         0xA6
#define MIKROE_SSD1306_DISPLAYON                             0xAF
#define MIKROE_SSD1306_DISPLAYALLON                          0xA5
#define MIKROE_SSD1306_INVERTDISPLAY                         0xA7
#define MIKROE_SSD1306_SETLOWCOLUMN                          0x00
#define MIKROE_SSD1306_SETHIGHCOLUMN                         0x10
#define MIKROE_SSD1306_MEMORYMODE                            0x20
#define MIKROE_SSD1306_COLUMNADDR                            0x21
#define MIKROE_SSD1306_PAGEADDR                              0x22
#define MIKROE_SSD1306_COMSCANINC                            0xC0
#define MIKROE_SSD1306_EXTERNALVCC                           0x1
#define MIKROE_SSD1306_SWITCHCAPVCC                          0x2
#define MIKROE_SSD1306_ACTIVATE_SCROLL                       0x2F
#define MIKROE_SSD1306_DEACTIVATE_SCROLL                     0x2E
#define MIKROE_SSD1306_SET_VERTICAL_SCROLL_AREA              0xA3
#define MIKROE_SSD1306_RIGHT_HORIZONTAL_SCROLL               0x26
#define MIKROE_SSD1306_LEFT_HORIZONTAL_SCROLL                0x27
#define MIKROE_SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL  0x29
#define MIKROE_SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL   0x2A

/** @} (end addtogroup ssd1306_registers) */

/***************************************************************************//**
 * @addtogroup ssd1306_data_mode Data mode value definitions
 * @brief Data mode value definitions
 * @{
 ******************************************************************************/
#define MIKROE_SSD1306_COMMAND                               0x3C
#define MIKROE_SSD1306_DATA                                  0x3D

/** @} (end addtogroup ssd1306_data_mode) */

/***************************************************************************//**
 * @brief
 *    Click Initialization function.
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_init(SPIDRV_Handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the SPI instance used by platform functions
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors
 *    SL_STATUS_INVALID_PARAMETER if spi_instance is null
 ******************************************************************************/
sl_status_t mikroe_ssd1306_set_spi_instance(SPIDRV_Handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    Send function.
 *
 * @param[in] tx_data Data to be sent.
 * @param[in] data_mode 0x3C - Command, 0x3D - Data.
 *
 * @returns SL_STATUS_OK - OK
 *          SL_STATUS_FAIL - Invalid data mode.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_send(oledw_data_t tx_data,
                                oledw_data_mode_t data_mode);

/***************************************************************************//**
 * @brief
 *    Click Default Configuration function.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_default_cfg(void);

/***************************************************************************//**
 * @brief
 *    Page Setting function.
 *
 * @param[in] page_addr Page address.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_set_page(oledw_data_t page_addr);

/***************************************************************************//**
 * @brief
 *    Column Setting function.
 *
 * @param[in] col_addr Column address.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_set_column(oledw_data_t col_addr);

/***************************************************************************//**
 * @brief
 *    Display Picture function.
 *
 * @param[in] pic  Resource code.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_display_picture(oledw_resources_t *pic);

/**************************************************************************//**
 * @brief
 *   Draw total of rows to SSD1306.
 *
 * @param[in] data
 *   Pointer to the pixel matrix buffer to draw. The format of the buffer
 *   depends on the color mode of SSD1306.
 *
 * @return
 *   SL_STATUS_OK if there are no errors.
 *****************************************************************************/
sl_status_t mikroe_ssd1306_draw(const uint8_t *data);

/***************************************************************************//**
 * @brief
 *   Set the display ON/OFF.
 *
 * @param[in] on
 *   State of display
 *
 * @return
 *   SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_enable_display(bool on);

/***************************************************************************//**
 * @brief
 *   Set a inversion color to SSD1306.
 *
 * @return
 *   SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_set_invert_color(void);

/***************************************************************************//**
 * @brief
 *   Set a normal color to SSD1306.
 *
 * @return
 *   SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_set_normal_color(void);

/***************************************************************************//**
 * @brief
 *    Contrast Setting function.
 *
 * @param[in] value  Contrast value to be set.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_set_contrast(oledw_data_t value);

/***************************************************************************//**
 * @brief
 *    Scroll Right function.
 *
 * @param[in] start_page_addr  Start page address.
 * @param[in] end_page_addr  End page address.
 *
 ******************************************************************************/
sl_status_t mikroe_ssd1306_scroll_right(oledw_data_t start_page_addr,
                                        oledw_data_t end_page_addr);

/***************************************************************************//**
 * @brief
 *    Scroll Left function.
 *
 * @param[in] start_page_addr  Start page address.
 * @param[in] end_page_addr  End page address.
 *
 ******************************************************************************/
sl_status_t mikroe_ssd1306_scroll_left(oledw_data_t start_page_addr,
                                       oledw_data_t end_page_addr);

/***************************************************************************//**
 * @brief
 *    Scroll Diagonally Right function.
 *
 * @param[in] start_page_addr  Start page address.
 * @param[in] end_page_addr  End page address.
 *
 ******************************************************************************/
sl_status_t mikroe_ssd1306_scroll_diag_right(oledw_data_t start_page_addr,
                                             oledw_data_t end_page_addr);

/***************************************************************************//**
 * @brief
 *    Scroll Diagonally Left function.
 *
 * @param[in] start_page_addr  Start page address.
 * @param[in] end_page_addr  End page address.
 *
 ******************************************************************************/
sl_status_t mikroe_ssd1306_scroll_diag_left(oledw_data_t start_page_addr,
                                            oledw_data_t end_page_addr);

/***************************************************************************//**
 * @brief
 *    Stop Scrolling function.
 ******************************************************************************/
sl_status_t mikroe_ssd1306_stop_scroll(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup ssd1306) */

#endif /* MIKROE_SSD1306_H_ */
