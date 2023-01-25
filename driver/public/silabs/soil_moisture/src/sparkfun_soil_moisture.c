/***************************************************************************//**
 * @file sparkfun_soil_moisture.c
 * @brief Sparkfun Soil moisture sensor
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include "sparkfun_soil_moisture.h"

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

static sl_i2cspm_t *sparkfun_soil_moisture_i2cspm_instance = NULL;
static uint16_t sparkfun_soil_moisture_i2c_addr =
  SPARKFUN_SOIL_MOISTURE_DEFAULT_ADDR;
static bool initialized = false;
static sparkfun_soil_moisture_calibration_t calib = {
  .dry_value = 0,
  .wet_value = 1023,
};

// -----------------------------------------------------------------------------
//                       Local Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 *  Writes block of data to sparkfun soil moisture sensor.
 *****************************************************************************/
static sl_status_t sparkfun_soil_moisture_write_blocking(uint8_t *pdata,
                                                         uint8_t len);

/**************************************************************************//**
 *  Reads block of data from sparkfun soil moisture sensor.
 *****************************************************************************/
static sl_status_t sparkfun_soil_moisture_read_blocking(uint8_t *pdata,
                                                        uint8_t len);

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 *  Initialize the sparkfun soil moisture sensor.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_init(sl_i2cspm_t *i2cspm, uint16_t address)
{
  if (initialized) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }
  if (i2cspm == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  if ((address < 0x07) | (address > 0x78)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  sparkfun_soil_moisture_i2cspm_instance = i2cspm;
  if (!sparkfun_soil_moisture_is_present(address)) {
    return SL_STATUS_INITIALIZATION;
  }
  sparkfun_soil_moisture_i2c_addr = address;
  initialized = true;
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Check if the sparkfun soil moisture sensor is on the I2C bus or not.
 *****************************************************************************/
bool sparkfun_soil_moisture_is_present(uint16_t address)
{
  sl_status_t sc;
  uint16_t backup_addr;

  backup_addr = sparkfun_soil_moisture_i2c_addr;
  sparkfun_soil_moisture_i2c_addr = address;
  sc = sparkfun_soil_moisture_write_blocking(NULL, 0);
  sparkfun_soil_moisture_i2c_addr = backup_addr;
  if (sc == SL_STATUS_OK) {
    return true;
  } else {
    return false;
  }
}

/**************************************************************************//**
 *  Sets I2C address for sparkfun soil moisture sensor.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_set_address(uint16_t address)
{
  uint8_t send_data[2];
  sl_status_t sc;

  if ((address < 0x07) | (address > 0x78)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  send_data[0] = CHANGE_ADDRESS;
  send_data[1] = (uint8_t)address;

  sc = sparkfun_soil_moisture_write_blocking(send_data, 2);
  if (sc == SL_STATUS_OK) {
    sparkfun_soil_moisture_i2c_addr = address;
  }
  return sc;
}

/**************************************************************************//**
 *  Scans I2C address of sparkfun soil moisture sensor.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_scan_address(uint16_t *address,
                                                uint8_t *num_dev)
{
  uint16_t backup_addr;
  sl_status_t sc;
  *num_dev = 0;

  if ((address == NULL) || (num_dev == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  // Back up current address before scanning
  backup_addr = sparkfun_soil_moisture_i2c_addr;

  for (uint16_t addr = 0x08; addr < 0x78; addr++) {
    sparkfun_soil_moisture_i2c_addr = addr;
    sc = sparkfun_soil_moisture_write_blocking(NULL, 0);
    if (sc == SL_STATUS_OK) {
      *(address + *num_dev) = addr;
      (*num_dev)++;
    }
  }

  // Restore the back up address
  sparkfun_soil_moisture_i2c_addr = backup_addr;
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Selects device on the I2C bus.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_select_device(uint16_t address)
{
  if ((address < 0x07) || (address > 0x78)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  sparkfun_soil_moisture_i2c_addr = address;
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Gets current I2C address of sparkfun soil moisture sensor.
 *****************************************************************************/
uint16_t sparkfun_soil_moisture_get_device_address(void)
{
  return sparkfun_soil_moisture_i2c_addr;
}

/**************************************************************************//**
 *  Turns on the LED of sparkfun soil moisture sensor.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_led_on(void)
{
  uint8_t cmd = LED_ON;

  return sparkfun_soil_moisture_write_blocking(&cmd, 1);
}

/**************************************************************************//**
 *  Turns off the LED of sparkfun soil moisture sensor.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_led_off(void)
{
  uint8_t cmd = LED_OFF;

  return sparkfun_soil_moisture_write_blocking(&cmd, 1);
}

/**************************************************************************//**
 *  Sets dry value for calibration.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_set_dry_value(uint16_t dry_value)
{
  // wet value have to be greater than the dry value.
  if ((dry_value > calib.wet_value) || (dry_value > 1023)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  calib.dry_value = dry_value;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets wet value for calibration.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_set_wet_value(uint16_t wet_value)
{
  // wet value have to be greater than the dry value.
  if ((wet_value < calib.dry_value) || (wet_value > 1023)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  calib.wet_value = wet_value;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Gets the range of moisture raw value.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_get_range_values(
  sparkfun_soil_moisture_calibration_t *range_values)
{
  if (range_values == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  range_values->dry_value = calib.dry_value;
  range_values->wet_value = calib.wet_value;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Gets the calibrated moisture value.
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_get_moisture(uint8_t *moisture)
{
  uint16_t value;
  sl_status_t sc;

  if (moisture == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  sc = sparkfun_soil_moisture_get_moisture_raw(&value);
  if (sc != SL_STATUS_OK) {
    return sc;
  }
  if (value < calib.dry_value) {
    *moisture = 0;
  } else {
    *moisture = (uint8_t)((value - calib.dry_value) * 100
                          / (calib.wet_value - calib.dry_value));
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Gets the moisture raw value from the sparkfun soil moisture sensor.
 *****************************************************************************/
sl_status_t sparkfun_soil_moisture_get_moisture_raw(uint16_t *value)
{
  sl_status_t sc;
  uint8_t recv_data[2];

  if (value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  sc = sparkfun_soil_moisture_read_blocking(recv_data, 2);

  if (sc != SL_STATUS_OK) {
    return sc;
  }

  /* The ADC value increases from 0 -> 1023 according to the decreament of
   * the moisture value. */
  *value = 0x3ff - ((uint16_t)(recv_data[1] << 8) + (uint16_t)recv_data[0]);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Writes block of data to sparkfun soil sensor.
 *
 * @details
 *  This function writes block of data to sparkfun soil sensor.
 *
 * @param[in] pdata
 *  Block of data the wants to send to the sensor.
 * @param[in] len
 *  Length of data that sends to the sensor.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
static sl_status_t sparkfun_soil_moisture_write_blocking(uint8_t *pdata,
                                                         uint8_t len)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr = sparkfun_soil_moisture_i2c_addr << 1;
  seq.flags = I2C_FLAG_WRITE;

  seq.buf[0].data = pdata;
  seq.buf[0].len = len;

  ret = I2CSPM_Transfer(sparkfun_soil_moisture_i2cspm_instance, &seq);

  return ret;
}

/***************************************************************************//**
 * @brief
 *  Reads block of data from sparkfun soil sensor.
 *
 * @details
 *  This function writes block of data to sparkfun soil sensor.
 *
 * @param[in] pdata
 *  Block of data the wants to send to the sensor.
 * @param[in] len
 *  Length of data that sends to the sensor.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
static sl_status_t sparkfun_soil_moisture_read_blocking(uint8_t *pdata,
                                                        uint8_t len)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t send_data = GET_VALUE;

  if (pdata == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  seq.addr = sparkfun_soil_moisture_i2c_addr << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  seq.buf[0].data = &send_data;
  seq.buf[0].len = 1;

  seq.buf[1].data = pdata;
  seq.buf[1].len = len;

  ret = I2CSPM_Transfer(sparkfun_soil_moisture_i2cspm_instance, &seq);

  return ret;
}
