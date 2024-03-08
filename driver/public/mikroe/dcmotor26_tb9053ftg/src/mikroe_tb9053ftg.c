/***************************************************************************//**
 * @file mikroe_tb9053ftg.c
 * @brief SCL TB9053FTG Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/

#include "sl_i2cspm.h"
#include "mikroe_tb9053ftg.h"
#include "mikroe_dcmotor26_config.h"
#include "dcmotor26.h"
#include "third_party_hw_drivers_helpers.h"

static dcmotor26_t ctx;
static dcmotor26_cfg_t ctx_cfg;

void mikroe_tb9053ftg_cfg_setup(void)
{
  dcmotor26_cfg_setup(&ctx_cfg);
}

sl_status_t mikroe_tb9053ftg_init(SPIDRV_Handle_t spi_instance,
                                  sl_i2cspm_t *i2c_instance,
                                  adc_t *adc)
{
  if ((NULL == spi_instance)
      || (NULL == i2c_instance) || (NULL == adc)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  THIRD_PARTY_HW_DRV_RETCODE_INIT();

  mikroe_tb9053ftg_cfg_setup();

  ctx.i2c.handle = i2c_instance;
  ctx.spi.handle = spi_instance;
  ctx.slave_address = DCMOTOR26_PCA9538A_ADDRESS;
  ctx.adc.handle = adc;

#if defined(DCMOTOR26_CHANNEL_MONITOR_PORT) \
  && defined(DCMOTOR26_CHANNEL_MONITOR_PIN)
  ctx_cfg.cm = hal_gpio_pin_name(DCMOTOR26_CHANNEL_MONITOR_PORT,
                                 DCMOTOR26_CHANNEL_MONITOR_PIN);
#endif

#if defined(DCMOTOR26_SLEEP_PORT) && defined(DCMOTOR26_SLEEP_PIN)
  ctx_cfg.slp = hal_gpio_pin_name(DCMOTOR26_SLEEP_PORT,
                                  DCMOTOR26_SLEEP_PIN);
#endif

#if defined(DCMOTOR26_INT_PORT) && defined(DCMOTOR26_INT_PIN)
  ctx_cfg.int_pin = hal_gpio_pin_name(DCMOTOR26_INT_PORT,
                                      DCMOTOR26_INT_PIN);
#endif

#if defined(DCMOTOR26_PWM_PORT) && defined(DCMOTOR26_PWM_PIN)
  ctx_cfg.clk = hal_gpio_pin_name(DCMOTOR26_PWM_PORT,
                                  DCMOTOR26_PWM_PIN);
#endif

  THIRD_PARTY_HW_DRV_RETCODE_TEST(dcmotor26_init(&ctx, &ctx_cfg));

  return THIRD_PARTY_HW_DRV_RETCODE_VALUE;
}

sl_status_t mikroe_tb9053ftg_default_cfg(void)
{
  return (dcmotor26_default_cfg(&ctx)
          == DCMOTOR26_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_write_register(uint8_t reg, uint32_t data_in)
{
  return (dcmotor26_pca9538a_write_register(&ctx, reg, data_in) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_read_register(uint8_t reg, uint8_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_pca9538a_read_register(&ctx, reg, data_out) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_pca9538a_write_register(uint8_t reg, uint8_t data_in)
{
  return (dcmotor26_pca9538a_write_register(&ctx, reg, data_in) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_pca9538a_read_register(uint8_t reg, uint8_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_pca9538a_read_register(&ctx, reg, data_out) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

void mikroe_tb9053ftg_enter_sleep_mode(void)
{
  dcmotor26_enter_sleep_mode(&ctx);
}

void mikroe_tb9053ftg_wake_up_device(void)
{
  dcmotor26_wake_up_device(&ctx);
}

uint8_t mikroe_tb9053ftg_get_int_pin(void)
{
  return dcmotor26_get_int_pin(&ctx);
}

void mikroe_tb9053ftg_set_clk_pin(uint8_t state)
{
  dcmotor26_set_clk_pin(&ctx, state);
}

sl_status_t mikroe_tb9053ftg_set_en1_pin(uint8_t state)
{
  return (dcmotor26_set_en1_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_en1_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_en1_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_en2_pin(uint8_t state)
{
  return (dcmotor26_set_en2_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_en2_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_en2_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_cm_sel_pin(uint8_t state)
{
  return (dcmotor26_set_cm_sel_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_cm_sel_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_cm_sel_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_diag1_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_diag1_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_diag2_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_diag2_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_sleep_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_sleep_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_read_an_pin_value(uint16_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_read_an_pin_value(&ctx, data_out) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_read_an_pin_voltage(float *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_read_an_pin_voltage(&ctx, data_out) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_motor_current(float *current)
{
  if (NULL == current) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_motor_current(&ctx, current) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch1_operation_mode(uint8_t mode)
{
  return (dcmotor26_set_ch1_operation_mode(&ctx, mode) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch2_operation_mode(uint8_t mode)
{
  return (dcmotor26_set_ch2_operation_mode(&ctx, mode) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch1_drive_frequency(uint16_t drv_freq)
{
  return (dcmotor26_set_ch1_drive_frequency(&ctx, drv_freq) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch2_drive_frequency(uint16_t drv_freq)
{
  return (dcmotor26_set_ch2_drive_frequency(&ctx, drv_freq) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch1_duty_period(uint16_t duty_period)
{
  return (dcmotor26_set_ch1_duty_period(&ctx, duty_period) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch2_duty_period(uint16_t duty_period)
{
  return (dcmotor26_set_ch2_duty_period(&ctx, duty_period) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}
