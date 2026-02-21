/*
 * lm75_driver.c
 *
 *  Created on: Feb 20, 2026
 *      Author: nalethak
 */

#include "tempsensor_lm75a.h"

/*******************************************************************************
 * Common LM75 Defines
 ******************************************************************************/

#define LM75_I2C_ADDR   0x48
#define LM75_TEMP_REG   0x00


/*******************************************************************************
 * Platform Specific Includes
 ******************************************************************************/

#ifdef PLATFORM_SI917X

#include "sl_si91x_i2c.h"
#include "sl_i2c_instances.h"
#include "rsi_debug.h"


#define INSTANCE_ZERO 0
#define INSTANCE_ONE  1
#define INSTANCE_TWO  2

#define I2C_INSTANCE_USED  INSTANCE_TWO

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
static sl_i2c_config_t   i2c_config;

#elif defined(PLATFORM_SERIES2)

#include "sl_i2cspm.h"
#include "sl_i2cspm_instances.h"

static sl_i2cspm_t *lm75_i2c = NULL;

#else
#error "Define PLATFORM_SI917X or PLATFORM_SERIES2"
#endif


/*******************************************************************************
 * LM75 INIT
 ******************************************************************************/
sl_status_t lm75_init(void)
{
#ifdef PLATFORM_SI917X

#if (I2C_INSTANCE_USED == 0)
  i2c_config = sl_i2c_i2c0_config;
#elif (I2C_INSTANCE_USED == 1)
  i2c_config = sl_i2c_i2c1_config;
#else
  i2c_config = sl_i2c_i2c2_config;
#endif

  sl_i2c_status_t status =
      sl_i2c_driver_init(i2c_instance, &i2c_config);

  if (status != SL_I2C_SUCCESS) {
    return SL_STATUS_FAIL;
  }

  sl_i2c_driver_configure_fifo_threshold(i2c_instance, 0, 0);
  return SL_STATUS_OK;

#elif defined(PLATFORM_SERIES2)

  lm75_i2c = sl_i2cspm_qwiic;

  return SL_STATUS_OK;

#endif
}


/*******************************************************************************
 * Read Temperature
 ******************************************************************************/
sl_status_t lm75_get_temperature(float *temp_c)
{

#ifdef PLATFORM_SI917X

  uint8_t reg = LM75_TEMP_REG;
  uint8_t data[2] = {0};

  if (sl_i2c_driver_send_data_blocking(i2c_instance,
                                       LM75_I2C_ADDR,
                                       &reg,
                                       1) != SL_I2C_SUCCESS) {
    return SL_STATUS_TRANSMIT;
  }

  if (sl_i2c_driver_receive_data_blocking(i2c_instance,
                                          LM75_I2C_ADDR,
                                          data,
                                          2) != SL_I2C_SUCCESS) {
    return SL_STATUS_RECEIVE;
  }

  int16_t raw = (data[0] << 8) | data[1];
  raw >>= 7;

  *temp_c = raw * 0.5f;
  return SL_STATUS_OK;


#elif defined(PLATFORM_SERIES2)

  uint8_t reg = LM75_TEMP_REG;
  uint8_t buffer[2];

  I2C_TransferSeq_TypeDef seq;
  seq.addr  = LM75_I2C_ADDR << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  seq.buf[0].data = &reg;
  seq.buf[0].len  = 1;

  seq.buf[1].data = buffer;
  seq.buf[1].len  = 2;

  if (I2CSPM_Transfer(lm75_i2c, &seq) != i2cTransferDone) {
    return SL_STATUS_TRANSMIT;
  }

  int16_t raw = (buffer[0] << 8) | buffer[1];
  raw >>= 7;

  *temp_c = raw * 0.5f;
  return SL_STATUS_OK;

#endif
}


/*******************************************************************************
 * Celsius to Fahrenheit
 ******************************************************************************/
float lm75_celsius_to_fahrenheit(float c)
{
  return (c * 9.0f / 5.0f) + 32.0f;
}

