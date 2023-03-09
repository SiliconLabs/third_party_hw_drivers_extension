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

#include "sl_simple_button_instances.h"
#include "sl_i2cspm_instances.h"
#include "glib.h"
#include "glib_font.h"
#include "micro_oled_ssd1306.h"
#include "sl_udelay.h"

static void oled_app_init(void);

static glib_context_t glib_context;
#define DISPLAY_MODE_COUNT  (12)
static uint8_t display_mode = 0;

static const unsigned char silicon_labs_logo_64x23[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x80, 0xff, 0x1f, 0x00, 0x1e, 0x00, 0x00, 0x00, 0xf0,
  0xff, 0x7f, 0x00, 0x3e, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x7f, 0x00, 0x7f,
  0x00, 0x00, 0x00, 0xff, 0xff, 0x3f, 0x80, 0x7f, 0x00, 0x06, 0x80, 0xff,
  0xff, 0xff, 0xe0, 0xff, 0x80, 0x03, 0xc0, 0xff, 0x01, 0xf8, 0xff, 0x7f,
  0xe0, 0x01, 0xc0, 0x03, 0x00, 0x00, 0xfe, 0x7f, 0xf0, 0x01, 0x00, 0x01,
  0x00, 0x00, 0xf8, 0x3f, 0xf8, 0x03, 0x00, 0xf0, 0x07, 0x00, 0xe0, 0x3f,
  0xfc, 0x03, 0x00, 0x00, 0x20, 0x00, 0xe0, 0x1f, 0xfe, 0x0f, 0x00, 0x00,
  0xc0, 0x00, 0xe0, 0x07, 0xfe, 0xff, 0x00, 0x00, 0xfc, 0x00, 0xe0, 0x03,
  0xff, 0xc3, 0xff, 0xff, 0xff, 0x00, 0xf0, 0x00, 0x7e, 0x00, 0xfe, 0xff,
  0x7f, 0x00, 0x38, 0x00, 0x3e, 0x00, 0xff, 0xff, 0x3f, 0x00, 0x0c, 0x00,
  0x1c, 0x80, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x18, 0x00, 0xff, 0xff,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  /* Initialize the oled. */
  oled_app_init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t i;
  switch (display_mode) {
    case 0:
      glib_clear(&glib_context);
      glib_draw_string(&glib_context, "SILABS", 0, 2);

      glib_draw_string(&glib_context, "OLED", 0, 12);

      glib_draw_string(&glib_context, "Sparkfun", 0, 22);

      glib_draw_string(&glib_context, "PressBTN0", 0, 32);

      glib_update_display();
      break;

    case 1:
      glib_clear(&glib_context);
      glib_draw_rect(&glib_context,
                     5, 5, 40, 20,
                     GLIB_WHITE);
      glib_draw_string(&glib_context, "rect", 15, 35);
      glib_update_display();
      break;

    case 2:
      glib_clear(&glib_context);
      glib_fill_rect(&glib_context,
                     5, 5, 55, 25,
                     GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 6, 7);
      glib_draw_string(&glib_context, "rectangle", 6, 18);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 3:
      glib_clear(&glib_context);
      glib_draw_round_rect(&glib_context,
                           2, 5, 60, 40,
                           10,
                           GLIB_WHITE);
      glib_draw_string(&glib_context, "round", 6, 14);
      glib_draw_string(&glib_context, "rectangle", 6, 25);
      glib_update_display();
      break;

    case 4:
      glib_clear(&glib_context);
      glib_fill_round_rect(&glib_context,
                           2, 5, 60, 40,
                           10,
                           GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 6, 7);
      glib_draw_string(&glib_context, "round", 6, 18);
      glib_draw_string(&glib_context, "rectangle", 6, 29);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 5:
      glib_clear(&glib_context);
      glib_draw_triangle(&glib_context,
                         2, 2,
                         40, 3,
                         5, 40,
                         GLIB_WHITE);
      glib_draw_string(&glib_context, "triangle", 15, 31);
      glib_update_display();
      break;

    case 6:
      glib_clear(&glib_context);
      glib_fill_triangle(&glib_context,
                         2, 2,
                         40, 3,
                         5, 40,
                         GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 40, 20);
      glib_draw_string(&glib_context, "triangle", 15, 31);
      glib_update_display();
      break;

    case 7:
      glib_clear(&glib_context);
      glib_draw_circle(&glib_context, 32, 24, 20,
                       GLIB_WHITE);
      glib_draw_string(&glib_context, "circle", 15, 18);
      glib_update_display();
      break;

    case 8:
      glib_clear(&glib_context);
      glib_fill_circle(&glib_context, 32, 24, 20,
                       GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 16, 12);
      glib_draw_string(&glib_context, "circle", 16, 23);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 9:
      glib_clear(&glib_context);
      glib_fill_circle_helper(&glib_context, 40, 20, 15,
                              2, 10, GLIB_WHITE);
      glib_update_display();
      break;

    case 10:
      glib_clear(&glib_context);
      glib_set_font(&glib_context, &glib_font_free_sans_9pt7b);
      glib_draw_string(&glib_context, "Custom", 1, 14);
      glib_draw_string(&glib_context, "font", 1, 30);
      glib_set_font(&glib_context, NULL);
      glib_update_display();
      break;

    case 11:
      glib_clear(&glib_context);
      glib_fill(&glib_context, GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "FILL", 16, 23);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 12:
      glib_clear(&glib_context);
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_64x23,
                        64, 23, GLIB_WHITE);
      glib_draw_string(&glib_context, "XBITMAP", 10, 30);
      glib_update_display();

      sl_udelay_wait(500000);
      glib_set_invert_color();
      sl_udelay_wait(500000);
      glib_set_normal_color();
      sl_udelay_wait(500000);
      glib_set_invert_color();
      sl_udelay_wait(500000);
      glib_set_normal_color();

      for (i = 0x8F; i > 0x00; i--) {
        glib_set_contrast(i);
        sl_udelay_wait(5000);
      }

      for (i = 0x00; i < 0x8F; i++) {
        glib_set_contrast(i);
        sl_udelay_wait(5000);
      }

      glib_scroll_right(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_64x23,
                        64, 23, GLIB_WHITE);
      glib_draw_string(&glib_context, "XBITMAP", 10, 30);
      glib_update_display();

      glib_scroll_left(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_64x23,
                        64, 23, GLIB_WHITE);
      glib_draw_string(&glib_context, "XBITMAP", 10, 30);
      glib_update_display();

      glib_scroll_diag_right(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_64x23,
                        64, 23, GLIB_WHITE);
      glib_draw_string(&glib_context, "XBITMAP", 10, 30);
      glib_update_display();

      glib_scroll_diag_left(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
      break;
  }
}

/***************************************************************************//**
 * Callback on button change.
 *
 * This function overrides a weak implementation defined in the simple_button
 * module. It is triggered when the user activates one of the buttons.
 *
 ******************************************************************************/
void sl_button_on_change(const sl_button_t *handle)
{
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
    if (&sl_button_btn0 == handle) {
      if (display_mode < DISPLAY_MODE_COUNT) {
        display_mode++;
      } else {
        display_mode = 0;
      }
    }
  }
}

/***************************************************************************//**
 * Initialize example.
 ******************************************************************************/
static void oled_app_init(void)
{
  /* Initialize the display */
  ssd1306_init(sl_i2cspm_qwiic);
  glib_init(&glib_context);

  /* Fill lcd with background color */
  glib_clear(&glib_context);
}
