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
#include <string.h>

#include "sl_status.h"
#include "sl_simple_timer.h"
#include "sl_iostream_init_usart_instances.h"
#include "sl_iostream_init_eusart_instances.h"

#include "app_log.h"
#include "app_assert.h"

#include "mikroe_a172mrq.h"

#define PROCESS_COUTER                100

static uint8_t flag;
static sl_simple_timer_t a172mrq_timer;

static void app_a172mrq_timer_handle(sl_simple_timer_t *timer, void *data);
static void app_a172mrq_process(void);
static void app_a172mrq_reg_one(uint8_t fngr_number);
static void app_a172mrq_compare(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t ret_code;

  sl_iostream_set_default(sl_iostream_vcom_handle);
  app_log_iostream_set(sl_iostream_vcom_handle);

  // Initialize Fingerprint 2 Click
  mikroe_a172mrq_cfg_setup();
  ret_code = mikroe_a172mrq_init(sl_iostream_uart_mikroe_handle);
  app_assert_status(ret_code);

  // Reset Fingerprint 2 Click
  mikroe_a172mrq_reset();
  sl_sleeptimer_delay_millisecond(1000);
  app_log("Fingerprint 2 Click is initialized.\r\n");

  // Write fingerprint data at position 0
  app_a172mrq_reg_one(0);
  sl_sleeptimer_delay_millisecond(1000);

  ret_code = sl_simple_timer_start(&a172mrq_timer,
                                   5000,
                                   app_a172mrq_timer_handle,
                                   NULL,
                                   true);
  app_assert_status(ret_code);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}

static void app_a172mrq_process(void)
{
  int32_t rsp_size;
  sl_status_t ret_code;
  int32_t check_buf_cnt;
  uint8_t process_cnt = PROCESS_COUTER;
  char uart_rx_buffer[MIKROE_FP2_RX_BUFFER_SIZE] = { 0 };
  flag = 0;

  while (process_cnt != 0)
  {
    ret_code = mikroe_a172mrq_generic_read(uart_rx_buffer,
                                           MIKROE_FP2_RX_BUFFER_SIZE,
                                           &rsp_size);
    if (SL_STATUS_OK == ret_code) {
      if (rsp_size > 0) {
        // Validation of received data
        for (check_buf_cnt = 0; check_buf_cnt < rsp_size; check_buf_cnt++)
        {
          if (uart_rx_buffer[check_buf_cnt] == 0) {
            uart_rx_buffer[check_buf_cnt] = 13;
          }
        }
        app_log("%s", uart_rx_buffer);
        if (strstr(uart_rx_buffer, "</R>")) {
          flag = 1;
          process_cnt = 5;
        }
        // Clear RX buffer
        memset(uart_rx_buffer, 0, MIKROE_FP2_RX_BUFFER_SIZE);
      } else {
        process_cnt--;
        // Process delay
        sl_sleeptimer_delay_millisecond(100);
      }
    } else {
      app_log("Fingerprint processing failed!\r\n");
    }
  }
}

// Register a fingerprint on index
static void app_a172mrq_reg_one(uint8_t fngr_number)
{
  app_log("Registration is being processed.\r\n");
  sl_sleeptimer_delay_millisecond(500);

  mikroe_a172mrq_reg_one_fp(fngr_number);
  do{
    app_a172mrq_process();
  }
  while (flag == 0);
}

// Compare fingerprint on input with all other fingerprints that are memorized.
static void app_a172mrq_compare(void)
{
  sl_status_t ret_code;

  ret_code = mikroe_a172mrq_generic_write(MIKROE_FINGERPRINT2_CMD_FP_CMP,
                                          strlen(MIKROE_FINGERPRINT2_CMD_FP_CMP));
  if (SL_STATUS_OK == ret_code) {
    app_log("Comparison is being processed.\r\n");
    app_a172mrq_process();

    do
    {
      app_a172mrq_process();
    }
    while (flag == 0);
  } else {
    app_log("Comparison processing failed!\r\n");
  }
}

static void app_a172mrq_timer_handle(sl_simple_timer_t *timer, void *data)
{
  (void)timer;
  (void)data;

  app_a172mrq_compare();
}
