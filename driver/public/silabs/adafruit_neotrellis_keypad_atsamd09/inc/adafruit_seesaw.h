#ifndef _ADAFRUIT_SEESAW_H_
#define _ADAFRUIT_SEESAW_H_

#include <stdio.h>
#include "sl_status.h"
#include "em_i2c.h"
#include "sl_i2cspm.h"

/***************************************************************************//**
 *    //                       Macro
 ******************************************************************************/
/** Module Base Addreses
 *  The module base addresses for keypad and neopixel seesaw modules.
 */
enum {
  SEESAW_NEOPIXEL_BASE = 0x0E,
  SEESAW_KEYPAD_BASE = 0x10,
};

/** neopixel module function address registers
 */
enum {
  SEESAW_NEOPIXEL_STATUS = 0x00,
  SEESAW_NEOPIXEL_PIN = 0x01,
  SEESAW_NEOPIXEL_SPEED = 0x02,
  SEESAW_NEOPIXEL_BUF_LENGTH = 0x03,
  SEESAW_NEOPIXEL_BUF = 0x04,
  SEESAW_NEOPIXEL_SHOW = 0x05,
};

/** touch module function address registers
 */
enum {
  SEESAW_TOUCH_CHANNEL_OFFSET = 0x10,
};

/** keypad module function address registers
 */
enum {
  SEESAW_KEYPAD_STATUS = 0x00,
  SEESAW_KEYPAD_EVENT = 0x01,
  SEESAW_KEYPAD_INTENSET = 0x02,
  SEESAW_KEYPAD_INTENCLR = 0x03,
  SEESAW_KEYPAD_COUNT = 0x04,
  SEESAW_KEYPAD_FIFO = 0x10,
};

/** keypad module edge definitions
 */
enum {
  SEESAW_KEYPAD_EDGE_HIGH = 0,
  SEESAW_KEYPAD_EDGE_LOW,
  SEESAW_KEYPAD_EDGE_FALLING,
  SEESAW_KEYPAD_EDGE_RISING,
};

/** raw key event stucture for keypad module */
typedef union keyEventRaw {
  struct {
    uint8_t EDGE : 2; ///< the edge that was triggered
    uint8_t NUM : 6;  ///< the event number
  } bit;              ///< bitfield format
  uint8_t reg;        ///< register format
}keyEventRaw;

/** extended key event stucture for keypad module */
typedef union keyEvent {
  struct {
    uint8_t EDGE : 2;  ///< the edge that was triggered
    uint16_t NUM : 14; ///< the event number
  } bit;               ///< bitfield format
  uint16_t reg;        ///< register format
}keyEvent;

/** key state struct that will be written to seesaw chip keypad module */
typedef union keyState {
  struct {
    uint8_t STATE : 1;  ///< the current state of the key
    uint8_t ACTIVE : 4; ///< the registered events for that key
  } bit;                ///< bitfield format
  uint8_t reg;          ///< register format
}keyState;

typedef struct seesaw_t {
  uint8_t i2c_addr;
  sl_i2cspm_t *i2c_instance;
}seesaw_t;

/**************************************************************************/

/*!
 *     @brief  Seesaw chip Initialization.
 *     @param  seesaw_dev seesaw instance
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_seesaw_i2c_init(seesaw_t seesaw_dev);

/**************************************************************************/

/*!
 *     @brief  I2C Read function.
 *     @param  seesaw_dev seesaw instance
 *     @param  reg_high   Module base address
 *     @param  reg_low    Module function address
 *     @param  pdata      Buffer to store data
 *     @param  len        Length of data to be read
 *     @param  delay      Time to delay
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_seesaw_i2c_register_read(seesaw_t seesaw_dev,
                                              uint8_t reg_high,
                                              uint8_t reg_low,
                                              uint8_t *pdata,
                                              uint8_t len,
                                              uint16_t delay);

/**************************************************************************/

/*!
 *     @brief  I2C write function.
 *     @param  seesaw_dev seesaw instance
 *     @param  reg_high   Module base address
 *     @param  reg_low    Module function address
 *     @param  pdata      Data to be written
 *     @param  len        Length of data to be written
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_seesaw_i2c_register_write(seesaw_t seesaw_dev,
                                               uint8_t reg_high,
                                               uint8_t reg_low,
                                               uint8_t *pdata,
                                               uint16_t len);

/**************************************************************************/

/*!
 *     @brief  Get keypad count from keypad module.
 *     @param  seesaw_dev seesaw instance
 *     @return keypad count.
 */

/**************************************************************************/

int8_t adafruit_seesaw_get_keypad_count(seesaw_t seesaw_dev);

/**************************************************************************/

/*!
 *     @brief  Seesaw chip Initialization.
 *     @param  seesaw_dev seesaw instance
 *     @param  buf keypad buffer
 *     @param  count number event
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_seesaw_read_keypad(seesaw_t seesaw_dev,
                                        keyEventRaw *buf,
                                        uint8_t count);

#endif /* _ADAFRUIT_SEESAW_H_ */
