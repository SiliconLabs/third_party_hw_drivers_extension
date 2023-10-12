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
#include <stdlib.h>

#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "sl_iostream_init_usart_instances.h"
#include "sl_iostream_init_eusart_instances.h"

#include "app_log.h"
#include "app_assert.h"

#include "mikroe_stn1110.h"

#define PROCESS_BUFFER_SIZE   200
#define READING_INTERVAL_MSEC 1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

static uint8_t app_buf[PROCESS_BUFFER_SIZE] = { 0 };
static int32_t app_buf_len = 0;

/***************************************************************************//**
 * @brief
 *   This function clears memory of application buffer and reset its length.
 ******************************************************************************/
static void app_obdii_clear_buf(void);

/***************************************************************************//**
 * @brief
 *   This function logs data from application buffer.
 ******************************************************************************/
static void app_obdii_log_buf(void);

/***************************************************************************//**
 * @brief
 *   This function reads data from device and concatenates data to
 *   application buffer.
 * @return @retval SL_STATUS_OK Success
 *         @retval SL_STATUS_FAIL  Reading error
 ******************************************************************************/
static sl_status_t app_obdii_process(void);

/***************************************************************************//**
 * @brief
 *   This function checks for response and returns the status of response.
 * @param[in] rsp
 *   Expected response.
 * @return @retval SL_STATUS_OK Success
 *         @retval SL_STATUS_TIMEOUT Time-out
 *         @retval SL_STATUS_FAIL  Checking error
 ******************************************************************************/
static sl_status_t app_obdii_rsp_check(uint8_t *rsp);

static void application_task(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t ret_code;

  app_log_iostream_set(sl_iostream_vcom_handle);

  ret_code = mikroe_stn1110_init(sl_iostream_uart_mikroe_handle);
  app_assert_status(ret_code);
  app_log("Mikroe OBDii Click is initialized.\r\n");

  mikroe_stn1110_reset_device();
  sl_sleeptimer_delay_millisecond(1000);

  app_obdii_process();
  app_obdii_clear_buf();

  app_log("> Reset device\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_RESET_DEVICE);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  app_obdii_log_buf();
  sl_sleeptimer_delay_millisecond(1000);

  app_log(" Disable echo\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_DISABLE_ECHO);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  app_obdii_log_buf();

  app_log(" Remove spaces\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_SPACES_OFF);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  app_obdii_log_buf();

  ret_code = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                                   READING_INTERVAL_MSEC,
                                                   app_timer_cb,
                                                   (void *) NULL,
                                                   0,
                                                   0);
  app_assert_status(ret_code);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  application_task();
}

static void app_obdii_clear_buf(void)
{
  memset(app_buf, 0, app_buf_len);
  app_buf_len = 0;
}

static void application_task(void)
{
  uint8_t *start_ptr = NULL;
  char *ptr = NULL;
  uint8_t data_buf[5] = { 0 };
  uint16_t rpm = 0;
  uint8_t speed = 0;

  app_log(" Get current RPM\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_GET_CURRENT_RPM);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  start_ptr = (uint8_t *)strstr((const char *)app_buf,
                                MIKROE_OBDII_RSP_CURRENT_RPM);
  if (start_ptr) {
    memcpy(data_buf, (start_ptr + 4), 4);
    data_buf[4] = 0;
    rpm = (uint16_t)strtol((const char *)data_buf, &ptr, 16) / 4;
    app_log("RPM: %u\r\n\n>", rpm);
  } else {
    app_obdii_log_buf();
  }

  app_log(" Get current speed\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_GET_CURRENT_SPEED);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  start_ptr = (uint8_t *)strstr((const char *)app_buf,
                                MIKROE_OBDII_RSP_CURRENT_SPEED);
  if (start_ptr) {
    memcpy(data_buf, (start_ptr + 4), 2);
    data_buf[2] = 0;
    speed = (uint8_t)strtol((const char *)data_buf, &ptr, 16);
    app_log("Speed: %u km/h\r\n\n>", ( uint16_t ) speed);
  } else {
    app_obdii_log_buf();
  }
  sl_sleeptimer_delay_millisecond(1000);
}

static sl_status_t app_obdii_process(void)
{
  uint8_t rx_buf[PROCESS_BUFFER_SIZE] = { 0 };
  int32_t rx_size = 0;
  rx_size = mikroe_stn1110_generic_read(rx_buf, PROCESS_BUFFER_SIZE);
  if (rx_size > 0) {
    int32_t buf_cnt = app_buf_len;
    if (((app_buf_len + rx_size) > PROCESS_BUFFER_SIZE) && (app_buf_len > 0)) {
      buf_cnt = PROCESS_BUFFER_SIZE
                - ((app_buf_len + rx_size) - PROCESS_BUFFER_SIZE);
      memmove(app_buf, &app_buf[PROCESS_BUFFER_SIZE - buf_cnt], buf_cnt);
    }
    for ( int32_t rx_cnt = 0; rx_cnt < rx_size; rx_cnt++ )
    {
      if (rx_buf[rx_cnt]) {
        app_buf[buf_cnt++] = rx_buf[rx_cnt];
        if (app_buf_len < PROCESS_BUFFER_SIZE) {
          app_buf_len++;
        }
      }
    }
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}

static void app_obdii_log_buf(void)
{
  for ( int32_t buf_cnt = 0; buf_cnt < app_buf_len; buf_cnt++ )
  {
    app_log("%c", app_buf[buf_cnt]);
  }
}

static sl_status_t app_obdii_rsp_check(uint8_t *rsp)
{
  uint32_t timeout_cnt = 0;
  uint32_t timeout = 60000;
  app_obdii_clear_buf();
  app_obdii_process();
  while (0 == strstr((const char *)app_buf, (const char *)rsp))
  {
    app_obdii_process();
    if (timeout_cnt++ > timeout) {
      app_obdii_clear_buf();
      return SL_STATUS_TIMEOUT;
    }
    sl_sleeptimer_delay_millisecond(1);
  }
  sl_sleeptimer_delay_millisecond(100);
  app_obdii_process();
  if (strstr((const char *)app_buf, (const char *)rsp)) {
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
