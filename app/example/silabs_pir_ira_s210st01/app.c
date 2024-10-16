/***************************************************************************//**
 * @file
 * @brief PIR driver simple example.
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_emu.h"

#include "sl_simple_led.h"
#include "sl_simple_led_instances.h"

#include <pir_ira_s210st01.h>
#define QUEUE_LENGTH  4

static pir_sample_t pir_queue[QUEUE_LENGTH]; // User defined sample queue
static volatile bool adc_int = false;        // ADC interrupt flag

/***************************************************************************//**
 * @brief
 *   Motion detection callback
 *
 * @note
 *   Use the LED on Occupancy-Sensor-EXP board to indicate motion on/off
 ******************************************************************************/
static void pir_motion_detect_callback(bool motion_status)
{
  if (motion_status) {
    sl_led_turn_on(SL_SIMPLE_LED_INSTANCE(0));
  } else {
    sl_led_turn_off(SL_SIMPLE_LED_INSTANCE(0));
  }
}

/***************************************************************************//**
 * @brief
 *   ADC IRQ callback
 ******************************************************************************/
static void pir_adc_irq_callback(void)
{
  adc_int = true;
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  // Initialize PIR
  pir_init_t pir = PIR_INIT_DEFAULT;
  pir.opamp_mode = pir_opamp_mode_external;
  pir.motion_detection_callback = pir_motion_detect_callback;
  pir.sample_queue_size = QUEUE_LENGTH;
  pir.sample_queue = pir_queue;
  pir.use_timestamp = true;
  pir.adc_irq_callback = pir_adc_irq_callback;
  pir_init(&pir, true);

  // Start PIR
  pir_start();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  // Infinite loop
  if (adc_int) {
    adc_int = false;
    // Run motion detection algorithm
    pir_detect_motion();
    // Debug PIR samples if necessary
    while (1) {
      uint16_t size = 0;

      if ((SL_STATUS_OK == pir_get_queue_size(&size)) && (size > 0)) {
        pir_sample_t sample;

        pir_read_queue(&sample);
      } else {
        break;
      }
    }
  }
  EMU_EnterEM2(true);
}
