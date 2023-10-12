/***************************************************************************//**
 * @file mikroe_thunder.c
 * @brief SCL Thunder click Source File
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

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------
#include "mikroe_thunder_as3935.h"
#include "third_party_hw_drivers_helpers.h"

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
static thunder_t  thunder_ctx;
static thunder_cfg_t  thunder_cfg;
static bool initialized = false;

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/**************************************************************************//**
*  Thunder click initialization.
******************************************************************************/
sl_status_t mikroe_thunder_as3935_init(SPIDRV_Handle_t spi_instance)
{
  THIRD_PARTY_HW_DRV_RETCODE_INIT();

  if (spi_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (initialized) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  thunder_ctx.spi.handle = spi_instance;
  thunder_cfg_setup(&thunder_cfg);

  thunder_cfg.irq = hal_gpio_pin_name(THUNDER_IRQ_PORT, THUNDER_IRQ_PIN);
  thunder_cfg.cs = hal_gpio_pin_name(spi_instance->portCs, spi_instance->pinCs);
  GPIO_PinModeSet(spi_instance->portCs,
                  spi_instance->pinCs,
                  gpioModePushPull,
                  1);

  THIRD_PARTY_HW_DRV_RETCODE_TEST(thunder_init(&thunder_ctx, &thunder_cfg));
  initialized = true;

  return THIRD_PARTY_HW_DRV_RETCODE_VALUE;
}

/**************************************************************************//**
*  Thunder click default configuration.
******************************************************************************/
sl_status_t mikroe_thunder_as3935_default_cfg(void)
{
  thunder_default_cfg(&thunder_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  Thunder click write reg function.
******************************************************************************/
sl_status_t mikroe_thunder_as3935_write_reg(uint8_t reg, uint8_t data_in)
{
  return thunder_write_reg(&thunder_ctx, reg, data_in);
}

/**************************************************************************//**
*  Thunder click read reg function.
******************************************************************************/
sl_status_t mikroe_thunder_as3935_read_reg(uint8_t reg, uint8_t *data_out)
{
  return thunder_read_reg(&thunder_ctx, reg, data_out);
}

/**************************************************************************//**
*  Thunder click command Send function.
******************************************************************************/
sl_status_t mikroe_thunder_as3935_send_command(uint8_t command)
{
  thunder_send_command(&thunder_ctx, command);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  Thunder click interrupt Check function.
******************************************************************************/
uint8_t mikroe_thunder_as3935_check_int(void)
{
  return thunder_check_int(&thunder_ctx);
}

/**************************************************************************//**
*  Thunder click storm Information Get function.
******************************************************************************/
sl_status_t mikroe_thunder_as3935_get_storm_info(uint32_t *energy_out,
                                                 uint8_t *distance_out)
{
  thunder_get_storm_info(&thunder_ctx, energy_out, distance_out);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  Thunder click IRQ pin Check function.
******************************************************************************/
uint8_t mikroe_thunder_as3935_check_irq_pin(void)
{
  return thunder_check_irq_pin(&thunder_ctx);
}
