/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 * # EXPERIMENTAL QUALITY
 * This code has not been formally tested and is provided as-is.
 * It is not suitable for production environments.
 * This code will not be maintained.
 ******************************************************************************/
#include "bma400.h"
#include "printf.h"
#include "app_assert.h"

#define MIKROE_BMA400_READ_MODE_INTERRUPT
// #define MIKROE_BMA400_READ_MODE_POLLING

#ifdef MIKROE_BMA400_READ_MODE_POLLING
#include "sl_sleeptimer.h"
#endif

#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT
#include "gpiointerrupt.h"
#include "em_gpio.h"
#endif

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT
#include "sl_spidrv_instances.h"
#include "mikroe_bma400_spi.h"
#include "mikroe_bma400_spi_config.h"
#endif

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT
#include "sl_i2cspm_instances.h"
#include "mikroe_bma400_i2c.h"
#include "mikroe_bma400_i2c_config.h"
#endif

#define READING_INTERVAL_MSEC 200
// Earth's gravity in m/s^2
#define GRAVITY_EARTH         (9.80665f)
// 39.0625us per tick
#define SENSOR_TICK_TO_S      (0.0000390625f)

static struct bma400_dev bma;
static volatile bool enable_reading_data = false;

#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT
static void app_gpio_int_cb(uint8_t intNo);

#endif

#ifdef MIKROE_BMA400_READ_MODE_POLLING
static sl_sleeptimer_timer_handle_t app_periodic_timer;
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

#endif

static void app_process_data(void);
static float lsb_to_ms2(int16_t accel_data, uint8_t g_range, uint8_t bit_width);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  int8_t rslt;
  struct bma400_sensor_conf conf;
  struct bma400_int_enable int_en;

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT
  rslt = bma400_spi_init(sl_spidrv_mikroe_handle, &bma);
  app_assert(rslt == BMA400_OK,
             "[E: 0x%04x] Failed to init BMA400 interface\r\n",
             (int)rslt);
#endif

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT
  rslt = bma400_i2c_init(sl_i2cspm_mikroe, MIKROE_BMA400_ADDR, &bma);
  app_assert(rslt == BMA400_OK,
             "[E: 0x%04x] Failed to init BMA400 interface\r\n",
             (int)rslt);
#endif

  rslt = bma400_soft_reset(&bma);
  app_assert(rslt == BMA400_OK,
             "[E: 0x%04x] Failed to init BMA400 interface\r\n",
             (int)rslt);
  rslt = bma400_init(&bma);
  app_assert(rslt == BMA400_OK,
             "[E: 0x%04x] Failed to init BMA400 interface\r\n",
             (int)rslt);
  // Select the type of configuration to be modified
  conf.type = BMA400_ACCEL;

  // Get the accelerometer configurations which are set in the sensor
  rslt = bma400_get_sensor_conf(&conf, 1, &bma);
  app_assert(rslt == BMA400_OK,
             "[E: 0x%04x] Failed to init BMA400 interface\r\n",
             (int)rslt);

  // Modify the desired configurations as per macros
  // available in bma400_defs.h file
#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT
  conf.param.accel.int_chan = BMA400_INT_CHANNEL_1;
#endif
  conf.param.accel.odr = BMA400_ODR_100HZ;
  conf.param.accel.range = BMA400_RANGE_2G;
  conf.param.accel.data_src = BMA400_DATA_SRC_ACCEL_FILT_1;
  // Set the desired configurations to the sensor
  rslt = bma400_set_sensor_conf(&conf, 1, &bma);
  app_assert(rslt == BMA400_OK,
             "[E: 0x%04x] Failed to init BMA400 interface\r\n",
             (int)rslt);

  rslt = bma400_set_power_mode(BMA400_MODE_NORMAL, &bma);
  app_assert(rslt == BMA400_OK,
             "[E: 0x%04x] Failed to init BMA400 interface\r\n",
             (int)rslt);

  int_en.type = BMA400_DRDY_INT_EN;
  int_en.conf = BMA400_ENABLE;
  rslt = bma400_enable_interrupt(&int_en, 1, &bma);
  app_assert(rslt == BMA400_OK,
             "[E: 0x%04x] Failed to init BMA400 interface\r\n",
             (int)rslt);

  printf("Accel Gravity data in m/s^2\r\n");

#ifdef MIKROE_BMA400_READ_MODE_POLLING
  // Start timer used for periodic indications.
  sl_sleeptimer_start_periodic_timer(&app_periodic_timer,
                                     READING_INTERVAL_MSEC,
                                     app_periodic_timer_cb,
                                     (void *) NULL,
                                     0,
                                     0);
#endif

#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT
  GPIO_PinModeSet(MIKROE_BMA400_INT1_PORT,
                  MIKROE_BMA400_INT1_PIN,
                  gpioModeInputPullFilter,
                  1);
  GPIO_ExtIntConfig(MIKROE_BMA400_INT1_PORT,
                    MIKROE_BMA400_INT1_PIN,
                    MIKROE_BMA400_INT1_PIN,
                    1,
                    0,
                    1);
  GPIOINT_CallbackRegister(MIKROE_BMA400_INT1_PIN, app_gpio_int_cb);
  GPIO_IntEnable(MIKROE_BMA400_INT1_PIN);
#endif
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (enable_reading_data) {
    enable_reading_data = false;
    app_process_data();
  }
}

#ifdef MIKROE_BMA400_READ_MODE_POLLING

/**************************************************************************//**
 * Timer callback
 * Called periodically to reading data from BMA400.
 *****************************************************************************/
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)data;
  (void)timer;

  enable_reading_data = true;
}

#endif

#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT

/**************************************************************************//**
 * GPIO Int callback
 * Called to reading data from BMA400.
 *****************************************************************************/
static void app_gpio_int_cb(uint8_t intNo)
{
  (void) intNo;

  enable_reading_data = true;
}

#endif

static void app_process_data(void)
{
  int8_t rslt;
  float t, x, y, z;
  struct bma400_sensor_data accel;
  uint16_t int_status = 0;

  rslt = bma400_get_interrupt_status(&int_status, &bma);
  if (rslt != BMA400_OK) {
    printf("[E: 0x%04x] Failed to get interrupt status\r\n", (int)rslt);
    return;
  }

  if (int_status & BMA400_ASSERTED_DRDY_INT) {
    rslt = bma400_get_accel_data(BMA400_DATA_SENSOR_TIME, &accel, &bma);
    if (rslt != BMA400_OK) {
      printf("[E: 0x%04x] Failed to get accel data\r\n", (int)rslt);
      return;
    }

    /* 12-bit accelerometer at range 2G */
    x = lsb_to_ms2(accel.x, 2, 12);
    y = lsb_to_ms2(accel.y, 2, 12);
    z = lsb_to_ms2(accel.z, 2, 12);
    t = (float)accel.sensortime * SENSOR_TICK_TO_S;
    printf(
      "Gravity-x : %.2f,   Gravity-y : %.2f,  Gravity-z :  %.2f,   t(s) : %.4f\r\n",
      x,
      y,
      z,
      t);
  }
}

static float lsb_to_ms2(int16_t accel_data, uint8_t g_range, uint8_t bit_width)
{
  float accel_ms2;
  int16_t half_scale;

  half_scale = 1 << (bit_width - 1);
  accel_ms2 = (GRAVITY_EARTH * accel_data * g_range) / half_scale;

  return accel_ms2;
}
