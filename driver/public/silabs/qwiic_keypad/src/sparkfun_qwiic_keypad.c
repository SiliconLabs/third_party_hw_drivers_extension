/***************************************************************************//**
 * @file sparkfun_qwiic_keypad.c
 * @brief Sparkfun Keypad source file.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include "sparkfun_qwiic_keypad.h"

static sl_i2cspm_t *keypad_i2cspm_instance = NULL;
static uint8_t keypad_i2c_addr = SPARKFUN_KEYPAD_DEFAULT_ADDR;

/***************************************************************************//**
 *  Initialize the keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_init(sl_i2cspm_t *i2c_handle, uint8_t address)
{
#ifdef SAPRKFUN_KEYPAD_INT_PIN_EN

  GPIO_PinModeSet(SPARKFUN_KEYPAD_GPIO_INT_PORT,
                  SPARKFUN_KEYPAD_GPIO_INT_PIN,
                  gpioModeInput,
                  0);
  GPIO_ExtIntConfig(SPARKFUN_KEYPAD_GPIO_INT_PORT,
                    SPARKFUN_KEYPAD_GPIO_INT_PIN,
                    SPARKFUN_KEYPAD_GPIO_INT_PIN,
                    true,
                    false,
                    true);
  GPIOINT_CallbackRegister(SPARKFUN_KEYPAD_GPIO_INT_PIN,
                           app_sparkfun_buttonEvent_callback);
  GPIO_IntEnable(SPARKFUN_KEYPAD_GPIO_INT_PIN);

#endif /* SAPRKFUN_KEYPAD_INT_PIN_EN */

  if (i2c_handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  if ((address < 0x08) || (address > 0x77)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  keypad_i2cspm_instance = i2c_handle;

  if (!sparkfun_keypad_present(address)) {
    // Wait for keypad to become ready
    sl_sleeptimer_delay_millisecond(80);

    if (!sparkfun_keypad_present(address)) {
      return SL_STATUS_INITIALIZATION;
    }
  }

  keypad_i2c_addr = address;
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Check whether a special keypad is present on the I2C bus or not.
 ******************************************************************************/
bool sparkfun_keypad_present(uint8_t device_id)
{
  sl_status_t sc;
  uint8_t backup_addr;

  // Back up the current i2c addr
  backup_addr = keypad_i2c_addr;
  // Use special addr to check
  keypad_i2c_addr = device_id;
  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_ID, &device_id);
  // Restore to the backed up i2c addr
  keypad_i2c_addr = backup_addr;
  if (sc != SL_STATUS_OK) {
    return false;
  }

  return true;
}

/***************************************************************************//**
 *  Sets new I2C address for Sparkfun keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_set_address(uint8_t address)
{
  sl_status_t sc;

  if ((address < 0x08) || (address > 0x77)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sc = sparkfun_keypad_write_register(SPARKFUN_KEYPAD_CHANGE_ADDRESS,
                                      address);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  keypad_i2c_addr = address;
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Gets current I2C address used of keypad.
 ******************************************************************************/
uint8_t sparkfun_keypad_get_address(void)
{
  return keypad_i2c_addr;
}

/***************************************************************************//**
 *  Scans I2C address of Sparkfun keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_scan_address(uint8_t *address, uint8_t *num_dev)
{
  *num_dev = 0;

  if ((address == NULL) || (num_dev == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  for (uint8_t addr = 0x08; addr < 0x78; addr++) {
    if (sparkfun_keypad_present(addr)) {
      *(address + *num_dev) = addr;
      (*num_dev)++;
    }
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Selects device on the I2C bus.
 ******************************************************************************/
sl_status_t sparkfun_keypad_select_device(uint8_t address)
{
  if ((address < 0x08) || (address > 0x77)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  keypad_i2c_addr = address;
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads Firmware Version from the keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_get_firmware_version(frw_rev_t *fwRev)
{
  sl_status_t sc;
  uint8_t major;
  uint8_t minor;

  if (fwRev == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_VERSION1, &major);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_VERSION2, &minor);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  fwRev->major = major;
  fwRev->minor = minor;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads the last button pressed from keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_read_last_button(uint8_t *data)
{
  sl_status_t sc;

  if (data == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_LAST_BUTTON,
                                 data);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads the time elapsed since the last button event.
 ******************************************************************************/
sl_status_t sparkfun_keypad_time_since_last_button_event(uint16_t *time)
{
  sl_status_t sc;

  uint8_t msb;
  uint8_t lsb;

  if (time == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_TIME_MSB,
                                 &msb);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_TIME_LSB,
                                 &lsb);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  *time = ((uint16_t)msb << 8) | lsb;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  "commands" keypad to plug in the next button into the registerMap
 ******************************************************************************/
sl_status_t sparkfun_keypad_update_fifo()
{
  sl_status_t sc;

  sc = sparkfun_keypad_write_register(SPARKFUN_KEYPAD_UPDATE_FIFO, 0x01);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Reads a byte of data to the keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_read_data(uint8_t reg_addr, uint8_t *data)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef result;

  uint8_t i2c_write_data[1];

  seq.addr = keypad_i2c_addr << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  i2c_write_data[0] = reg_addr;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len = 1;

  seq.buf[1].data = data;
  seq.buf[1].len = 1;

  result = I2CSPM_Transfer(keypad_i2cspm_instance, &seq);

  if (result != i2cTransferDone) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Writes a byte of data to the keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_write_register(uint8_t reg_addr, uint8_t data)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef result;
  uint8_t i2c_write_data[2];
  uint8_t i2c_read_data[1];

  seq.addr = keypad_i2c_addr << 1;
  seq.flags = I2C_FLAG_WRITE;

  i2c_write_data[0] = reg_addr;
  i2c_write_data[1] = data;

  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len = 2;

  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len = 0;

  result = I2CSPM_Transfer(keypad_i2cspm_instance, &seq);
  if (result != i2cTransferDone) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}
