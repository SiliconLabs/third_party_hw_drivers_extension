/***************************************************************************//**
 * @file at_parser_platform.c
 * @brief AT command parser platform driver source
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

#include <string.h>
#include <sl_string.h>
#include "em_eusart.h"
#include "at_parser_platform.h"
#include "sl_iostream_handles.h"
#include "sl_sleeptimer.h"
#include "app_queue.h"
#include "mikroe_lte_iot2_bg96_config.h"

at_platform_status_t status = NOT_INITIALIZED;
ln_cb_t global_cb = 0;
sl_sleeptimer_timer_handle_t my_timer;
static uint8_t line_counter = 0;
static uint8_t input_buffer[IN_BUFFER_SIZE];
static uint16_t input_buffer_index = 0;
static sl_iostream_t *bg96_iostream_handle = NULL;

static void timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/**************************************************************************//**
 * @brief
 *    CMU initialization.
 *****************************************************************************/
void initCMU(void)
{
  // Enable clock to GPIO and EUSART1
  CMU_ClockEnable(cmuClock_GPIO, true);
}

/**************************************************************************//**
 * @brief
 *    GPIO initialization.
 *****************************************************************************/
void initGPIO(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(BG96_PWK_PORT,
                  BG96_PWK_PIN,
                  gpioModePushPull,
                  0);

  GPIO_PinModeSet(BG96_STA_PORT,
                  BG96_STA_PIN,
                  gpioModeInputPull,
                  0);
}

/**************************************************************************//**
 * @brief
 *   Initialization of platform driver.
 *
 * @param[in] line_callback
 *   Callback function for new line (and ">" character for special commands).
 *
 *****************************************************************************/
void at_platform_init(sl_iostream_t *iostream_handle, ln_cb_t line_callback)
{
  bg96_iostream_handle = iostream_handle;
  global_cb = line_callback;
  initCMU();
  initGPIO();
  status = READY;
}

/**************************************************************************//**
 * @brief
 *   Check ready status of platform driver.
 *
 *****************************************************************************/
sl_status_t at_platform_check_device_ready(void)
{
  sl_status_t stt = SL_STATUS_INVALID_STATE;
  switch (status) {
    case NOT_INITIALIZED:
      stt = SL_STATUS_NOT_INITIALIZED;
      break;
    case TRANSMIT:
      stt = SL_STATUS_BUSY;
      break;
    case READY:
      stt = SL_STATUS_OK;
      break;
    default:
      stt = SL_STATUS_INVALID_STATE;
  }
  return stt;
}

/**************************************************************************//**
 * @brief
 *   Platform driver send command function.
 *   This function adds \r to the command string.
 *   Command string SHALL be allocated until command is sent.
 *   This function uses UART TX and RX interrupt.
 *
 * @param[in] cmd
 *   Pointer to the command to send.
 *
 * @param[in] timeout_ms
 *    Timeout for the command in milliseconds.
 *
 * @return
 *   SL_STATUS_OK if there are no errors.
 *   SL_STATUS_ALLOCATION_FAILED if cmd == NULL.
 *****************************************************************************/
sl_status_t at_platform_send_cmd(volatile uint8_t *cmd,
                                 volatile uint16_t timeout_ms)
{
  sl_status_t st;

  if (NULL != cmd) {
    size_t cmd_length = sl_strlen((char *) cmd);
    if (cmd_length < CMD_MAX_SIZE - 1) {
      sl_strcat_s((char *) cmd, CMD_MAX_SIZE, "\r");
      sl_iostream_write(bg96_iostream_handle, (const void *)cmd,
                        sl_strlen((char *) cmd));

      line_counter = 0;
      status = TRANSMIT;
      st = sl_sleeptimer_restart_timer_ms(&my_timer, timeout_ms, timer_cb,
                                          (void *) NULL, 0, 0);
      return st;
    }
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *   Platform driver finish function.
 *   Used to end ongoing communication.
 *   Stops timeout and hardware interrupts.
 *
 *****************************************************************************/
void at_platform_finish_cmd(void)
{
  status = READY;
  sl_sleeptimer_stop_timer(&my_timer);
}

/**************************************************************************//**
 * @brief
 *   Platform driver process function.
 *   Used to process incoming uart rx data
 *
 *****************************************************************************/
void at_platform_process(void)
{
  static uint8_t tempdata;

  while (SL_STATUS_OK == sl_iostream_getchar(bg96_iostream_handle,
                                             ( char *)&tempdata)) {
    input_buffer[input_buffer_index] = tempdata;

    if (input_buffer_index == 0) {
      if ((input_buffer[0] == '\r') || (input_buffer[0] == '\n')) {
        input_buffer[0] = 0;
      } else {
        input_buffer_index++;
      }
    } else if (input_buffer_index == 1) {
      if ((input_buffer[1] == ' ') && (input_buffer[0] == '>')) {
        global_cb(input_buffer, ++line_counter);
        memset(input_buffer, 0, IN_BUFFER_SIZE);
        input_buffer_index = 0;
      } else {
        input_buffer_index++;
      }
    } else {
      if (input_buffer[input_buffer_index] == '\r') {
        input_buffer[input_buffer_index] = 0;
        global_cb(input_buffer, ++line_counter);
        memset((void *) input_buffer, 0, IN_BUFFER_SIZE);
        input_buffer_index = 0;
      } else if (input_buffer_index < (IN_BUFFER_SIZE - 1)) {
        input_buffer_index++;
      } else {
        global_cb(input_buffer, ++line_counter);
        memset(input_buffer, 0, IN_BUFFER_SIZE);
        input_buffer_index = 0;
      }
    }
  }
}

/**************************************************************************//**
 * @brief
 *   Timeout handler function.
 *
 * @param[in] handle
 *    Timer handler which called the callback function.
 *
 * @param [in] data
 *    Timer data sent by the caller of callback function.
 *
 *****************************************************************************/
static void timer_cb(sl_sleeptimer_timer_handle_t *handle,
                     void *data)
{
  (void) (data);
  (void) handle;
  status = READY;
  if (NULL != global_cb) {
    global_cb(NULL, 0);
  }
}
