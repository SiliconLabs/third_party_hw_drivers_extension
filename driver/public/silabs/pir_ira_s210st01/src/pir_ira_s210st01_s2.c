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
#include "em_iadc.h"
#include "em_letimer.h"

#include <pir_ira_s210st01.h>
#include <pir_ira_s210st01_config.h>
#include <stdbool.h>
#include <stdio.h>

#define ADC_PERIPHERAL           IADC0
#define ADC_CLOCK                cmuClock_IADC0

// Set CLK_ADC to 10 MHz
#define CLK_SRC_ADC_FREQ         5000000  // CLK_SRC_ADC
#define CLK_ADC_FREQ             1000000  // CLK_ADC - 10 MHz max in normal mode

// Desired Sampling frequency in Hz
#define SAMPLING_FREQ            32
#define ADC_SAMPLING_FREQ        96

// Set IADC timer cycles
#define ADC_TIMER_CYCLES         (CLK_SRC_ADC_FREQ / ADC_SAMPLING_FREQ)

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
#error "The posInput of the ADC is not defined for the kit, only the negInput"
#endif

#define adc_mux_even(ref, adc, bus, index) \
  GPIO->bus ## BUSALLOC |=                 \
    GPIO_ ## bus ## BUSALLOC_ ## bus ## EVEN ## index ## _ ## adc;
#define adc_mux_odd(ref, adc, bus, index) \
  GPIO->bus ## BUSALLOC |=                \
    GPIO_ ## bus ## BUSALLOC_ ## bus ## ODD ## index ## _ ## adc;

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

static int32_t pos_thresh = 0;
static int32_t neg_thresh = 0;

static sample_queue_t app_queue;   // Buffer PIR samples for application layer
                                   //   debugging.
static sample_queue_t adc_queue;   // Buffer ADC samples for motion detection
                                   //   algorithm.
static pir_sample_t adc_sample_queue[ADC_SAMPLE_QUEUE_SIZE]; // ADC sample
                                                             //   buffer.

#if PIR_SOURCE == PIR_SOURCE_DIFF || PIR_SOURCE == PIR_SOURCE_POS
static sl_status_t allocate_analog_bus_even0(IADC_TypeDef *iadc,
                                             GPIO_Port_TypeDef port)
{
  switch (port) {
    case gpioPortA:
#if (IADC_COUNT == 1)
      if (iadc == IADC0) {
        adc_mux_even(iadc, ADC0, A, 0);
        return SL_STATUS_OK;
#elif (IADC_COUNT == 2)
  } else if (iadc == IADC1) {
    adc_mux_even(iadc, ADC1, A, 0);
    return SL_STATUS_OK;
#endif
      } else {
        return SL_STATUS_FAIL;
      }

    case gpioPortB:
#if (IADC_COUNT == 1)
      if (iadc == IADC0) {
        adc_mux_even(iadc, ADC0, B, 0);
        return SL_STATUS_OK;
#elif (IADC_COUNT == 2)
  } else if (iadc == IADC1) {
    adc_mux_even(iadc, ADC1, B, 0);
    return SL_STATUS_OK;
#endif
      } else {
        return SL_STATUS_FAIL;
      }

    case gpioPortC:
    case gpioPortD:
#if (IADC_COUNT == 1)
      if (iadc == IADC0) {
        adc_mux_even(iadc, ADC0, CD, 0);
        return SL_STATUS_OK;
#elif (IADC_COUNT == 2)
  } else if (iadc == IADC1) {
    adc_mux_even(iadc, ADC1, CD, 0);
    return SL_STATUS_OK;
#endif
      } else {
        return SL_STATUS_FAIL;
      }

    default:
      return SL_STATUS_FAIL;
  }
}

#endif

#if PIR_SOURCE == PIR_SOURCE_DIFF || PIR_SOURCE == PIR_SOURCE_NEG
static sl_status_t allocate_analog_bus_odd0(IADC_TypeDef *iadc,
                                            GPIO_Port_TypeDef port)
{
  switch (port) {
    case gpioPortA:
#if (IADC_COUNT == 1)
      if (iadc == IADC0) {
        adc_mux_odd(iadc, ADC0, A, 0);
        return SL_STATUS_OK;
#elif (IADC_COUNT == 2)
  } else if (iadc == IADC1) {
    adc_mux_odd(iadc, ADC1, A, 0);
    return SL_STATUS_OK;
#endif
      } else {
        return SL_STATUS_FAIL;
      }

    case gpioPortB:
#if (IADC_COUNT == 1)
      if (iadc == IADC0) {
        adc_mux_odd(iadc, ADC0, B, 0);
        return SL_STATUS_OK;
#elif (IADC_COUNT == 2)
  } else if (iadc == IADC1) {
    adc_mux_odd(iadc, ADC1, B, 0);
    return SL_STATUS_OK;
#endif
      } else {
        return SL_STATUS_FAIL;
      }

    case gpioPortC:
    case gpioPortD:
#if (IADC_COUNT == 1)
      if (iadc == IADC0) {
        adc_mux_odd(iadc, ADC0, CD, 0);
        return SL_STATUS_OK;
#elif (IADC_COUNT == 2)
  } else if (iadc == IADC1) {
    adc_mux_odd(iadc, ADC1, CD, 0);
    return SL_STATUS_OK;
#endif
      } else {
        return SL_STATUS_FAIL;
      }

    default:
      return SL_STATUS_FAIL;
  }
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
  pos_thresh = win_base + win_size / 2;
  neg_thresh = win_base - win_size / 2;

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
  // Declare initialization structures
  IADC_Init_t adcInit = IADC_INIT_DEFAULT;
  IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
  IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;
  // Single input structure
  IADC_SingleInput_t adcInitSingle = IADC_SINGLEINPUT_DEFAULT;

  CMU_ClockEnable(ADC_CLOCK, true);

  // Reset IADC to reset configuration in case it has been modified
  IADC_reset(ADC_PERIPHERAL);

  // Use the FSRC0 as the IADC clock so it can run in EM2
  CMU_ClockSelectSet(ADC_CLOCK, cmuSelect_FSRCO);

  // Set the prescaler needed for the intended IADC clock frequency
  adcInit.srcClkPrescale = IADC_calcSrcClkPrescale(ADC_PERIPHERAL,
                                                   CLK_SRC_ADC_FREQ,
                                                   0);

  // Shutdown between conversions to reduce current
  adcInit.warmup = iadcWarmupNormal;

  // Set timer cycles to configure sampling rate
#if ADC_TIMER_CYCLES > 65535
  adcInit.timerCycles = 65535;
#else
  adcInit.timerCycles = ADC_TIMER_CYCLES;
#endif

  /*
   * Configuration 0 is used by both scan and single conversions by
   * default.  Use internal bandgap as the reference and specify the
   * reference voltage in mV.
   *
   * Resolution is not configurable directly but is based on the
   * selected oversampling ratio (osrHighSpeed), which defaults to
   * 2x and generates 12-bit results.
   */
  initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;
  initAllConfigs.configs[0].vRef = 3300;
  initAllConfigs.configs[0].osrHighSpeed = iadcCfgOsrHighSpeed2x;
  initAllConfigs.configs[0].analogGain = iadcCfgAnalogGain1x;

  /*
   * CLK_SRC_ADC is prescaled to derive the intended CLK_ADC frequency.
   *
   * Based on the default 2x oversampling rate (OSRHS)...
   *
   * conversion time = ((4 * OSRHS) + 2) / fCLK_ADC
   *
   * ...which, results in a maximum sampling rate of 833 ksps with the
   * 2-clock input multiplexer switching time is included.
   */
  initAllConfigs.configs[0].adcClkPrescale = IADC_calcAdcClkPrescale(
    ADC_PERIPHERAL,
    CLK_ADC_FREQ,
    0,
    iadcCfgModeNormal,
    adcInit.srcClkPrescale);
#if PIR_SOURCE == PIR_SOURCE_POS
  adcInitSingle.posInput = IADC_portPinToPosInput(PIR_IRA_S210ST01_ADC_P_PORT,
                                                  PIR_IRA_S210ST01_ADC_P_PIN);
  adcInitSingle.negInput = iadcNegInputGnd;

  // Allocate the analog bus for ADC inputs
  if (SL_STATUS_OK != allocate_analog_bus_even0(ADC_PERIPHERAL,
                                                PIR_IRA_S210ST01_ADC_P_PORT)) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }
#elif PIR_SOURCE == PIR_SOURCE_NEG
  // The posSel of the ADC_N is not defined for the kit, only the negSel.
  adcInitSingle.posInput = iadcPosInputAvdd;
  adcInitSingle.negInput = IADC_portPinToNegInput(PIR_IRA_S210ST01_ADC_N_PORT,
                                                  PIR_IRA_S210ST01_ADC_N_PIN);

  // Allocate the analog bus for ADC inputs
  if (SL_STATUS_OK != allocate_analog_bus_odd0(ADC_PERIPHERAL,
                                               PIR_IRA_S210ST01_ADC_N_PORT)) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }
#elif PIR_SOURCE == PIR_SOURCE_DIFF
  adcInitSingle.posInput = IADC_portPinToPosInput(PIR_IRA_S210ST01_ADC_P_PORT,
                                                  PIR_IRA_S210ST01_ADC_P_PIN);
  adcInitSingle.negInput = IADC_portPinToNegInput(PIR_IRA_S210ST01_ADC_N_PORT,
                                                  PIR_IRA_S210ST01_ADC_N_PIN);

  // Allocate the analog bus for ADC inputs
  if (SL_STATUS_OK != allocate_analog_bus_even0(ADC_PERIPHERAL,
                                                PIR_IRA_S210ST01_ADC_P_PORT)) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }
  if (SL_STATUS_OK != allocate_analog_bus_odd0(ADC_PERIPHERAL,
                                               PIR_IRA_S210ST01_ADC_N_PORT)) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }
#endif

  // Single initialization
  initSingle.triggerSelect = iadcTriggerSelTimer;

  // Enable triggering of single conversion
  initSingle.start = true;

  // Initialize IADC
  IADC_init(ADC_PERIPHERAL, &adcInit, &initAllConfigs);

  // Initialize a single-channel conversion
  IADC_initSingle(ADC_PERIPHERAL, &initSingle, &adcInitSingle);

  // Clear any previous interrupt flags
  IADC_clearInt(ADC_PERIPHERAL, _IADC_IF_MASK);

  // Enable single-channel done interrupts
  IADC_enableInt(ADC_PERIPHERAL, IADC_IEN_SINGLEDONE);

  // Enable IADC interrupts
  NVIC_ClearPendingIRQ(IADC_IRQn);
  NVIC_EnableIRQ(IADC_IRQn);

  update_adc_thresholds(win_base, pir_instance.win_size);

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
#if defined(PIR_IRA_S210ST01_ADC_P_PORT) && \
  defined(PIR_IRA_S210ST01_ADC_P_PIN)
  // ADC_P
  GPIO_PinModeSet(PIR_IRA_S210ST01_ADC_P_PORT,
                  PIR_IRA_S210ST01_ADC_P_PIN,
                  gpioModeDisabled,
                  0);
#endif
#if defined(PIR_IRA_S210ST01_ADC_N_PORT) && \
  defined(PIR_IRA_S210ST01_ADC_N_PIN)
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

/***************************************************************************//**
 * @brief
 *  Initialize timers for time stamping samples to the GUI.
 *
 * @details
 *  LETIMER0 will operate at 1024 Hz time stamp clock.
 ******************************************************************************/
static void init_timestamp_clock(void)
{
  CMU_LFXOInit_TypeDef lfxoInit = CMU_LFXOINIT_DEFAULT;
  LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;

  // Initialize the LFXO and use it as the EM23GRPACLK source
  CMU_LFXOInit(&lfxoInit);
  CMU_ClockSelectSet(cmuClock_EM23GRPACLK, cmuSelect_LFXO);
  CMU_ClockEnable(cmuClock_LETIMER0, true);

  LETIMER_Init(LETIMER0, &letimerInit);

//  // Clear any previous interrupt flags
//  LETIMER_IntClear(LETIMER0, _LETIMER_IF_MASK);
//
//  // Enable underflow interrupts
//  LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF);
//
//  // Enable LETIMER interrupts
//  NVIC_ClearPendingIRQ(LETIMER0_IRQn);
//  NVIC_EnableIRQ(LETIMER0_IRQn);
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
    return SL_STATUS_NOT_SUPPORTED;
  }

  if (pir_init->use_timestamp) {
    init_timestamp_clock();
  }

  if (SL_STATUS_OK != init_adc(adc_enter_em2)) {
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

  // Clear any previous interrupt flags
  IADC_clearInt(ADC_PERIPHERAL, _IADC_IF_MASK);

  // Enable single-channel done interrupts
  IADC_enableInt(ADC_PERIPHERAL, IADC_IEN_SINGLEDONE);

  // Enable IADC interrupts
  NVIC_ClearPendingIRQ(IADC_IRQn);
  NVIC_EnableIRQ(IADC_IRQn);

  // IADC single already enabled; must enable timer block in order to trigger
  IADC_command(ADC_PERIPHERAL, iadcCmdEnableTimer);

//  // Enable underflow interrupts
//  LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF);
//
//  // Enable LETIMER interrupts
//  NVIC_ClearPendingIRQ(LETIMER0_IRQn);
//  NVIC_EnableIRQ(LETIMER0_IRQn);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Stops PIR sensor sampling.
 ******************************************************************************/
sl_status_t pir_stop(void)
{
  // Disable single-channel done interrupts
  IADC_disableInt(ADC_PERIPHERAL, IADC_IEN_SINGLEDONE);

  NVIC_DisableIRQ(IADC_IRQn);

  // Stop an IADC conversion
  IADC_command(ADC_PERIPHERAL, iadcCmdDisableTimer);

//  // Disable underflow interrupts
//  LETIMER_IntDisable(LETIMER0, LETIMER_IEN_UF);
//
//  // Disable LETIMER interrupts
//  NVIC_DisableIRQ(LETIMER0_IRQn);

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

  // Read out each ADC sample, update the LPF mean, and check if it was within
  //   the window thresholds
  while (adc_queue.used > 0) {
    num_of_samples++;
    dequeue_sample(&adc_queue, &adc_sample);
    // Capture the ADC and winBase of the current sample.
    pir_sample.adc_sample = adc_sample.adc_sample;
    pir_sample.win_base = win_base;

    // If window was broken, move window thresholds to include the latest ADC
    //   reading.
    // Thresholds are recalculated in software because samples are batch
    //   processed.
    if (adc_sample.adc_sample > pos_thresh) {
      win_base = adc_sample.adc_sample - pir_instance.win_size / 2;
      motion = true;
    } else if (adc_sample.adc_sample < neg_thresh) {
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
    pir_sample.adc_upper_threshold = pos_thresh;
    pir_sample.adc_lower_threshold = neg_thresh;
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
void IADC_IRQHandler(void)
{
  pir_sample_t adc_sample;
  static int32_t last_timestamp = -1;
  static uint8_t irq_count = 0;

  IADC_clearInt(ADC_PERIPHERAL, IADC_IF_SINGLEDONE);
  NVIC_ClearPendingIRQ(IADC_IRQn);

  //  IADC_Result_t sample = IADC_pullSingleFifoResult(ADC_PERIPHERAL);
  IADC_Result_t sample = IADC_readSingleResult(ADC_PERIPHERAL);
  if ((++irq_count % (ADC_SAMPLING_FREQ / SAMPLING_FREQ)) == 0) {
    // Enqueue the ADC sample
    if ((last_timestamp < 0) && pir_instance.use_timestamp) {
      last_timestamp = ~LETIMER_CounterGet(LETIMER0);
    }

    // adjust this with over-sampling for OSR < 64. Need to left shift to get to
    //   16b.
    adc_sample.adc_sample = sample.data << 3;
    adc_sample.timestamp_ms = last_timestamp;
    enqueue_sample(&adc_queue, adc_sample);

    pir_instance.adc_irq_callback();
    irq_count = 0;
  }
}

/**************************************************************************//**
 * @brief  LETIMER IRQ Handler
 *****************************************************************************/
void LETIMER0_IRQHandler(void)
{
  uint32_t flags = LETIMER_IntGet(LETIMER0);

  // Trigger an IADC scan conversion
//  IADC_command(ADC_PERIPHERAL, iadcCmdStartSingle);

  // Clear LETIMER interrupt flags
  LETIMER_IntClear(LETIMER0, flags);
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
