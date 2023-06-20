/***************************************************************************//**
 * @file mikroe_e_paper.c
 * @brief SCL Mikroe E-Paper Header File
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
#ifndef MIKROE_E_PAPER_154_INCH_H_
#define MIKROE_E_PAPER_154_INCH_H_

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include "eink_154inch.h"
#include "spidrv.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
//                               Macros
// -----------------------------------------------------------------------------

/* E-Paper Screen color */
#define E_PAPER_SCREEN_COLOR_WHITE                                     0xFF
#define E_PAPER_SCREEN_COLOR_BLACK                                     0x00

/* E-Paper Display Command */
#define E_PAPER_CMD_DRIVER_OUTPUT_CONTROL                              0x01
#define E_PAPER_CMD_BOOSTER_SOFT_START_CONTROL                         0x0C
#define E_PAPER_CMD_GATE_SCAN_START_POSITION                           0x0F
#define E_PAPER_CMD_DEEP_SLEEP_MODE                                    0x10
#define E_PAPER_CMD_DATA_ENTRY_MODE_SETTING                            0x11
#define E_PAPER_CMD_SW_RESET                                           0x12
#define E_PAPER_CMD_TEMPERATURE_SENSOR_CONTROL                         0x1A
#define E_PAPER_CMD_MASTER_ACTIVATION                                  0x20
#define E_PAPER_CMD_DISPLAY_UPDATE_CONTROL_1                           0x21
#define E_PAPER_CMD_DISPLAY_UPDATE_CONTROL_2                           0x22
#define E_PAPER_CMD_WRITE_RAM                                          0x24
#define E_PAPER_CMD_WRITE_VCOM_REGISTER                                0x2C
#define E_PAPER_CMD_WRITE_LUT_REGISTER                                 0x32
#define E_PAPER_CMD_SET_DUMMY_LINE_PERIOD                              0x3A
#define E_PAPER_CMD_SET_GATE_TIME                                      0x3B
#define E_PAPER_CMD_BORDER_WAVEFORM_CONTROL                            0x3C
#define E_PAPER_CMD_SET_RAM_X_ADDRESS_START_END_POSITION               0x44
#define E_PAPER_CMD_SET_RAM_Y_ADDRESS_START_END_POSITION               0x45
#define E_PAPER_CMD_SET_RAM_X_ADDRESS_COUNTER                          0x4E
#define E_PAPER_CMD_SET_RAM_Y_ADDRESS_COUNTER                          0x4F
#define E_PAPER_CMD_TERMINATE_FRAME_READ_WRITE                         0xFF

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_init(SPIDRV_Handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    Generic transfer function.
 *
 * @param[in] wr_buf
 *    Write data buffer
 * @param[in] wr_len
 *    Number of byte in write data buffer
 * @param[in] rd_buf
 *    Read data buffer
 * @param[in] rd_len
 *    Number of byte in read data buffer
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_generic_transfer(uint8_t *wr_buf,
                                                     uint16_t wr_len,
                                                     uint8_t *rd_buf,
                                                     uint16_t rd_len);

/***************************************************************************//**
 * @brief
 *    Sending command function.
 *
 * @param[in] command
 *    The command to be sent
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_send_command(uint8_t command);

/***************************************************************************//**
 * @brief
 *    Sending data function.
 *
 * @param[in] data
 *    The data to be sent
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_send_data(uint8_t data);

/***************************************************************************//**
 * @brief
 *    Reset chip function.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_reset(void);

/***************************************************************************//**
 * @brief
 *    Sleep mode function.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_sleep_mode(void);

/***************************************************************************//**
 * @brief
 *    Configuration display function.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_start_config(void);

/***************************************************************************//**
 * @brief
 *    Set LUT table function.
 *
 * @param[in] lut
 *    Lut table
 * @param[in] n_bytes
 *    Number of bytes in Lut table
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_set_lut(const uint8_t *lut,
                                            uint8_t n_bytes);

/***************************************************************************//**
 * @brief
 *    Setting pointers in memory.
 *
 * @param[in] x
 *    x position
 * @param[in] y
 *    y position
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_set_memory_pointer(uint8_t x, uint8_t y);

/***************************************************************************//**
 * @brief
 *    Setting area in memory.
 *
 * @param[in] xy
 *    Struct object
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_set_memory_area(eink_154_xy_t *xy);

/***************************************************************************//**
 * @brief
 *    Update display function.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_update_display();

/***************************************************************************//**
 * @brief
 *    Function that fills the screen.
 *
 * @param[in] lut
 *    Lut table
 * @param[in] color
 *    The color to which the screen will be colored.
 *    Options :
 *      E_PAPER_SCREEN_COLOR_WHITE
 *      E_PAPER_SCREEN_COLOR_BLACK
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_fill_screen(uint8_t color);

/***************************************************************************//**
 * @brief
 *    Displays image function.
 *
 * @param[in] image_buffer
 *    Buffer containing the image.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_display_image(const uint8_t *image_buffer);

/***************************************************************************//**
 * @brief
 *    Displays image function.
 *
 * @param[in] text
 *    Text buffer.
 * @param[in] text_set
 *    Struct object.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_text(char *text,
                                         eink_154_text_set_t *text_set);

/***************************************************************************//**
 * @brief
 *    Displays image function.
 *
 * @param[in] cfg_font
 *    Struct object.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_set_font(eink_154inch_font_t *cfg_font);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_E_PAPER_H_ */
