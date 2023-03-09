/***************************************************************************//**
 * @file mikroe_mm5d91_00.c
 * @brief mikroSDK 2.0 Radar Click driver public API
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "mikroe_mm5d91_00.h"
#include "mikroe_mm5d91_00_config.h"

sl_status_t mikroe_radar_usart_init(mikroe_radar_t *ctx,
                                    sl_iostream_uart_t *stream,
                                    uint32_t baud_rate)
{
  radar_cfg_t cfg;
  err_t retval;

  if (!ctx || !stream) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  radar_cfg_setup(&cfg);
  cfg.rst = hal_gpio_pin_name(CONFIG_MM5D91_00_RST_PORT,
                              CONFIG_MM5D91_00_RST_PIN);
#if defined(CONFIG_MM5D91_00_GPIO0_PORT) && defined(CONFIG_MM5D91_00_GPIO0_PIN)
  cfg.gp0 = hal_gpio_pin_name(CONFIG_MM5D91_00_GPIO0_PORT,
                              CONFIG_MM5D91_00_GPIO0_PIN);
#endif
#if defined(CONFIG_MM5D91_00_GPIO1_PORT) && defined(CONFIG_MM5D91_00_GPIO1_PIN)
  cfg.gp1 = hal_gpio_pin_name(CONFIG_MM5D91_00_GPIO1_PORT,
                              CONFIG_MM5D91_00_GPIO1_PIN);
#endif
#if defined(CONFIG_MM5D91_00_GPIO2_PORT) && defined(CONFIG_MM5D91_00_GPIO2_PIN)
  cfg.gp2 = hal_gpio_pin_name(CONFIG_MM5D91_00_GPIO2_PORT,
                              CONFIG_MM5D91_00_GPIO2_PIN);
#endif
  if (baud_rate > 0) {
    cfg.baud_rate = baud_rate;
  }

  ctx->uart.handle = stream;
  retval = radar_init(ctx, &cfg);
  if (UART_SUCCESS == retval) {
    retval = radar_default_cfg(ctx);
  }
  return RADAR_OK == retval ? SL_STATUS_OK : SL_STATUS_FAIL;
}

int32_t mikroe_radar_generic_write(mikroe_radar_t *ctx,
                                   uint8_t *data_buf,
                                   uint16_t len)
{
  if (!ctx || !data_buf || (len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (int32_t)radar_generic_write(ctx, data_buf, len);
}

int32_t mikroe_radar_generic_read(mikroe_radar_t *ctx,
                                  uint8_t *data_buf,
                                  uint16_t max_len)
{
  if (!ctx || !data_buf || (max_len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (int32_t)radar_generic_read(ctx, data_buf, max_len);
}

sl_status_t mikroe_radar_enable_device(mikroe_radar_t *ctx)
{
  if (!ctx) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  radar_enable_device(ctx);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_disable_device(mikroe_radar_t *ctx)
{
  if (!ctx) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  radar_disable_device(ctx);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_get_gpio2_pin(mikroe_radar_t *ctx, uint8_t *gpio_val)
{
  if (!ctx || !gpio_val) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *gpio_val = radar_get_gpio2_pin(ctx);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_get_gpio1_pin(mikroe_radar_t *ctx, uint8_t *gpio_val)
{
  if (!ctx || !gpio_val) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *gpio_val = radar_get_gpio1_pin(ctx);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_get_gpio0_pin(mikroe_radar_t *ctx, uint8_t *gpio_val)
{
  if (!ctx || !gpio_val) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *gpio_val = radar_get_gpio0_pin(ctx);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_set_command(mikroe_radar_t *ctx,
                                     uint8_t cmd_id,
                                     uint8_t *payload,
                                     uint8_t payload_size)
{
  if (!ctx) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return RADAR_OK == radar_set_command(ctx, cmd_id, payload, payload_size) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_get_command(mikroe_radar_t *ctx,
                                     uint8_t cmd_id,
                                     uint8_t *payload,
                                     uint8_t *payload_size)
{
  if (!ctx) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return RADAR_OK
         == radar_get_command(ctx, cmd_id, payload,
                              payload_size) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_get_event(mikroe_radar_t *ctx,
                                   uint8_t *evt_id,
                                   uint8_t *payload,
                                   uint8_t *payload_size)
{
  if (!ctx) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return RADAR_OK
         == radar_get_event(ctx, evt_id, payload,
                            payload_size) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_get_temperature(mikroe_radar_t *ctx,
                                         float *temperature)
{
  if (!ctx || !temperature) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return RADAR_OK == radar_get_temperature(ctx, temperature) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_set_detection_range(mikroe_radar_t *ctx,
                                             float min,
                                             float max)
{
  if (!ctx) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return RADAR_OK == radar_set_detection_range(ctx, min, max) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_reset_config(mikroe_radar_t *ctx)
{
  if (!ctx) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return RADAR_OK == radar_reset_config(ctx) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_float_ieee_to_mchip(float *f_data)
{
  if (!f_data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  radar_float_ieee_to_mchip(f_data);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_float_mchip_to_ieee(float *f_data)
{
  if (!f_data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  radar_float_mchip_to_ieee(f_data);
  return SL_STATUS_OK;
}
