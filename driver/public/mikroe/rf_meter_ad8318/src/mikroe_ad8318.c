/***************************************************************************//**
 * @file mirkroe_ad8318c
 * @brief SCL ad8318 Source code file.
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "mikroe_ad8318.h"
#include "third_party_hw_drivers_helpers.h"
#include "rfmeter.h"

static rfmeter_t rf_meter;
static rfmeter_cfg_t rf_meter_cfg;

void mikroe_ad8318_cfg_setup(void)
{
  rfmeter_cfg_setup(&rf_meter_cfg);
}

sl_status_t mikroe_ad8318_set_spi_instance(SPIDRV_Handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  rf_meter.spi.handle = spi_instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_ad8318_init(SPIDRV_Handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  THIRD_PARTY_HW_DRV_RETCODE_INIT();

  rfmeter_cfg_setup(&rf_meter_cfg);

  rf_meter.spi.handle = spi_instance;
  rf_meter_cfg.cs = hal_gpio_pin_name(spi_instance->portCs,
                                      spi_instance->pinCs);

  THIRD_PARTY_HW_DRV_RETCODE_TEST(rfmeter_init(&rf_meter, &rf_meter_cfg));

  return THIRD_PARTY_HW_DRV_RETCODE_VALUE;
}

sl_status_t mikroe_ad8318_read_data(uint16_t *read_data)
{
  if (NULL == read_data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *read_data = rfmeter_read_data(&rf_meter);

  return SL_STATUS_OK;
}

sl_status_t mikroe_ad8318_get_raw_data(uint16_t *raw_data)
{
  if (NULL == raw_data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *raw_data = rfmeter_get_raw_data(&rf_meter);

  return SL_STATUS_OK;
}

sl_status_t mikroe_ad8318_get_voltage(float *voltage)
{
  if (NULL == voltage) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *voltage = rfmeter_get_voltage(&rf_meter);

  return SL_STATUS_OK;
}

sl_status_t mikroe_ad8318_get_signal_strength(float slope,
                                              float intercept,
                                              float *signal_strength)
{
  if (NULL == signal_strength) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *signal_strength = rfmeter_get_signal_strenght(&rf_meter, slope, intercept);

  return SL_STATUS_OK;
}
