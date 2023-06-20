/***************************************************************************//**
 * @file
 * @brief PIR driver.
 * @version 1.0.2
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
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_prs.h"
#include "em_cryotimer.h"
#include "em_opamp.h"
#include "em_adc.h"
#include "em_vdac.h"
#include "em_opamp.h"
#include "em_letimer.h"

#include <pir_ira_s210st01.h>
#include <pir_ira_s210st01_config.h>
#include <stdbool.h>
#include <stdio.h>

#define ADC_PERIPHERAL           ADC0
#define ADC_PERIPHERAL_NO        0

#define ADC_SAMPLE_QUEUE_SIZE    4         // Must be at least 4

/// The adcSource variable should be set to adcSourceAdcDiff for PIR operation.
/// The single ended modes are additional modes for signal chain analysis with a
///   voltage source.
#define PIR_SOURCE_POS           0
#define PIR_SOURCE_NEG           1
#define PIR_SOURCE_DIFF          2
#if defined(PIR_IRA_S210ST01_ADC_P_PORT) && \
  defined(PIR_IRA_S210ST01_ADC_P_PIN) &&    \
  defined(PIR_IRA_S210ST01_ADC_N_PORT) &&   \
  defined(PIR_IRA_S210ST01_ADC_N_PIN)
#define PIR_SOURCE               PIR_SOURCE_DIFF
#elif defined(PIR_IRA_S210ST01_ADC_P_PORT) && \
  defined(PIR_IRA_S210ST01_ADC_P_PIN)
#define PIR_SOURCE               PIR_SOURCE_POS
#else
#define PIR_SOURCE               PIR_SOURCE_NEG
#error "The posSel of the ADC is not defined for the kit, only the negSel"
#endif

/// PIR sample queue structure
typedef struct {
  volatile uint16_t head;                  ///< Index of next byte to get.
  volatile uint16_t tail;                  ///< Index of where to enqueue next
                                           ///<   byte.
  volatile uint16_t used;                  ///< Number of bytes queued.
  uint16_t size;                           ///< Size of queue.
  pir_sample_t *sample;                    ///< Pointer to PIR sample buffer.
} sample_queue_t;

static pir_init_t pir_instance;      // An instance that holds the configuration
static uint32_t lockout_counter = 0; // The time, in samples, remaining on the
                                     //   lockout window after detection.
static int32_t win_base = 0;         // The low frequency mean of the PIR input
                                     //   signal.

static sample_queue_t app_queue;   // Buffer PIR samples for application layer
                                   //   debugging.
static sample_queue_t adc_queue;   // Buffer ADC samples for motion detection
                                   //   algorithm.
static pir_sample_t adc_sample_queue[ADC_SAMPLE_QUEUE_SIZE]; // ADC sample
                                                             //   buffer.

#if PIR_SOURCE == PIR_SOURCE_DIFF || PIR_SOURCE == PIR_SOURCE_POS
static const ADC_PosSel_TypeDef adc_aportx_table_A[] = {
  adcPosSelAPORT3XCH8,
  adcPosSelAPORT4XCH9,
  adcPosSelAPORT3XCH10,
  adcPosSelAPORT4XCH11,
  adcPosSelAPORT3XCH12,
  adcPosSelAPORT4XCH13,
  adcPosSelAPORT3XCH14,
  adcPosSelAPORT4XCH15
};
static const ADC_PosSel_TypeDef adc_aportx_table_B[] = {
  adcPosSelAPORT3XCH22,
  adcPosSelAPORT4XCH23,
  adcPosSelAPORT3XCH24,
  adcPosSelAPORT4XCH25,
  adcPosSelAPORT3XCH26,
  adcPosSelAPORT4XCH27,
  adcPosSelAPORT3XCH28,
  adcPosSelAPORT4XCH29,
  adcPosSelAPORT3XCH30,
  adcPosSelAPORT4XCH31
};
static const ADC_PosSel_TypeDef adc_aportx_table_C[] = {
  adcPosSelAPORT1XCH0,
  adcPosSelAPORT2XCH1,
  adcPosSelAPORT1XCH2,
  adcPosSelAPORT2XCH3,
  adcPosSelAPORT1XCH4,
  adcPosSelAPORT2XCH5,
  adcPosSelAPORT1XCH6,
  adcPosSelAPORT2XCH7,
  adcPosSelAPORT1XCH8,
  adcPosSelAPORT2XCH9,
  adcPosSelAPORT1XCH10,
  adcPosSelAPORT2XCH11,
};
static const ADC_PosSel_TypeDef adc_aportx_table_D[] = {
  adcPosSelAPORT3XCH0,
  adcPosSelAPORT4XCH1,
  adcPosSelAPORT3XCH2,
  adcPosSelAPORT4XCH3,
  adcPosSelAPORT3XCH4,
  adcPosSelAPORT4XCH5,
  adcPosSelAPORT3XCH6,
  adcPosSelAPORT4XCH7,
};
static const ADC_PosSel_TypeDef adc_aportx_table_F[] = {
  adcPosSelAPORT1XCH16,
  adcPosSelAPORT2XCH17,
  adcPosSelAPORT1XCH18,
  adcPosSelAPORT2XCH19,
  adcPosSelAPORT1XCH20,
  adcPosSelAPORT2XCH21,
  adcPosSelAPORT1XCH22,
  adcPosSelAPORT2XCH23,
  adcPosSelAPORT1XCH24,
  adcPosSelAPORT2XCH25,
  adcPosSelAPORT1XCH26,
  adcPosSelAPORT2XCH27,
  adcPosSelAPORT1XCH28,
  adcPosSelAPORT2XCH29,
  adcPosSelAPORT1XCH30,
  adcPosSelAPORT2XCH31
};
static const ADC_PosSel_TypeDef adc_aportx_table_I[] = {
  adcPosSelAPORT0XCH0,
  adcPosSelAPORT0XCH1,
  adcPosSelAPORT0XCH2,
  adcPosSelAPORT0XCH3
};
#endif

#if PIR_SOURCE == PIR_SOURCE_DIFF || PIR_SOURCE == PIR_SOURCE_NEG
static const ADC_NegSel_TypeDef adc_aporty_table_A[] = {
  adcNegSelAPORT4YCH8,
  adcNegSelAPORT3YCH9,
  adcNegSelAPORT4YCH10,
  adcNegSelAPORT3YCH11,
  adcNegSelAPORT4YCH12,
  adcNegSelAPORT3YCH13,
  adcNegSelAPORT4YCH14,
  adcNegSelAPORT3YCH15
};
static const ADC_NegSel_TypeDef adc_aporty_table_B[] = {
  adcNegSelAPORT4YCH22,
  adcNegSelAPORT3YCH23,
  adcNegSelAPORT4YCH24,
  adcNegSelAPORT3YCH25,
  adcNegSelAPORT4YCH26,
  adcNegSelAPORT3YCH27,
  adcNegSelAPORT4YCH28,
  adcNegSelAPORT3YCH29,
  adcNegSelAPORT4YCH30,
  adcNegSelAPORT3YCH31
};
static const ADC_NegSel_TypeDef adc_aporty_table_C[] = {
  adcNegSelAPORT2YCH0,
  adcNegSelAPORT1YCH1,
  adcNegSelAPORT2YCH2,
  adcNegSelAPORT1YCH3,
  adcNegSelAPORT2YCH4,
  adcNegSelAPORT1YCH5,
  adcNegSelAPORT2YCH6,
  adcNegSelAPORT1YCH7,
  adcNegSelAPORT2YCH8,
  adcNegSelAPORT1YCH9,
  adcNegSelAPORT2YCH10,
  adcNegSelAPORT1YCH11,
};
static const ADC_NegSel_TypeDef adc_aporty_table_D[] = {
  adcNegSelAPORT4YCH0,
  adcNegSelAPORT3YCH1,
  adcNegSelAPORT4YCH2,
  adcNegSelAPORT3YCH3,
  adcNegSelAPORT4YCH4,
  adcNegSelAPORT3YCH5,
  adcNegSelAPORT4YCH6,
  adcNegSelAPORT3YCH7,
};
static const ADC_NegSel_TypeDef adc_aporty_table_F[] = {
  adcNegSelAPORT2YCH16,
  adcNegSelAPORT1YCH17,
  adcNegSelAPORT2YCH18,
  adcNegSelAPORT1YCH19,
  adcNegSelAPORT2YCH20,
  adcNegSelAPORT1YCH21,
  adcNegSelAPORT2YCH22,
  adcNegSelAPORT1YCH23,
  adcNegSelAPORT2YCH24,
  adcNegSelAPORT1YCH25,
  adcNegSelAPORT2YCH26,
  adcNegSelAPORT1YCH27,
  adcNegSelAPORT2YCH28,
  adcNegSelAPORT1YCH29,
  adcNegSelAPORT2YCH30,
  adcNegSelAPORT1YCH31
};
static const ADC_NegSel_TypeDef adc_aporty_table_I[] = {
  adcNegSelAPORT0YCH0,
  adcNegSelAPORT0YCH1,
  adcNegSelAPORT0YCH2,
  adcNegSelAPORT0YCH3
};
#endif

#if PIR_SOURCE == PIR_SOURCE_DIFF || PIR_SOURCE == PIR_SOURCE_POS
static ADC_PosSel_TypeDef get_adc_aportx(GPIO_Port_TypeDef port_index,
                                         unsigned int pin_index)
{
  switch (port_index) {
    case gpioPortA:
      if (pin_index <= 7) {
        return adc_aportx_table_A[pin_index];
      } else {
        return adcPosSelVSS;
      }

    case gpioPortB:
      if ((pin_index >= 6) && (pin_index <= 15)) {
        return adc_aportx_table_B[pin_index - 6];
      } else {
        return adcPosSelVSS;
      }

    case gpioPortC:
      if (pin_index <= 11) {
        return adc_aportx_table_C[pin_index];
      } else {
        return adcPosSelVSS;
      }

    case gpioPortD:
      if ((pin_index >= 8) && (pin_index <= 15)) {
        return adc_aportx_table_D[pin_index - 8];
      } else {
        return adcPosSelVSS;
      }

    case gpioPortF:
      if (pin_index <= 15) {
        return adc_aportx_table_F[pin_index];
      } else {
        return adcPosSelVSS;
      }

    case gpioPortI:
      if (pin_index <= 3) {
        return adc_aportx_table_I[pin_index];
      } else {
        return adcPosSelVSS;
      }

    default:
      return adcPosSelVSS;
  }
  return adcPosSelVSS;
}

#endif

#if PIR_SOURCE == PIR_SOURCE_DIFF || PIR_SOURCE == PIR_SOURCE_NEG
static ADC_NegSel_TypeDef get_adc_aporty(GPIO_Port_TypeDef port_index,
                                         unsigned int pin_index)
{
  switch (port_index) {
    case gpioPortA:
      if (pin_index <= 7) {
        return adc_aporty_table_A[pin_index];
      } else {
        return adcNegSelVSS;
      }

    case gpioPortB:
      if ((pin_index >= 6) && (pin_index <= 15)) {
        return adc_aporty_table_B[pin_index - 6];
      } else {
        return adcNegSelVSS;
      }

    case gpioPortC:
      if (pin_index <= 11) {
        return adc_aporty_table_C[pin_index];
      } else {
        return adcNegSelVSS;
      }

    case gpioPortD:
      if ((pin_index >= 8) && (pin_index <= 15)) {
        return adc_aporty_table_D[pin_index - 8];
      } else {
        return adcNegSelVSS;
      }

    case gpioPortF:
      if (pin_index <= 15) {
        return adc_aporty_table_F[pin_index];
      } else {
        return adcNegSelVSS;
      }

    case gpioPortI:
      if (pin_index <= 3) {
        return adc_aporty_table_I[pin_index];
      } else {
        return adcNegSelVSS;
      }

    default:
      return adcNegSelVSS;
  }
  return adcNegSelVSS;
}

#endif

/***************************************************************************//**
 * @brief
 *  Updates the ADC hardware thresholds for a motion event trigger.
 *
 *  Inputs must mast the conversion data representation. For oversampled
 *  conversions, the resolution is 16-bit, regardless if the OVS setting
 *  does not achieve 16-bit resolution.
 *
 * @param[in] win_base
 *  The midpoint of the window. Thresholds are the base +/- 0.5*winSize
 *
 * @param[in] win_size
 *  The total peak to peak width of the window.
 ******************************************************************************/
static void update_adc_thresholds(int32_t win_base, uint32_t win_size)
{
  int32_t pos_thresh = win_base + win_size / 2;
  int32_t neg_thresh = win_base - win_size / 2;

  // Ensure thresholds are within int16_t for ADC window comparator.
  int32_t pos_thresh_max, neg_thresh_min;
#if PIR_SOURCE == PIR_SOURCE_DIFF
  pos_thresh_max = 32767;
  neg_thresh_min = -32768;
#else
  pos_thresh_max = 65535;
  neg_thresh_min = 0;
#endif

  if (pos_thresh > pos_thresh_max) {
    pos_thresh = pos_thresh_max;
  }
  if (neg_thresh < neg_thresh_min) {
    neg_thresh = neg_thresh_min;
  }

  ADC0->CMPTHR =
    ((pos_thresh << _ADC_CMPTHR_ADGT_SHIFT) & _ADC_CMPTHR_ADGT_MASK)
    | ((neg_thresh <<
        _ADC_CMPTHR_ADLT_SHIFT) & _ADC_CMPTHR_ADLT_MASK);
}

/***************************************************************************//**
 * @brief
 *  Initializes the non inverting amplifier for PIR signal chain.
 ******************************************************************************/
static void init_opamp(void)
{
  CMU_ClockEnable(cmuClock_VDAC0, true);

  // Errata VDAC_E201 for PG/JG/MG/BG12 causes contention if APORT is used for
  //   output.
  // Use of dedicated opamp output pin or careful mapping of APORT buses is
  //   recommended.
  OPAMP_Init_TypeDef opaInit = OPA_INIT_NON_INVERTING;
  opaInit.resInMux = opaResInMuxDisable;
  opaInit.resSel = opaResSelDefault;

  opaInit.posSel = opaPosSelPosPad;
  opaInit.negSel = opaNegSelNegPad;
  opaInit.outMode = opaOutModeDisable;
  opaInit.outPen = VDAC_OPA_OUT_MAINOUTEN;

  const uint32_t opampCh = 1;
  OPAMP_Enable(VDAC0, OPA1, &opaInit);

  // Set INCBW for 2.5x increase in GBW, only stable for G > 3.
  VDAC0->OPA[opampCh].CTRL |= VDAC_OPA_CTRL_INCBW;

  // Set DRIVESTRENGTH=0 to minimize current.
  VDAC0->OPA[opampCh].CTRL &= ~_VDAC_OPA_CTRL_DRIVESTRENGTH_MASK;
  VDAC0->OPA[opampCh].CTRL |= (0 << _VDAC_OPA_CTRL_DRIVESTRENGTH_SHIFT);
}

/***************************************************************************//**
 * @brief
 *  Initializes the ADC, CRYOTIMER and PRS for PIR operation.
 *
 * @details
 *   PRS triggering of the ADC in EM2 is only possible on > Series 1 MCUs.
 *
 * @param[in] adc_enter_em2
 *  Sets up ADC to run in EM2 when set to true.
 ******************************************************************************/
static sl_status_t init_adc(bool adc_enter_em2)
{
  // Initialize the ADC.
  CMU_ClockEnable(cmuClock_ADC0, true);

  ADC_Init_TypeDef adcInit = ADC_INIT_DEFAULT;
  adcInit.ovsRateSel = adcOvsRateSel2;

  if (adc_enter_em2) {
    CMU_AUXHFRCOBandSet(cmuAUXHFRCOFreq_4M0Hz);
    CMU->ADCCTRL = CMU_ADCCTRL_ADC0CLKSEL_AUXHFRCO;
    adcInit.em2ClockConfig = adcEm2ClockOnDemand;
    adcInit.prescale = ADC_PrescaleCalc(cmuAUXHFRCOFreq_4M0Hz,
                                        CMU_AUXHFRCOBandGet());
    adcInit.timebase = ADC_TimebaseCalc(CMU_AUXHFRCOBandGet());
  } else {
    adcInit.timebase = ADC_TimebaseCalc(0);
  }

  ADC_InitSingle_TypeDef adcInitSingle = ADC_INITSINGLE_DEFAULT;
  adcInitSingle.resolution = adcResOVS;
  adcInitSingle.reference = adcRef1V25;
  adcInitSingle.prsEnable = true;
  adcInitSingle.prsSel = adcPRSSELCh0;
  adcInitSingle.leftAdjust = true;
  adcInitSingle.acqTime = adcAcqTime64;

#if PIR_SOURCE == PIR_SOURCE_POS
  adcInitSingle.posSel = get_adc_aportx(PIR_IRA_S210ST01_ADC_P_PORT,
                                        PIR_IRA_S210ST01_ADC_P_PIN);
  adcInitSingle.negSel = adcNegSelVSS;
  if (adcInitSingle.posSel == adcPosSelVSS) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }
  adcInitSingle.diff = false;
#elif PIR_SOURCE == PIR_SOURCE_NEG
  // The posSel of the ADC_N is not defined for the kit, only the negSel.
  adcInitSingle.posSel = adcPosSelAVDD;
  adcInitSingle.negSel = get_adc_aporty(PIR_IRA_S210ST01_ADC_N_PORT,
                                        PIR_IRA_S210ST01_ADC_N_PIN);
  if (adcInitSingle.negSel == adcNegSelVSS) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }
  adcInitSingle.diff = false;
#elif PIR_SOURCE == PIR_SOURCE_DIFF
  adcInitSingle.posSel = get_adc_aportx(PIR_IRA_S210ST01_ADC_P_PORT,
                                        PIR_IRA_S210ST01_ADC_P_PIN);
  adcInitSingle.negSel = get_adc_aporty(PIR_IRA_S210ST01_ADC_N_PORT,
                                        PIR_IRA_S210ST01_ADC_N_PIN);
  if ((adcInitSingle.posSel == adcPosSelVSS)
      || (adcInitSingle.negSel == adcNegSelVSS)) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }
  adcInitSingle.diff = true;
#endif

  ADC_Init(ADC0, &adcInit);
  ADC_InitSingle(ADC0, &adcInitSingle);

  // Set ADC FIFO level to max to minimize EM0 wakeups when the ADC sample
  //   remains within window.
  static const uint32_t dataValidLevel = 3; // ADC SINGLE IRQ is when DVL+1
                                            //   single channels are available
                                            //   in the FIFO.
  ADC0->SINGLECTRLX &= ~_ADC_SINGLECTRLX_DVL_MASK;
  ADC0->SINGLECTRLX |= (dataValidLevel << _ADC_SINGLECTRLX_DVL_SHIFT);

  update_adc_thresholds(win_base, pir_instance.win_size);
//
//  // Initialize the CRYOTIMER.
//  CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
  CMU_ClockEnable(cmuClock_CRYOTIMER, true);

  // Run CRYOTIMER on ULFRCO (1024 Hz) and trigger ADC to set the sampling
  //   period.
  CRYOTIMER_Init_TypeDef cryoInit = CRYOTIMER_INIT_DEFAULT;
  cryoInit.osc = cryotimerOscULFRCO;
  cryoInit.presc = cryotimerPresc_1;
  cryoInit.period = cryotimerPeriod_8; // Sampling frequency is 1024 / 32 = 32
                                       //   Hz.
  CRYOTIMER_Init(&cryoInit);

  // PRS is the triggering connection between CRYOTIMER and ADC.
  CMU_ClockEnable(cmuClock_PRS, true);
  PRS_SourceAsyncSignalSet(0,
                           PRS_CH_CTRL_SOURCESEL_CRYOTIMER,
                           PRS_CH_CTRL_SIGSEL_CRYOTIMERPERIOD);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Initialize GPIO.
 ******************************************************************************/
static void init_gpio(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(PIR_IRA_S210ST01_LDO_SHDN_B_PORT,
                  PIR_IRA_S210ST01_LDO_SHDN_B_PIN,
                  gpioModePushPull,
                  0);
#if defined(PIR_IRA_S210ST01_ADC_P_PORT) && defined(PIR_IRA_S210ST01_ADC_P_PIN)
  // ADC_P
  GPIO_PinModeSet(PIR_IRA_S210ST01_ADC_P_PORT,
                  PIR_IRA_S210ST01_ADC_P_PIN,
                  gpioModeDisabled,
                  0);
#endif
#if defined(PIR_IRA_S210ST01_ADC_N_PORT) && defined(PIR_IRA_S210ST01_ADC_N_PIN)
  // ADC_N
  GPIO_PinModeSet(PIR_IRA_S210ST01_ADC_N_PORT,
                  PIR_IRA_S210ST01_ADC_N_PIN,
                  gpioModeDisabled,
                  0);
#endif
}

/***************************************************************************//**
 * @brief
 *  Initialize timers for time stamping samples to the GUI.
 *
 * @details
 *  LETIMER0 will operate at 1024 Hz time stamp clock.
 ******************************************************************************/
static void init_timestamp_clock(void)
{
  // LETIMER Initialization
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
  CMU_ClockEnable(cmuClock_HFLE, true);
  CMU_ClockEnable(cmuClock_LETIMER0, true);
  CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_32); // 32,768 Hz / 32 = 1024 Hz
                                                    //   = 976.5625 us period

  LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;
  LETIMER_Init(LETIMER0, &letimerInit);
}

/***************************************************************************//**
 * @brief
 *   Enqueue the PIR sample.
 ******************************************************************************/
static void enqueue_sample(sample_queue_t *queue, pir_sample_t sample)
{
  queue->sample[queue->head] = sample;
  queue->head++;
  if (queue->head == queue->size) {
    queue->head = 0;
  }
  queue->used++;

  if (queue->head == queue->tail) {
    // If buffer is full, drop the oldest sample in the circular buffer and
    //   update the tail pointer.
    queue->tail++;
    if (queue->tail == queue->size) {
      queue->tail = 0;
    }
    queue->used = queue->size;
  }
}

/***************************************************************************//**
 * @brief
 *   Dequeue the PIR sample.
 ******************************************************************************/
static void dequeue_sample(sample_queue_t *queue, pir_sample_t *sample)
{
  if (queue->used < 1) {
    sample = NULL;
  }

  *sample = queue->sample[queue->tail];
  queue->tail++;
  if (queue->tail == queue->size) {
    queue->tail = 0;
  }
  queue->used -= 1;
}

/***************************************************************************//**
 * @brief
 *   Initializes peripherals for PIR
 *
 * @param[in] pir_init
 *   A pointer to PIR initialization structure
 *
 * @param[in] adc_enter_em2
 *   Sets up ADC to run in EM2 when set to true
 ******************************************************************************/
sl_status_t pir_init(pir_init_t *pir_init, bool adc_enter_em2)
{
  init_gpio();
  // Enable LDO to startup VPIR.
  GPIO_PinOutSet(PIR_IRA_S210ST01_LDO_SHDN_B_PORT,
                 PIR_IRA_S210ST01_LDO_SHDN_B_PIN);

  pir_instance.opamp_mode = pir_init->opamp_mode;
  pir_instance.motion_on_time = pir_init->motion_on_time;
  pir_instance.win_size = pir_init->win_size;
  pir_instance.motion_detection_callback = pir_init->motion_detection_callback;
  pir_instance.adc_irq_callback = pir_init->adc_irq_callback;
  pir_instance.use_timestamp = pir_init->use_timestamp;
  if ((pir_instance.motion_detection_callback == NULL)
      || (pir_instance.adc_irq_callback == NULL)) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }

  app_queue.size = pir_init->sample_queue_size;
  app_queue.sample = pir_init->sample_queue;
  if (app_queue.size < 4) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }

  adc_queue.size = ADC_SAMPLE_QUEUE_SIZE;
  adc_queue.sample = adc_sample_queue;

  // Peripheral initialization
  if (pir_init->opamp_mode == pir_opamp_mode_internal) {
    init_opamp();
  }
  if (pir_init->use_timestamp) {
    init_timestamp_clock();
  }
  if (SL_STATUS_OK == init_adc(adc_enter_em2)) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }

  // Default to motion off state
  pir_instance.motion_detection_callback(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Starts PIR sensor sampling.
 ******************************************************************************/
sl_status_t pir_start(void)
{
  lockout_counter = 0;

  ADC_Start(ADC0, adcStartSingle);
  // Wake up on FIFO reaching threshold.
  ADC_IntEnable(ADC0, ADC_IF_SINGLE);
  // Wake up on ADC exceeding window threshold.
  ADC_IntEnable(ADC0, ADC_IF_SINGLECMP);
  NVIC_EnableIRQ(ADC0_IRQn);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Stops PIR sensor sampling.
 ******************************************************************************/
sl_status_t pir_stop(void)
{
  ADC_IntDisable(ADC0, ADC_IF_SINGLE);
  ADC_IntDisable(ADC0, ADC_IF_SINGLECMP);
  NVIC_DisableIRQ(ADC0_IRQn);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Runs the motion detection algorithm
 *
 * @note
 *   This algorithms takes roughly 150us and must run on the latest ADC sample
 *   after receiving the ADC interrupt.
 ******************************************************************************/
sl_status_t pir_detect_motion(void)
{
  bool motion = false;
  uint32_t num_of_samples = 0;
  pir_sample_t adc_sample;
  pir_sample_t pir_sample;
  int32_t adc_thresh_high, adc_thresh_low;

  // Read out each ADC sample, update the LPF mean, and check if it was within
  //   the window thresholds
  while (adc_queue.used > 0) {
    num_of_samples++;
    dequeue_sample(&adc_queue, &adc_sample);
    // Capture the ADC and winBase of the current sample.
    pir_sample.adc_sample = adc_sample.adc_sample;
    pir_sample.win_base = win_base;
#if PIR_SOURCE == PIR_SOURCE_DIFF
    adc_thresh_high =
      ((int16_t) ((ADC0->CMPTHR & _ADC_CMPTHR_ADGT_MASK) >>
                  _ADC_CMPTHR_ADGT_SHIFT));
    adc_thresh_low =
      ((int16_t) ((ADC0->CMPTHR & _ADC_CMPTHR_ADLT_MASK) >>
                  _ADC_CMPTHR_ADLT_SHIFT));
#elif PIR_SOURCE == PIR_SOURCE_POS || PIR_SOURCE == PIR_SOURCE_NEG
    adc_thresh_high =
      ((uint16_t) ((ADC0->CMPTHR & _ADC_CMPTHR_ADGT_MASK) >>
                   _ADC_CMPTHR_ADGT_SHIFT));
    adc_thresh_low =
      ((uint16_t) ((ADC0->CMPTHR & _ADC_CMPTHR_ADLT_MASK) >>
                   _ADC_CMPTHR_ADLT_SHIFT));
#endif

    // If window was broken, move window thresholds to include the latest ADC
    //   reading.
    // Thresholds are recalculated in software because samples are batch
    //   processed.
    if (adc_sample.adc_sample > adc_thresh_high) {
      win_base = adc_sample.adc_sample - pir_instance.win_size / 2;
      motion = true;
    } else if (adc_sample.adc_sample < adc_thresh_low) {
      win_base = adc_sample.adc_sample + pir_instance.win_size / 2;
      motion = true;
    } else {
      // Window was not broken, update winBase to follow the low frequency drift
      //   using a DT 1st order LPF.
      // Let a = 2^-a_shift
      // Equivalent continuous RC is given by Ts * (1-a) / a
      uint32_t a_shift = 5;
      win_base =
        (adc_sample.adc_sample >> a_shift) + (win_base - (win_base >> a_shift));
    }

    update_adc_thresholds(win_base, pir_instance.win_size);

    // Enqueue the PIR sample
    pir_sample.timestamp_ms = adc_sample.timestamp_ms;
    pir_sample.motion_status = (lockout_counter > 0 || motion);
    pir_sample.adc_upper_threshold = adc_thresh_high;
    pir_sample.adc_lower_threshold = adc_thresh_low;
    enqueue_sample(&app_queue, pir_sample);
  } // while (data_valid)

  if (lockout_counter > 0) {
    lockout_counter -=
      (num_of_samples < lockout_counter) ? num_of_samples : lockout_counter;
    if (lockout_counter == 0) {
      // Motion off
      pir_instance.motion_detection_callback(false);
    }
  }

  // Motion On
  if (motion) {
    lockout_counter = pir_instance.motion_on_time * 32; // 32Hz
    pir_instance.motion_detection_callback(true);
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  ADC interrupt handler
 ******************************************************************************/
void ADC0_IRQHandler(void)
{
  uint32_t flags;
  pir_sample_t adc_sample;
  static int32_t last_timestamp = -1;

  flags = ADC_IntGetEnabled(ADC0);
  ADC_IntClear(ADC0, flags);
  NVIC_ClearPendingIRQ(ADC0_IRQn);

  // Enqueue the ADC sample
  while (ADC0->SINGLEFIFOCOUNT > 0)
  {
    if ((last_timestamp < 0) && pir_instance.use_timestamp) {
      last_timestamp = ~LETIMER_CounterGet(LETIMER0);
    }
    // adjust this with over-sampling for OSR < 64. Need to left shift to get to
    //   16b.
    adc_sample.adc_sample = ADC_DataSingleGet(ADC0) << 3;
    adc_sample.timestamp_ms = last_timestamp;
    enqueue_sample(&adc_queue, adc_sample);
  }

  pir_instance.adc_irq_callback();
}

/***************************************************************************//**
 * @brief
 *   Reads out a sample from the PIR sample queue.
 *
 * @param[out] pir_sample
 *   Pointer to the PIR sample
 ******************************************************************************/
sl_status_t pir_read_queue(pir_sample_t *pir_sample)
{
  if (app_queue.sample == NULL) {
    return SL_STATUS_FAIL;
  }
  dequeue_sample(&app_queue, pir_sample);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Gets number of samples in the queue.
 *
 * @return
 *   The number of samples in the queue.
 ******************************************************************************/
sl_status_t pir_get_queue_size(uint16_t *size)
{
  if ((app_queue.sample == NULL) || (app_queue.used == 0)) {
    *size = 0;
    return SL_STATUS_FAIL;
  } else {
    *size = app_queue.used;
  }

  return SL_STATUS_OK;
}
