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
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "app_log.h"
#include "ir_generate.h"

volatile uint16_t timer_1ms = 0;
static bool repeat_flag = false;
static uint8_t restart_conter = 0;
void TIMER2_init(void);
void TIMER2_IRQHandler(void);

void TIMER2_IRQHandler(void)
{
  // Acknowledge the interrupt
  uint32_t flags = TIMER_IntGet(TIMER2);
  TIMER_IntClear(TIMER2, flags);
  timer_1ms++;
}

void TIMER2_init(void)
{
  uint32_t timerFreq = 0;

  CMU_ClockEnable(cmuClock_TIMER2, true);

  // Initialize the timer
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
  timerInit.prescale = timerPrescale2;
  timerInit.enable = false;

  // Configure but do not start the timer
  TIMER_Init(TIMER2, &timerInit);

  timerFreq = CMU_ClockFreqGet(cmuClock_TIMER2) / (timerInit.prescale + 1);
  TIMER_TopSet(TIMER2, (timerFreq / 1000));// 1000 for 1ms, 200 for 5ms

  // Start the timer
  TIMER_Enable(TIMER2, true);

  // Enable TIMER2 compare event interrupts to update the duty cycle
  TIMER_IntEnable(TIMER2, TIMER_IF_OF);
  NVIC_EnableIRQ(TIMER2_IRQn);
}

/**
 * @brief ir sent callback
 *
 * @param none
 *
 * @return none
 *
 */
void app_ir_complete(void)
{
  app_log("ir complete\r\n");
}

void app_init(void)
{
  TIMER2_init();
  app_log("test ir\r\n");

  ir_generate_init(CODE_NEC, app_ir_complete);

  // ir_generate_stream(0xFF, 0xFF, false); // for test
}

void app_process_action(void)
{
  uint8_t frame_interval[2] = { 108, 45 }; // 108 for NEC, 45 for SONY

  if (timer_1ms >= frame_interval[CODE_NEC]) {
    timer_1ms -= frame_interval[CODE_NEC];

    ir_generate_stream(0xFF, 0xFF, repeat_flag);   // for test

    // for NEC protocol, change after 4th repeat frame.
    if (restart_conter++ % 5 == 0) {
      repeat_flag = false;
    } else {
      repeat_flag = true;
    }
  }
}
