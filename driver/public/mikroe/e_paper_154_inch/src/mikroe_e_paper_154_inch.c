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

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include "mikroe_e_paper_154_inch.h"
#include "third_party_hw_drivers_helpers.h"
#include "mikroe_e_paper_154_inch_config.h"

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

static eink_154inch_t eink_ctx;
static eink_154inch_cfg_t eink_cfg;
static bool initialized = false;

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/**************************************************************************//**
*  E-Paper initialization.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_init(SPIDRV_Handle_t spi_instance)
{
  THIRD_PARTY_HW_DRV_RETCODE_INIT();

  if (spi_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (initialized) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  eink_ctx.spi.handle = spi_instance;
  eink_154inch_cfg_setup(&eink_cfg);

  eink_cfg.dc = hal_gpio_pin_name(E_PAPER_DC_PORT, E_PAPER_DC_PIN);
  eink_cfg.bsy = hal_gpio_pin_name(E_PAPER_BSY_PORT, E_PAPER_BSY_PIN);
  eink_cfg.rst = hal_gpio_pin_name(E_PAPER_RST_PORT, E_PAPER_RST_PIN);

  THIRD_PARTY_HW_DRV_RETCODE_TEST(eink_154inch_init(&eink_ctx, &eink_cfg));
  initialized = true;

  return THIRD_PARTY_HW_DRV_RETCODE_VALUE;
}

/**************************************************************************//**
*  E-Paper generic transfer.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_generic_transfer(uint8_t *wr_buf,
                                                     uint16_t wr_len,
                                                     uint8_t *rd_buf,
                                                     uint16_t rd_len)
{
  eink_154inch_generic_transfer(&eink_ctx, wr_buf, wr_len, rd_buf, rd_len);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper send command.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_send_command(uint8_t command)
{
  eink154_send_command(&eink_ctx, command);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper send data.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_send_data(uint8_t data)
{
  eink154_send_data(&eink_ctx, data);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper reset.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_reset(void)
{
  eink154_reset(&eink_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper sleep mode.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_sleep_mode(void)
{
  eink154_sleep_mode(&eink_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper start config.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_start_config(void)
{
  eink154_start_config(&eink_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper set lut.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_set_lut(const uint8_t *lut, uint8_t n_bytes)
{
  eink154_set_lut(&eink_ctx, lut, n_bytes);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper set memory pointer.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_set_memory_pointer(uint8_t x, uint8_t y)
{
  eink154_set_memory_pointer(&eink_ctx, x, y);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper set memory area.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_set_memory_area(eink_154_xy_t *xy)
{
  eink154_set_memory_area(&eink_ctx, xy);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper update display.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_update_display(void)
{
  eink154_update_display(&eink_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper fill screen.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_fill_screen(uint8_t color)
{
  eink154_fill_screen(&eink_ctx, color);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper display image.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_display_image(const uint8_t *image_buffer)
{
  eink154_display_image(&eink_ctx, image_buffer);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper display image for ESL.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_display_image_non_blocking(
  const uint8_t image_index)
{
  return eink154_display_image_non_blocking(&eink_ctx, image_index);
}

/**************************************************************************//**
*  E-Paper text.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_text(char *text,
                                         eink_154_text_set_t *text_set)
{
  eink154_text(&eink_ctx, text, text_set);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper set font.
******************************************************************************/
sl_status_t mikroe_e_paper_154_inch_set_font(eink_154inch_font_t *cfg_font)
{
  eink154_set_font(&eink_ctx, cfg_font);
  return SL_STATUS_OK;
}
