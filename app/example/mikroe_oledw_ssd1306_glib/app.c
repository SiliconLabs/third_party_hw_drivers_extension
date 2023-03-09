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
#include "sl_spidrv_instances.h"
#include "glib.h"
#include "glib_font.h"
#include "mikroe_ssd1306.h"

static void oled_app_init(void);

static glib_context_t glib_context;
#define DISPLAY_MODE_COUNT  (15)
static uint8_t display_mode = 0;

static const unsigned char silicon_labs_logo_96x35[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0xe0, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x0f, 0x00, 0xc0, 0x07,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x3f, 0x00, 0xc0, 0x1f,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xe0, 0x3f,
  0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xf0, 0x7f,
  0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x3f, 0x00, 0xf8, 0x7f,
  0x00, 0x80, 0x01, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x1f, 0x00, 0xfe, 0x7f,
  0x00, 0xe0, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
  0x00, 0x78, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff,
  0x00, 0x3c, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x7f,
  0x00, 0x3f, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x7f,
  0x80, 0x3f, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x3f,
  0xe0, 0x3f, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x3f,
  0xf0, 0x3f, 0x00, 0x00, 0x80, 0xff, 0x1f, 0x00, 0x00, 0x80, 0xff, 0x1f,
  0xf8, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x01, 0x00, 0x80, 0xff, 0x0f,
  0xfc, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0xff, 0x07,
  0xfc, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0xff, 0x01,
  0xfe, 0xff, 0x3f, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0xff, 0x00,
  0xfe, 0xff, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x0f, 0x00, 0x80, 0x3f, 0x00,
  0xfe, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xc0, 0x0f, 0x00,
  0xfe, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x07, 0x00, 0xe0, 0x03, 0x00,
  0xfe, 0x03, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x03, 0x00, 0xf0, 0x00, 0x00,
  0xfe, 0x01, 0x80, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x38, 0x00, 0x00,
  0xfc, 0x00, 0x80, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x0e, 0x00, 0x00,
  0xfc, 0x00, 0x80, 0xff, 0xff, 0xff, 0x1f, 0x00, 0x80, 0x01, 0x00, 0x00,
  0xf0, 0x00, 0x80, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xe0, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x00, 0x00, 0xe0, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
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
      glib_draw_string(&glib_context, "Mikroe", 0, 12);
      glib_draw_string(&glib_context, "OLED", 0, 22);
      glib_draw_string(&glib_context, "PressBTN0", 0, 32);

      glib_update_display();
      break;

    case 1:
      glib_clear(&glib_context);
      glib_draw_rect(&glib_context,
                     5, 5, 40, 20,
                     GLIB_WHITE);
      glib_draw_string(&glib_context, "rect", 15, 30);
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
                           0, 5, 64, 32,
                           10,
                           GLIB_WHITE);
      glib_draw_string(&glib_context, "round", 6, 12);
      glib_draw_string(&glib_context, "rectangle", 6, 23);
      glib_update_display();
      break;

    case 4:
      glib_clear(&glib_context);
      glib_fill_round_rect(&glib_context,
                           0, 5, 64, 32,
                           10,
                           GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 6, 7);
      glib_draw_string(&glib_context, "round", 6, 16);
      glib_draw_string(&glib_context, "rectangle", 6, 27);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 5:
      glib_clear(&glib_context);
      glib_draw_triangle(&glib_context,
                         2, 2,
                         40, 3,
                         5, 38,
                         GLIB_WHITE);
      glib_draw_string(&glib_context, "triangle", 15, 31);
      glib_update_display();
      break;

    case 6:
      glib_clear(&glib_context);
      glib_fill_triangle(&glib_context,
                         2, 2,
                         40, 3,
                         5, 38,
                         GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 40, 20);
      glib_draw_string(&glib_context, "triangle", 15, 31);
      glib_update_display();
      break;

    case 7:
      glib_clear(&glib_context);
      glib_draw_circle(&glib_context, 32, 19, 19,
                       GLIB_WHITE);
      glib_draw_string(&glib_context, "circle", 15, 16);
      glib_update_display();
      break;

    case 8:
      glib_clear(&glib_context);
      glib_fill_circle(&glib_context, 32, 19, 19,
                       GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 16, 10);
      glib_draw_string(&glib_context, "circle", 16, 19);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 9:
      glib_clear(&glib_context);
      glib_fill_circle_helper(&glib_context, 40, 17, 15,
                              2, 6, GLIB_WHITE);
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
      glib_set_rotation(&glib_context, 1);
      glib_draw_string(&glib_context, "Rotation", 1, 14);
      glib_draw_string(&glib_context, "90*", 1, 31);
      glib_set_font(&glib_context, NULL);
      glib_update_display();
      break;

    case 12:
      glib_clear(&glib_context);
      glib_set_rotation(&glib_context, 2);
      glib_draw_string(&glib_context, "Rotation", 2, 14);
      glib_draw_string(&glib_context, "180*", 2, 24);
      glib_set_font(&glib_context, NULL);
      glib_update_display();
      glib_set_rotation(&glib_context, 0);
      break;

    case 13:
      glib_clear(&glib_context);
      glib_set_rotation(&glib_context, 3);
      glib_draw_string(&glib_context, "Rotation", 2, 14);
      glib_draw_string(&glib_context, "270*", 2, 31);
      glib_set_font(&glib_context, NULL);
      glib_update_display();
      glib_set_rotation(&glib_context, 0);
      break;

    case 14:
      glib_clear(&glib_context);
      glib_fill(&glib_context, GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "FILL", 16, 23);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 15:
      glib_clear(&glib_context);
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_96x35,
                        96, 35, GLIB_WHITE);
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
                        0, 0, silicon_labs_logo_96x35,
                        96, 35, GLIB_WHITE);
      glib_update_display();

      glib_scroll_left(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_96x35,
                        96, 35, GLIB_WHITE);
      glib_update_display();

      glib_scroll_diag_right(0x00, 0x06);
      sl_udelay_wait(1000000);
      glib_stop_scroll();
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_96x35,
                        96, 35, GLIB_WHITE);
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
  mikroe_ssd1306_init(sl_spidrv_mikroe_handle);
  glib_init(&glib_context);

  /* Fill lcd with background color */
  glib_clear(&glib_context);
}
