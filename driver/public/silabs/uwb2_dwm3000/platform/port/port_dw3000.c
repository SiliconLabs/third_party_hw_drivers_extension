/*! ----------------------------------------------------------------------------
 * @file    port_dw3000.c
 * @brief   HW specific definitions and functions for DW3000 Interface
 *
 * @attention
 *
 * Copyright 2016 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author
 */

#include "port.h"
#include "em_gpio.h"
#include "sl_udelay.h"
#include "gpiointerrupt.h"

#if defined(HAL_UWB_WAKEUP_PORT) && defined(HAL_UWB_WAKEUP_PIN)
#define port_wakeup_start()  GPIO_PinModeSet(HAL_UWB_WAKEUP_PORT, \
                                             HAL_UWB_WAKEUP_PIN,  \
                                             gpioModePushPull,    \
                                             1);
#define port_wakeup_end()    GPIO_PinModeSet(HAL_UWB_WAKEUP_PORT, \
                                             HAL_UWB_WAKEUP_PIN,  \
                                             gpioModePushPull,    \
                                             0);
#else
#define port_wakeup_start()  GPIO_PinModeSet(HAL_SPI_CS_PORT,  \
                                             HAL_SPI_CS_PIN,   \
                                             gpioModePushPull, \
                                             0);
#define port_wakeup_end()    GPIO_PinModeSet(HAL_SPI_CS_PORT,  \
                                             HAL_SPI_CS_PIN,   \
                                             gpioModePushPull, \
                                             1);
#endif

/* @brief     manually configuring of EXTI priority
 * */
void init_dw3000_irq(void)
{
}

void disable_dw3000_irq(void)
{
  GPIO_IntDisable(1 << HAL_UWB_IRQ_IT_NBR);
}

void enable_dw3000_irq(void)
{
  GPIO_IntEnable(1 << HAL_UWB_IRQ_IT_NBR);
}

/* @fn      reset_DW1000
 * @brief   DW_RESET pin on DW1000 has 2 functions
 *          In general it is output, but it also can be used to reset the
 *          digital part of DW1000 by driving this pin low.
 *          Note, the DW_RESET pin should not be driven high externally.
 * */
void reset_DW3000(void)
{
  GPIO_PinModeSet(HAL_UWB_RST_PORT, HAL_UWB_RST_PIN, gpioModePushPull, 0);
  sl_udelay_wait(2000);
  GPIO_PinModeSet(HAL_UWB_RST_PORT, HAL_UWB_RST_PIN, gpioModeInputPull, 1);
  sl_udelay_wait(2000);
}

__attribute__((weak)) void wakeup_device_with_io(void)
{
  port_wakeup_dw3000_fast();
}

/* @fn      port_wakeup_dw3000_fast
 * @brief   waking up of DW3000 using DW_CS pin
 *
 *          the fast wakeup takes ~1ms:
 *          500us to hold the CS  - TODO: this time can be reduced
 *          500us to the crystal to startup
 *          + ~various time 100us...10ms
 * */
error_e port_wakeup_dw3000_fast(void)
{
  port_wakeup_start();
  sl_udelay_wait(500);
  port_wakeup_end();

  return _NO_ERR;
}

/* @fn      port_wakeup_dw3000
 * @brief   waking up of DW3000 using DW_CS pin
 *
 *          the fast wakeup takes ~1ms:
 *          500us to hold the CS  - TODO: this time can be reduced
 *          500us to the crystal to startup
 *          + ~various time 100us...10ms
 * */
error_e port_wakeup_dw3000(void)
{
  // device wake-up
  port_wakeup_dw3000_fast();

  // it takes ~500us for crystal startup total for the DW3000 to switch to
  //   RC_IDLE 120MHz
  sl_udelay_wait(500);

  // we are here on 120MHz RC

  /* Need to make sure DW IC is in IDLE_RC before proceeding */
  uint32_t cnt = 0;
  for (; (!dwt_checkidlerc()) && (cnt < 10); cnt++)
  {
    // FIXME: DW3000 B0 silicon bugfix
    if (cnt == 5) {
      port_wakeup_dw3000_fast();
    }
    sl_udelay_wait(20);
  }
  return (cnt >= 10) ? _ERR_INIT : _NO_ERR;
}

/**
 *  @brief     Bare-metal level
 *          initialise master/slave DW1000 (check if can talk to device and wake
 *   up and reset)
 */
static int port_init_device(void)
{
  set_dw_spi_slow_rate();

  if (dwt_check_dev_id() != DWT_SUCCESS) {
    return DWT_ERROR;
  }

  // clear the sleep bit in case it is set - so that after the hard reset below
  //   the DW does not go into sleep
  dwt_softreset(0);

  return 0;
}

error_e port_disable_wake_init_dw(void)
{
  disable_dw3000_irq();               /**< disable NVIC IRQ until we configure
                                       *   the device */

  reset_DW3000();

  // this is called here to wake up the device (i.e. if it was in sleep mode
  //   before the restart)
  port_wakeup_dw3000();

  if (port_init_device() != 0x00) {
    return _ERR_INIT;
  }
  return _NO_ERR;
}

error_e port_init_dw_chip(void)
{
  dwt_setlocaldataptr(0);

  int sc = openspi();
  if (0 != sc) {
    return _ERR_SPI;
  }
  return _NO_ERR;
}

void port_stop_all_UWB(void)
{
  port_disable_dw_irq_and_reset(1);
  dwt_setcallbacks(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

/******************************************************************************
*
*                              IRQ section
*
******************************************************************************/
/*! ----------------------------------------------------------------------------
 * Function: decamutexon()
 *
 * Description: This function should disable interrupts.
 *
 *
 * input parameters: void
 *
 * output parameters: uint16
 * returns the state of the DW1000 interrupt
 */
decaIrqStatus_t decamutexon(void)
{
  uint32_t s = GPIO_EnabledIntGet();
  if (s & (1 << HAL_UWB_IRQ_IT_NBR)) {
    GPIO_IntDisable(1 << HAL_UWB_IRQ_IT_NBR);
  }
  return s;
}

/*! ----------------------------------------------------------------------------
 * Function: decamutexoff()
 *
 * Description: This function should re-enable interrupts, or at least restore
 *              their state as returned(&saved) by decamutexon
 * This is called at the end of a critical section
 *
 * input parameters:
 * @param s - the state of the DW1000 interrupt as returned by decamutexon
 *
 * output parameters
 *
 * returns the state of the DW1000 interrupt
 */
void decamutexoff(decaIrqStatus_t s)
{
  if (s & (1 << HAL_UWB_IRQ_IT_NBR)) {
    GPIO_IntEnable(1 << HAL_UWB_IRQ_IT_NBR);
  }
}

/* @fn      process_deca_irq
 * @brief   main call-back for processing of DW3000 IRQ
 *          it re-enters the IRQ routing and processes all events.
 *          After processing of all events, DW3000 will clear the IRQ line.
 * */
static void process_deca_irq(uint8_t interrupt_number)
{
  (void)interrupt_number;

  while (GPIO_PinInGet(HAL_UWB_IRQ_PORT, HAL_UWB_IRQ_PIN) != 0)
  {
    dwt_isr();
  }   // while DW3000 IRQ line active
}

void dw_irq_init(void)
{
  GPIO_PinModeSet(HAL_UWB_IRQ_PORT, HAL_UWB_IRQ_PIN, gpioModeInputPull, 0);

  GPIOINT_CallbackRegister(HAL_UWB_IRQ_IT_NBR, process_deca_irq);
  GPIO_ExtIntConfig(HAL_UWB_IRQ_PORT, HAL_UWB_IRQ_PIN,
                    HAL_UWB_IRQ_IT_NBR, true, false, false);
}

/*
 * @brief disable DW_IRQ, reset DW3000
 *        and set
 *        app.DwCanSleep = DW_CANNOT_SLEEP;
 *        app.DwEnterSleep = DW_NOT_SLEEPING;
 * */
error_e port_disable_dw_irq_and_reset(int reset)
{
  disable_dw3000_irq();   /**< disable NVIC IRQ until we configure the device */

  // this is called to reset the DW device
  if (reset) {
    reset_DW3000();
  }

  return _NO_ERR;
}
