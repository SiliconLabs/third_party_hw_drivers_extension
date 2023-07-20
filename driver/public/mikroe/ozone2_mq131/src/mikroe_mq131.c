/***************************************************************************//**
 * @file mikroe_mq131.c
 * @brief Mikroe Ozone 2 Click Driver API
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#include "sl_status.h"
#include "spidrv.h"
#include "ozone2.h"

#include "mikroe_mq131.h"
#include "mikroe_mq131_config.h"

static ozone2_t ctx;
static ozone2_cfg_t cfg;

sl_status_t mikroe_ozone2_init(SPIDRV_Handle_t spidrv)
{
  if (spidrv == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  ozone2_cfg_setup(&cfg);
#if MIKROE_OZONE2_CLICK_ADC_SEL == 0
  cfg.adc_sel = OZONE2_ADC_SEL_AN;
  cfg.vref = 3.3;
#if defined(MIKROE_OZONE2_CLICK_AN_PORT) \
  && defined(MIKROE_OZONE2_CLICK_AN_PIN)
  cfg.an = hal_gpio_pin_name(MIKROE_OZONE2_CLICK_AN_PORT,
                             MIKROE_OZONE2_CLICK_AN_PIN);
#endif
#else
  cfg.adc_sel = OZONE2_ADC_SEL_SPI;
  cfg.miso = hal_gpio_pin_name(spidrv->initData.portRx,
                               spidrv->initData.pinRx);
  cfg.cs = hal_gpio_pin_name(spidrv->portCs,
                             spidrv->pinCs);
  ctx.spi.handle = spidrv;
#endif
  if (OZONE2_OK == ozone2_init(&ctx, &cfg)) {
    return SL_STATUS_OK;
  }
  GPIO_PinModeSet(spidrv->initData.portRx,
                  spidrv->initData.pinRx,
                  gpioModeInputPull,
                  0);
  return SL_STATUS_FAIL;
}

sl_status_t mikroe_ozone2_read_signal_voltage(float *data_out)
{
  if (data_out == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  if (OZONE2_OK == ozone2_read_signal_voltage(&ctx, data_out)) {
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}

sl_status_t mikroe_ozone2_read_measurement(uint16_t *data_out)
{
  if (data_out == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  if (OZONE2_OK == ozone2_read_measurement(&ctx, data_out)) {
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}
