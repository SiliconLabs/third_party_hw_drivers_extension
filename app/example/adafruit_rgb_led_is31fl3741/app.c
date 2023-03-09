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

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include <string.h>
#include "app_log.h"
#include "app_assert.h"
#include "sl_i2cspm_instances.h"
#include "adafruit_is31fl3741.h"
#include "sl_sleeptimer.h"
#include "glib.h"
#include "glib_font.h"

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

char *str = "Silicon Labs - Third Party Hardware Drivers Extension";
static glib_context_t glib_context;
static uint16_t color[] = { 0xf800, 0xffe0, 0x07e0, 0x07ff, 0x001f, 0xf81f,
                            0xffff, 0x73c0 };
static sl_sleeptimer_timer_handle_t my_timer;

// -----------------------------------------------------------------------------
//                       Local Functions
// -----------------------------------------------------------------------------

static void timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  char *ptr = str;
  static int16_t cursor_x = 0;
  int length = (int)strlen(str) * 6 - 1;
  int i = 0;

  (void)data;
  (void)handle;

  glib_set_cursor(&glib_context, cursor_x, 1);
  glib_fill(&glib_context, 0);
  while (*ptr) {
    glib_set_text_color(&glib_context, color[i++]);
    glib_write_char(&glib_context, *ptr++);
    if (i == 8) {
      i = 0;
    }
  }
  glib_update_display();
  if (cursor_x > -length) {
    cursor_x--;
  } else {
    cursor_x = glib_context.width;
  }
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t ret;

  ret = adafruit_is31fl3741_init(sl_i2cspm_qwiic);
  app_assert(ret == SL_STATUS_OK, "\rAdafruit RGB LED initialization fail\n");
  app_log("\rAdafruit RGB LED initialization done\n");

  // GLIB initialization
  glib_init(&glib_context);
  glib_set_rotation(&glib_context, 0);
  glib_set_contrast(0xFF);
  glib_set_bg_color(&glib_context, 0x0000);
  glib_enable_wrap(&glib_context, false);
  glib_enable_display(true);
  app_log("\rGLIB Initialization done\n");

  ret = sl_sleeptimer_start_periodic_timer_ms(&my_timer,
                                              50,
                                              timer_callback,
                                              NULL,
                                              0,
                                              0);
  app_assert(ret == SL_STATUS_OK, "\rStart displaying text fail\n");
  app_log("\rStart displaying text\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
