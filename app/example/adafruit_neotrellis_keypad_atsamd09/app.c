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
 *    //                       Include
 ******************************************************************************/

#include <string.h>
#include "app_log.h"
#include "app_assert.h"
#include "sl_i2cspm_instances.h"
#include "adafruit_neotrellis.h"
#include "adafruit_neotrellis_config.h"
#include "sl_sleeptimer.h"

/***************************************************************************//**
 *    //                       Definition
 ******************************************************************************/

#define NUMBER_NEOTRELLIS_BOARD (NEOTRELLIS_NUM_COLUMN_BOARDS \
                                 * NEOTRELLIS_NUM_ROW_BOARDS)

/***************************************************************************//**
 *    //                       Global Variables
 ******************************************************************************/

static uint8_t keypad_addr_list[NUMBER_NEOTRELLIS_BOARD] = { 0x2E };
static uint32_t color_list[8] = { 0xff0000, 0xff00ff, 0xffff00, 0x00ff00,
                                  0x0000ff, 0x00ffff, 0xffffff, 0xc0c0c0 };

/***************************************************************************//**
 *    //                       Static Functions
 ******************************************************************************/

static TrellisCallback blink(keyEvent evt);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  app_log("\rAdafruit NeoTrellis 4x4 Keypad Initializing....\n");

  sc = adafruit_neotrellis_init(sl_i2cspm_qwiic,
                                keypad_addr_list,
                                NUMBER_NEOTRELLIS_BOARD);
  app_assert(sc == SL_STATUS_OK, "Keypad Initilization fail\n");

  app_log("number keypad: %d\n", NUMBER_NEOTRELLIS_BOARD);

  for (uint8_t i = 0; i < 16; i++) {
    sc |= adafruit_neotrellis_activateKey(i, SEESAW_KEYPAD_EDGE_FALLING, true);
    sc |= adafruit_neotrellis_activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);
    adafruit_neotrellis_registerCallback(i, blink);
  }

  app_assert_status(sc);

  for (uint8_t i = 0; i < 16; i++) {
    adafruit_neotrellis_setPixelColor(i, 0xff00ff);
    adafruit_neotrellis_show();
    sl_sleeptimer_delay_millisecond(100);
    adafruit_neotrellis_setPixelColor(i, 0x000000);
    adafruit_neotrellis_show();
  }

  app_log("\rApplication Initialization done.\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  adafruit_neotrellis_read();
}

static TrellisCallback blink(keyEvent evt)
{
  static uint8_t i = 0;
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    adafruit_neotrellis_setPixelColor(evt.bit.NUM, color_list[i]);
    adafruit_neotrellis_show();
  } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    adafruit_neotrellis_setPixelColor(evt.bit.NUM, 0x000000);
    adafruit_neotrellis_show();
  }

  i = (i < 8) ? (i + 1) : 0;
  return 0;
}
