/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "sl_sleeptimer.h"
#include "sl_spidrv_instances.h"

#include "app_assert.h"

#include "mikroe_ssd1306.h"
#include "mikroe_resources.h"

void app_init(void)
{
  sl_status_t ret_code;

  ret_code = mikroe_ssd1306_init(sl_spidrv_mikroe_handle);
  app_assert_status(ret_code);

  mikroe_ssd1306_default_cfg();

  /*Change segment remap for Mikroe Bitmap*/
  mikroe_ssd1306_send(OLEDW_SEGREMAP, MIKROE_SSD1306_COMMAND);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t i;

  mikroe_ssd1306_display_picture(oledw_img);
  sl_sleeptimer_delay_millisecond(500);
  mikroe_ssd1306_send(MIKROE_SSD1306_INVERTDISPLAY, MIKROE_SSD1306_COMMAND);
  sl_sleeptimer_delay_millisecond(500);
  mikroe_ssd1306_send(MIKROE_SSD1306_NORMALDISPLAY, MIKROE_SSD1306_COMMAND);
  sl_sleeptimer_delay_millisecond(500);
  mikroe_ssd1306_send(MIKROE_SSD1306_INVERTDISPLAY, MIKROE_SSD1306_COMMAND);
  sl_sleeptimer_delay_millisecond(500);
  mikroe_ssd1306_send(MIKROE_SSD1306_NORMALDISPLAY, MIKROE_SSD1306_COMMAND);
  sl_sleeptimer_delay_millisecond(300);

  for (i = 0xAF; i > 0x00; i--) {
    mikroe_ssd1306_set_contrast(i);
    sl_sleeptimer_delay_millisecond(5);
  }

  for (i = 0x00; i < 0xAF; i++) {
    mikroe_ssd1306_set_contrast(i);
    sl_sleeptimer_delay_millisecond(5);
  }

  mikroe_ssd1306_scroll_right(0x00, 0x05);
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_ssd1306_stop_scroll();
  mikroe_ssd1306_display_picture(oledw_img);

  mikroe_ssd1306_scroll_left(0x00, 0x05);
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_ssd1306_stop_scroll();
  mikroe_ssd1306_display_picture(oledw_img);

  mikroe_ssd1306_scroll_diag_right(0x00, 0x05);
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_ssd1306_stop_scroll();
  mikroe_ssd1306_display_picture(oledw_img);

  mikroe_ssd1306_scroll_diag_left(0x00, 0x05);
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_ssd1306_stop_scroll();
}
