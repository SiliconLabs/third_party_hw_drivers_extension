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
#include <string.h>
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#include "sl_sleeptimer.h"
#include "mikroe_nt3h2111.h"
#include "sl_iostream.h"

sl_iostream_t *default_iostream;

// #define NTAG_I2C_BLOCK_SIZE 16

static uint16_t wdt_timer = 0;

static const uint8_t default_begining_of_memory[] = {
  0xAA, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0xE1, 0x10, 0x6D, 0x00
};

static const uint8_t ndef_rec[] = {
  0x03,          // NDEF Message
  15,            // Message size
  0xD1,          // Record header
  1,             // Type Length - 1 byte
  11,            // Payload Length - 11 bytes
  'U',           // Type / URI
  0x01,          // Payload
  's', 'i', 'l', 'a', 'b', 's', '.', 'c', 'o', 'm',
  0xFE           // NDEF Message End Mark
};

static const uint8_t null_block[] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static const uint8_t default_page_56[] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF
};

static const uint8_t default_page_57[] = {
  0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static const uint8_t default_page_58[] = {
  0x01, 0x00, 0xF8, 0x48,
  0x08, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static sl_status_t factory_reset_tag(void);
static sl_status_t test_read_session(void);
static sl_status_t test_write_session(void);
static uint8_t read_input (void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  /* Initialize NT3H2111 I2C communication. */
  nt3h2111_init(sl_i2cspm_mikroe);

  default_iostream = sl_iostream_get_default();

  /* Print project name. */
  app_log(
    "\r\n*********************************************************\r\n*\r\n");
  app_log("* Third Party Hardware Drivers - NT3H2111 - NFC Tag 2 Click\r\n");
  app_log("*\r\n*********************************************************\r\n");
  app_log("*\r\n* Give a input, your input should be between 0 and 4.\r\n");
  app_log("* Press enter when you finish.\r\n");
  app_log("*\r\n*  0. Factory reset Tag\r\n");
  app_log("*\r\n*  1. Read the session registers\r\n");
  app_log("*\r\n*  2. Write the session registers\r\n");
  app_log("*\r\n*  3. Write Tag\r\n");
  app_log("*\r\n*  4. Read Tag\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t stt;
  uint8_t input_cmd = 0;

  app_log("\r\nGive a your number, press enter when you finish: ");
  app_log("\r\n>");
  input_cmd = read_input();
  switch (input_cmd) {
    case 0:
      app_log("Start to reset the memory to its default values\r\n");
      stt = factory_reset_tag();
      if (stt == SL_STATUS_OK) {
        app_log("Reset successful!\r\n");
      } else {
        app_log("Reseting Failed with Err = 0x%lx\r\n", stt);
      }
      break;
    case 1:
      app_log("Start reading the content of Session Watchdog Timer\r\n");
      stt = test_read_session();
      if (stt == SL_STATUS_OK) {
        app_log("Current Watchdog Timer = %d\r\n", wdt_timer);
      } else {
        app_log("Read Watchdog Timer Failed with Err = 0x%lx\r\n", stt);
      }
      break;
    case 2:
      app_log("Start writing new value to the Session Watchdog Timer\r\n");
      stt = test_write_session();
      if (stt == SL_STATUS_OK) {
        app_log("Wrote successful Watchdog Timer to new value = %d\r\n",
                wdt_timer);
      } else {
        app_log("Set Watchdog Timer Failed with Err = 0x%lx\r\n", stt);
      }
      break;
    case 3:
      // Write an NDEF message into NT3H2111 EEPROM.
      app_log("Start writing an NDEF message into NT3H2111 EEPROM\r\n");
      stt = nt3h2111_write_bytes(16, ndef_rec, sizeof(ndef_rec));
      if (stt == SL_STATUS_OK) {
        app_log("Write Tag successful\r\n");
      } else {
        app_log("Writing Tag failed with Err = 0x%lx\r\n", stt);
      }
      break;
    case 4:
    {
      uint8_t buff[32];
      app_log("Start reading from NT3H2111 EEPROM\r\n");
      stt = nt3h2111_read_bytes(16, buff, 32);
      if (stt == SL_STATUS_OK) {
        app_log("Read Tag successful!\r\n");
        for (int i = 0; i < 8; i++) {
          app_log(" 0x%02x, 0x%02x, 0x%02x, 0x%02x\r\n",
                  buff[4 * i],
                  buff[4 * i + 1],
                  buff[4 * i + 2],
                  buff[4 * i + 3]);
        }
      } else {
        app_log("Read failed with Err = 0x%lx\r\n", stt);
      }
    }
    break;
  }
  app_log("*\r\n*********************************************************\r\n");
}

/*
 * Example how to use :
 *  - nt3h2111_write_bytes
 *  - nt3h2111_write_block
 *
 * Resets the memory to its default values
 */
static sl_status_t factory_reset_tag(void)
{
  sl_status_t result;

  // Reset default eeprom memory values (smart poster)
  result = nt3h2111_write_block(0, default_begining_of_memory);
  if (result != SL_STATUS_OK) {
    return result;
  }

  // Reset pages from 1 to 56
  uint8_t page = 1;
  while (page < 56) {
    result = nt3h2111_write_block(page, null_block);
    if (result != SL_STATUS_OK) {
      return result;
    }
    page++;
  }
  // Reset pages 56,57,58
  result = nt3h2111_write_block(56, default_page_56);
  if (result != SL_STATUS_OK) {
    return result;
  }
  result = nt3h2111_write_block(57, default_page_57);
  if (result != SL_STATUS_OK) {
    return result;
  }
  result = nt3h2111_write_block(58, default_page_58);
  if (result != SL_STATUS_OK) {
    return result;
  }

  return SL_STATUS_OK;
}

/*
 * Example how to use :
 *  - nfctag2_session_get
 *  - nfctag2_session_set
 *
 * Dumps the content of Session Watchdog Timer
 * Changing of session settings
 */
static sl_status_t test_read_session(void)
{
  sl_status_t stt;
  uint8_t temp[2];

  stt = nt3h2111_get_session(SESSION_WDT_LS, &temp[0]);
  stt |= nt3h2111_get_session(SESSION_WDT_MS, &temp[1]);
  if (stt == SL_STATUS_OK) {
    wdt_timer = (uint16_t)temp[1] << 8 | temp[0];
  }
  return stt;
}

static sl_status_t test_write_session(void)
{
  sl_status_t stt;

  wdt_timer++;
  stt = nt3h2111_set_session(SESSION_WDT_MS, 0xff, wdt_timer >> 8);
  stt |= nt3h2111_set_session(SESSION_WDT_LS, 0xff, wdt_timer & 0xff);
  return stt;
}

static uint8_t read_input(void)
{
  char char_buff;
  size_t bytes_read;
  uint8_t input_cmd = 0;
  uint8_t size_len = 0;

  while (1) {
    sl_iostream_read(default_iostream, &char_buff, 1, &bytes_read);

    if (bytes_read > 0) {
      if ((char_buff >= '0') && (char_buff <= '4')) {
        if (size_len < 1) {
          input_cmd = char_buff - '0';
          size_len++;
          app_log("%c", char_buff);
        } else {
          app_log(
            "\r\ntoo many digits. Please input a number between 0 and 4, press ENTER when you finish:");
          size_len = 0;
        }
      } else if (char_buff == '\r') {
        if (size_len == 0) {
          app_log(
            "\r\nPlease input a number between 0 and 9, press ENTER when you finish: ");
          continue;
        }
        app_log("\r\n");
        break;
      } else {
        app_log(
          "\r\nInvalid character. Please input a number between 0 and 4, press ENTER when you finish: ");
        size_len = 0;
      }
    }
  }

  return input_cmd;
}
