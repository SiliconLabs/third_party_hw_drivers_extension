#include "sl_udelay.h"
#include "adafruit_seesaw.h"

sl_status_t adafruit_seesaw_i2c_init(seesaw_t seesaw_dev)
{
  /// Variables and structures for transfer
  uint8_t data = 0;
  I2C_TransferSeq_TypeDef i2cTransfer;
  /// if the parameter is over 7 bit return error code
  if (seesaw_dev.i2c_addr > 0x4D) {
    return SL_STATUS_FAIL;
  }

  /// Initializing I2C transfer It supports 7-bit I2C follower address,
  /// the value of the "address" here should be left shifted by one.
  /// Use flag master write

  i2cTransfer.addr = seesaw_dev.i2c_addr << 1;
  i2cTransfer.flags = I2C_FLAG_WRITE_READ;

  /// Transmit buffer, no data to send
  i2cTransfer.buf[0].data = 0;
  i2cTransfer.buf[0].len = 1;

  /// Receive buffer, two bytes to receive
  i2cTransfer.buf[1].data = &data;
  i2cTransfer.buf[1].len = 1;

  return I2CSPM_Transfer(seesaw_dev.i2c_instance, &i2cTransfer);
}

sl_status_t adafruit_seesaw_i2c_register_read(seesaw_t seesaw_dev,
                                              uint8_t reg_high,
                                              uint8_t reg_low,
                                              uint8_t *pdata,
                                              uint8_t len,
                                              uint16_t delay)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef result;
  uint8_t pos = 0;
  uint8_t i2c_write_data[2];

  while (pos < len) {
    uint8_t read_now;

    if ((pos + 32) < len) {
      read_now = 32;
    } else {
      read_now = len;
    }

    seq.addr = seesaw_dev.i2c_addr << 1;
    seq.flags = I2C_FLAG_WRITE;

    i2c_write_data[0] = reg_high;
    i2c_write_data[1] = reg_low;

    seq.buf[0].data = i2c_write_data;
    seq.buf[0].len = 2;

    result = I2CSPM_Transfer(seesaw_dev.i2c_instance, &seq);
    if (result != i2cTransferDone) {
      return SL_STATUS_TRANSMIT;
    }

    sl_udelay_wait(delay);

    seq.flags = I2C_FLAG_READ;

    seq.buf[0].data = pdata;
    seq.buf[0].len = read_now;

    result = I2CSPM_Transfer(seesaw_dev.i2c_instance, &seq);
    pos += read_now;
    pdata += pos;

    if (result != i2cTransferDone) {
      return SL_STATUS_TRANSMIT;
    }
  }

  return SL_STATUS_OK;
}

sl_status_t adafruit_seesaw_i2c_register_write(seesaw_t seesaw_dev,
                                               uint8_t reg_high,
                                               uint8_t reg_low,
                                               uint8_t *pdata,
                                               uint16_t len)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef result;
  uint8_t i2_reg_addr[2];

  seq.addr = seesaw_dev.i2c_addr << 1;
  seq.flags = I2C_FLAG_WRITE_WRITE;

  i2_reg_addr[0] = reg_high;
  i2_reg_addr[1] = reg_low;

  seq.buf[0].data = i2_reg_addr;
  seq.buf[0].len = 2;

  seq.buf[1].data = pdata;
  seq.buf[1].len = len;

  result = I2CSPM_Transfer(seesaw_dev.i2c_instance, &seq);
  if (result != i2cTransferDone) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

int8_t adafruit_seesaw_get_keypad_count(seesaw_t seesaw_dev)
{
  sl_status_t sc;
  int8_t count = 0;

  sc = adafruit_seesaw_i2c_register_read(seesaw_dev,
                                         SEESAW_KEYPAD_BASE,
                                         SEESAW_KEYPAD_COUNT,
                                         (uint8_t *)&count,
                                         1, 500);

  if (sc != SL_STATUS_OK) {
    return -1;
  }

  return count;
}

/**
 *****************************************************************************
 *  @brief      Read all keyEvents into the passed buffer
 *
 *  @param      buf pointer to where the keyEvents should be stored
 *  @param		count the number of events to read
 *  @returns    True on I2C read success
 *****************************************************************************/
sl_status_t adafruit_seesaw_read_keypad(seesaw_t seesaw_dev,
                                        keyEventRaw *buf,
                                        uint8_t count)
{
  return adafruit_seesaw_i2c_register_read(seesaw_dev,
                                           SEESAW_KEYPAD_BASE,
                                           SEESAW_KEYPAD_FIFO,
                                           (uint8_t *)buf,
                                           count,
                                           1000);
}
