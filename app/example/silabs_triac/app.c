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
#include "em_letimer.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "em_cmu.h"

#include "sl_simple_button_instances.h"
#include "triacdrv.h"

/*
 * Using the LFXO for the LETIMER clock, 32768/120 = 273.066666.  Note
 * that the output low/high time has to be half of one 60 Hz period,
 * thus the 120 Hz output overflow frequency for the LETIMER.  Use 273
 * for the counter top value.  Use 328 to get a 50 Hz output.
 */
#define LETIMER_TOP 273

// Duty cycle value
uint32_t triacDuty;   // initially off

/**************************************************************************//**
 * @brief
 *   Initialize the LETIMER to output a 50/60 Hz square wave for testing.
 *****************************************************************************/
void letimerLineHz(void)
{
  LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;

  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

  CMU_ClockEnable(cmuClock_LETIMER0, true);

  // Enable LETIMER0 output0 on PC14 (EXP header pin 12)
  LETIMER0->ROUTEPEN |= LETIMER_ROUTEPEN_OUT0PEN;
  LETIMER0->ROUTELOC0 = LETIMER_ROUTELOC0_OUT0LOC_LOC5;

  // Reload COMP0 on underflow, pulse output, and run in repeat mode
  letimerInit.comp0Top = true;
  letimerInit.ufoa0 = letimerUFOAToggle;
  letimerInit.topValue = LETIMER_TOP;

  // Need REP0 != 0 to pulse on underflow
  LETIMER_RepeatSet(LETIMER0, 0, 1);

  // Initialize and enable LETIMER
  LETIMER_Init(LETIMER0, &letimerInit);
}

void initGPIO(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);

  /*
   * Use minimum slew rate alternate setting for port C pins.  This
   * results in a 100 ns rise/fall time when viewed on a scope.  The
   * default primary slew rate of 5 is retained for other puns.
   */
  GPIO_SlewrateSet(gpioPortC, 5, 0);
  GPIO_PinModeSet(gpioPortC, 14, gpioModePushPullAlternate, 0);
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  TRIACDRV_Init_TypeDef triacInit = TRIAC_INIT_DEFAULT;

  initGPIO();

  letimerLineHz();

  triacInit.acmpInput = acmpInputAPORT0YCH0;  // PC0, EXP header pin 3
  triacInit.acmpPrsChannel = 3;
  triacInit.timerPrsBaseChannel = 4;
  triacInit.timerPrsOutputPort = gpioPortC;
  triacInit.timerPrsOutputPin = 8;
  triacInit.timerPrsOutputLoc = 0;            // PC8, EXP header pin 10
  triacInit.pulseWidth = TRIACDRV_MAX_ENABLE_PULSE_WIDTH;

  // Halt if initialization fails
  if (triacdrv_init(&triacInit) != SL_STATUS_OK) {
    __BKPT(0);
  }

  // Set initial duty cycle
  triacDuty = 25;
  triacdrv_set_duty_cycle(triacDuty);

  // Calibrate
  triacdrv_calibrate(60);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  // Sit in EM1 and wait for button presses
  EMU_EnterEM1();
}

/***************************************************************************//**
 * Callback on button change.
 ******************************************************************************/
void sl_button_on_change(const sl_button_t *handle)
{
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
    if (&sl_button_btn0 == handle) {
      // PB0 pressed; reduce duty cycle by 5%
      if (triacDuty > 0) {
        triacDuty -= 5;
      } else {
        triacDuty = 0;
      }
    } else if (&sl_button_btn1 == handle) {
      // PB1 pressed; increase duty cycle by 5%
      if (triacDuty < 100) {
        triacDuty += 5;
      } else {
        triacDuty = 100;
      }
    }

    triacdrv_set_duty_cycle(triacDuty);
  }
}
