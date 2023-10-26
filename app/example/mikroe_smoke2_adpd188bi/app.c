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
#include "sl_component_catalog.h"

#if defined(SL_CATALOG_SMOKE2_ADPD188BI_I2C_PRESENT)
#include "sl_i2cspm_instances.h"
#include "mikroe_adpd188bi_i2c.h"
#elif defined(SL_CATALOG_SMOKE2_ADPD188BI_SPI_PRESENT)
#include "sl_spidrv_instances.h"
#include "mikroe_adpd188bi_spi.h"
#endif

#define SMOKE_THRESHOLD_VALUE     500

#define EXAMPLE_MODE_SMOKE        0
#define EXAMPLE_MODE_PROXIMITY    1
#define EXAMPLE_MODE              EXAMPLE_MODE_SMOKE

#if (EXAMPLE_MODE == EXAMPLE_MODE_SMOKE)
static void smoke_example (void);

#elif (EXAMPLE_MODE == EXAMPLE_MODE_PROXIMITY)
static void proximity_example (void);

#endif

void app_init(void)
{
  sl_status_t stt;
#if defined(SL_CATALOG_SMOKE2_ADPD188BI_I2C_PRESENT)
  app_log("Mikroe Smoke 2 Click - I2C Interface example\r\n");
  stt = mikroe_adpd188bi_init(sl_i2cspm_mikroe);
#elif defined(SL_CATALOG_SMOKE2_ADPD188BI_SPI_PRESENT)
  app_log("Mikroe Smoke 2 Click - SPI Interface example\r\n");
  stt = mikroe_adpd188bi_init(sl_spidrv_mikroe_handle);
#endif

  app_log("mikroe_adpd188bi_init = 0x%x\r\n", (uint16_t)stt);

  mikroe_adpd188bi_soft_reset();
  app_log("mikroe_adpd188bi_soft_reset\r\n");

  mikroe_adpd188bi_set_mode(MIKROE_ADPD188BI_MODE_IDLE);
  app_log("mikroe_adpd188bi_set_mode MIKROE_ADPD188BI_MODE_IDLE\r\n");

  uint16_t devid = mikroe_adpd188bi_read_data(MIKROE_ADPD188BI_REG_DEVID);

  app_log("Smoke2 Click ID:  0x%.2X\r\n", ( uint16_t ) (devid & 0xFF));
  app_log("Smoke2 Click REV: 0x%.2X\r\n", ( uint16_t ) ((devid >> 8) & 0xFF));
  sl_sleeptimer_delay_millisecond(1000);

  mikroe_adpd188bi_default_cfg();
  app_log("mikroe_adpd188bi_default_cfg\r\n");

#if (EXAMPLE_MODE == EXAMPLE_MODE_SMOKE)
  app_log("SMOKE MODE \r\n");
  app_log("Calibration Start\r\n");
  uint16_t calib_data = mikroe_adpd188bi_smoke_calibration(
    SMOKE_THRESHOLD_VALUE);
  app_log("Calibration data: %u\r\n", calib_data);

#elif (EXAMPLE_MODE == EXAMPLE_MODE_PROXIMITY)
  app_log("PROXIMITY MODE \r\n");
#endif
  app_log("Application Task \r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
#if (EXAMPLE_MODE == EXAMPLE_MODE_SMOKE)
  smoke_example();
#elif (EXAMPLE_MODE == EXAMPLE_MODE_PROXIMITY)
  proximity_example();
#endif
}

#if (EXAMPLE_MODE == EXAMPLE_MODE_SMOKE)
static void smoke_example(void)
{
  static uint8_t last_state = 0;
  uint8_t smoke_detected = mikroe_adpd188bi_check_smoke();

  if (last_state != smoke_detected) {
    if (MIKROE_ADPD188BI_SMOKE_DETECTED == smoke_detected) {
      app_log("SMOKE_DETECTED <<\r\n");
    } else if (MIKROE_ADPD188BI_SMOKE_NOT_DETECTED == smoke_detected) {
      app_log("SMOKE_CLEAR <<\r\n");
    } else {
      app_log("ERROR <<\r\n");
      for ( ; ; ) {}
    }
    last_state = smoke_detected;
  }
}

#elif (EXAMPLE_MODE == EXAMPLE_MODE_PROXIMITY)
static void proximity_example(void)
{
  uint16_t sens_data = 0;
  uint8_t fifo_int = 0;
  uint8_t slot_a_int = 0;
  uint8_t slot_b_int = 0;

  if (0 == mikroe_adpd188bi_get_int_pin()) {
    mikroe_adpd188bi_get_int(&fifo_int, &slot_a_int, &slot_b_int);
    if (0 != slot_a_int) {
      sens_data = mikroe_adpd188bi_read_sens_data(MIKROE_ADPD188BI_SLOT_A,
                                                  MIKROE_ADPD188BI_CHN_1);
      app_log("DATA A: %d\r\n", sens_data);
      slot_a_int = 0;
      sl_sleeptimer_delay_millisecond(100);
    }
    if (0 != slot_b_int) {
      sens_data = mikroe_adpd188bi_read_sens_data(MIKROE_ADPD188BI_SLOT_B,
                                                  MIKROE_ADPD188BI_CHN_1);
      app_log("DATA B: %d\r\n", sens_data);
      slot_b_int = 0;
      sl_sleeptimer_delay_millisecond(100);
    }
    if (0 != fifo_int) {
      app_log("Fifo int: --------------------\r\n");
      for ( uint8_t fifo_cnt = 0; fifo_cnt < fifo_int; fifo_cnt++ )
      {
        sens_data = mikroe_adpd188bi_read_data(
          MIKROE_ADPD188BI_REG_FIFO_ACCESS);
        app_log("FIFO: %d\r\n", sens_data);
      }
      sl_sleeptimer_delay_millisecond(100);
    }
  }
}

#endif
