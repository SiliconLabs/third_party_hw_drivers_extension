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
#include "app_log.h"
#include "sl_sleeptimer.h"
#include "mikroe_lea6s.h"
#include "sl_iostream_init_usart_instances.h"
#include <string.h>
#include <stdint.h>
#include "app_queue.h"

#define PROCESS_RX_BUFFER_SIZE     500
#define PROCESS_PARSER_BUFFER_SIZE PROCESS_RX_BUFFER_SIZE
#define PROCESS_QUEUE_SIZE         (PROCESS_RX_BUFFER_SIZE * 2)

static void gps_process (void);
static void parser_application (void);
void timer_callback_fcn(sl_sleeptimer_timer_handle_t *handle, void *data);

static uint8_t uart_rx_buffer[PROCESS_RX_BUFFER_SIZE];
static uint8_t parser_buffer[PROCESS_PARSER_BUFFER_SIZE];

APP_QUEUE(uart_rx_queue, uint8_t, PROCESS_QUEUE_SIZE);
static sl_sleeptimer_timer_handle_t timer_handle;
static bool trigger_gps_process = false;

void app_init(void)
{
  app_log("Hello World GPS Click !!!\r\n");
  app_log("mikroe_lea6s_init = 0x%lx\r\n",
          mikroe_lea6s_init(sl_iostream_uart_mikroe_handle));
  app_log("app_queue_init = 0x%lx\r\n", APP_QUEUE_INIT(&uart_rx_queue,
                                                       uint8_t,
                                                       PROCESS_QUEUE_SIZE));
  mikroe_lea6s_wakeup();
  sl_sleeptimer_delay_millisecond(5000);
  app_log("mikroe_lea6s_wakeup done\r\n");

  app_log("sl_sleeptimer_start_periodic = 0x%lx\r\n",
          sl_sleeptimer_start_periodic_timer_ms(&timer_handle,
                                                1000,
                                                timer_callback_fcn,
                                                NULL,
                                                0,
                                                0));
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (trigger_gps_process) {
    trigger_gps_process = false;
    gps_process();
    parser_application();
  }
}

void timer_callback_fcn(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;

  if (handle == &timer_handle) {
    trigger_gps_process = true;
  }
}

static void gps_process(void)
{
  uint16_t actual_size;
  uint16_t index;
  sl_status_t stt = SL_STATUS_OK;

  stt = mikroe_lea6s_generic_read((uint8_t *) &uart_rx_buffer,
                                  PROCESS_RX_BUFFER_SIZE, &actual_size);

  if ((actual_size > 0) && (stt == SL_STATUS_OK)) {
    // Validation of the received data and push data to the queue
    for ( index = 0; index < actual_size; index++ )
    {
      if (uart_rx_buffer[index] == 0) {
        uart_rx_buffer[index] = 13;
      }

      if (false == app_queue_is_full(&uart_rx_queue)) {
        app_queue_add(&uart_rx_queue, (uart_rx_buffer + index));
      }
    }

    // Clear RX buffer
    memset(uart_rx_buffer, 0, PROCESS_RX_BUFFER_SIZE);
  }
}

static void parser_application(void)
{
  uint8_t element_buf[200] = { 0 };
  static uint16_t index = 0;
  mikroe_leas6_parser_result_t stt;
  uint8_t latitude_int[10];
  uint8_t latitude_decimal[10];
  uint8_t longitude_int[10];
  uint8_t longitude_decimal[10];

  while (false == app_queue_is_empty(&uart_rx_queue)) {
    app_queue_remove(&uart_rx_queue, parser_buffer + index);

    if (*(parser_buffer + index) == '\n') {
      stt = mikroe_lea6s_generic_parser(parser_buffer,
                                        gps_command_nema_gpgga_e,
                                        gpgga_element_latitude_e,
                                        element_buf);

      if ((strlen((const char *) element_buf) > 0)
          && (gps_parser_no_error_e == stt)) {
        /*
         * Latitude ranges between -90 and 90 degrees.
         * Longitude ranges between -180 and 180 degrees.
         *
         * Convert to degree calculation example:
         * Latitude: [N] 02451.71 is 24 degrees north latitude and 51.71
         *   minutes.
         * Convert the points to degrees 51.71 / 60 = 0.86183.
         * The actual Google Map corresponds to 24 + 0.86183 = +24.86183.
         * Longitude: [E] 12100.99 is 121 degrees east long and 0.99 points.
         * Convert the points to degrees 0.99 / 60 = 0.0165.
         * The actual Google Map is 121 + 0.0165 = +121.0165.
         * Combine the converted latitude and longitude data into
         * + 24.86183, + 121.0165 and enter the field of Google Map to find the
         * actual corresponding location.
         */
        app_log("**************************************************\r\n");

        memcpy((void *)latitude_int, (const void *)element_buf, 2);
        memcpy((void *)latitude_decimal,
               (const void *)(element_buf + 2),
               strlen((const char *)element_buf) - 2);
        double latitude = (atof((const char *)latitude_int)
                           + (atof((const char *)(latitude_decimal)) / 60.0));
        app_log("Latitude:  %.6f\r\n", latitude);
        memset(element_buf, 0, sizeof(element_buf));

        mikroe_lea6s_generic_parser(parser_buffer,
                                    gps_command_nema_gpgga_e,
                                    gpgga_element_longitude_e,
                                    element_buf);

        memcpy((void *)longitude_int, (const void *)element_buf, 3);
        memcpy((void *)longitude_decimal,
               (const void *) (element_buf + 3),
               strlen((const char *)element_buf) - 3);
        double longtitude = (atof((const char *)longitude_int)
                             + (atof((const char *)(longitude_decimal))
                                / 60.0));
        app_log("Longitude:  %.6f\r\n", longtitude);
        memset(element_buf, 0, sizeof(element_buf));

        mikroe_lea6s_generic_parser(parser_buffer,
                                    gps_command_nema_gpgga_e,
                                    gpgga_element_altitude_e,
                                    element_buf);
        app_log("Altitude: %s \r\n", element_buf);
      }

      index = 0;
      memset(parser_buffer, 0, PROCESS_PARSER_BUFFER_SIZE);
    } else {
      if (index < (PROCESS_PARSER_BUFFER_SIZE - 1)) {
        index++;
      }
    }
  }
}
