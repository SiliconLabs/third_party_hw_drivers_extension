/***************************************************************************//**
 * @file mikroe_bma400.h
 * @brief SCL BMA400 Prototypes
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

#ifndef MIKROE_BMA400_H_
#define MIKROE_BMA400_H_

#include "sl_status.h"
#include "sl_i2cspm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * \addtogroup mikroe_maxm86161 MAXM86161 - HEARTRATE2 Click
 * @{
 *
 * @brief
 *  The integration of the Mikroe SDK Click extension MAXM86161 driver.
 *  (HEARTRATE 2 Click board.)
 *
 ******************************************************************************/

// -------------------------------------------------------------- PUBLIC MACROS

/**
 * \defgroup macros Macros
 * \{
 */

/**
 * \defgroup error_code Error Code
 * \{
 */
#define MIKROE_BMA400_OK                                  0
#define MIKROE_BMA400_INIT_ERROR                          (-1)

/** \} */

/**
 * \defgroup chip Chip
 * \{
 */
#define MIKROE_BMA400_CHIP_ID                             0x90

/** \} */

/**
 * \defgroup register Register
 * \{
 */
#define MIKROE_BMA400_REG_CHIP_ID                         0x00
#define MIKROE_BMA400_REG_ERR_REG                         0x02
#define MIKROE_BMA400_REG_STATUS                          0x03
#define MIKROE_BMA400_REG_ACC_X_LSB                       0x04
#define MIKROE_BMA400_REG_ACC_X_MSB                       0x05
#define MIKROE_BMA400_REG_ACC_Y_LSB                       0x06
#define MIKROE_BMA400_REG_ACC_Y_MSB                       0x07
#define MIKROE_BMA400_REG_ACC_Z_LSB                       0x08
#define MIKROE_BMA400_REG_ACC_Z_MSB                       0x09
#define MIKROE_BMA400_REG_SENSOR_TIME_0                   0x0A
#define MIKROE_BMA400_REG_SENSOR_TIME_1                   0x0B
#define MIKROE_BMA400_REG_SENSOR_TIME_2                   0x0C
#define MIKROE_BMA400_REG_EVENT                           0x0D
#define MIKROE_BMA400_REG_INT_STATUS_0                    0x0E
#define MIKROE_BMA400_REG_INT_STATUS_1                    0x0F
#define MIKROE_BMA400_REG_INT_STATUS_2                    0x10
#define MIKROE_BMA400_REG_TEMPERATURE                     0x11
#define MIKROE_BMA400_REG_FIFO_LENGTH_0                   0x12
#define MIKROE_BMA400_REG_FIFO_LENGTH_1                   0x13
#define MIKROE_BMA400_REG_FIFO_DATA                       0x14
#define MIKROE_BMA400_REG_STEP_CNT_0                      0x15
#define MIKROE_BMA400_REG_STEP_CNT_1                      0x16
#define MIKROE_BMA400_REG_STEP_CNT_2                      0x17
#define MIKROE_BMA400_REG_STEP_STATUS                     0x18
#define MIKROE_BMA400_REG_ACC_CONGIG_0                    0x19
#define MIKROE_BMA400_REG_ACC_CONGIG_1                    0x1A
#define MIKROE_BMA400_REG_ACC_CONGIG_2                    0x1B
#define MIKROE_BMA400_REG_INT_CONFIG_0                    0x1F
#define MIKROE_BMA400_REG_INT_CONFIG_1                    0x20
#define MIKROE_BMA400_REG_INT1_MAP                        0x21
#define MIKROE_BMA400_REG_INT2_MAP                        0x22
#define MIKROE_BMA400_REG_INT12_MAP                       0x23
#define MIKROE_BMA400_REG_INT12_IO_CTRL                   0x24
#define MIKROE_BMA400_REG_FIFO_CONFIG_0                   0x26
#define MIKROE_BMA400_REG_FIFO_CONFIG_1                   0x27
#define MIKROE_BMA400_REG_FIFO_CONFIG_2                   0x28
#define MIKROE_BMA400_REG_FIFO_PWR_CONFIG                 0x29
#define MIKROE_BMA400_REG_AUTO_LOW_POW_0                  0x2A
#define MIKROE_BMA400_REG_AUTO_LOW_POW_1                  0x2B
#define MIKROE_BMA400_REG_AUTO_WAKEUP_0                   0x2C
#define MIKROE_BMA400_REG_AUTO_WAKEUP_1                   0x2D
#define MIKROE_BMA400_REG_WAKEUP_CONFIG_0                 0x2F
#define MIKROE_BMA400_REG_WAKEUP_CONFIG_1                 0x30
#define MIKROE_BMA400_REG_WAKEUP_CONFIG_2                 0x31
#define MIKROE_BMA400_REG_WAKEUP_CONFIG_3                 0x32
#define MIKROE_BMA400_REG_WAKEUP_CONFIG_4                 0x33
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_0               0x35
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_1               0x36
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_2               0x37
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_3               0x38
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_4               0x39
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_5               0x3A
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_6               0x3B
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_7               0x3C
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_8               0x3D
#define MIKROE_BMA400_REG_ORIENTCH_CONFIG_9               0x3E
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_0               0x3F
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_1               0x40
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_2               0x41
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_3               0x42
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_31              0x43
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_4               0x44
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_5               0x45
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_6               0x46
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_7               0x47
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_8               0x48
#define MIKROE_BMA400_REG_GEN1_INT_CONFIG_9               0x49
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_0               0x4A
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_1               0x4B
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_2               0x4C
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_3               0x4D
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_31              0x4E
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_4               0x4F
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_5               0x50
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_6               0x51
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_7               0x52
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_8               0x53
#define MIKROE_BMA400_REG_GEN2_INT_CONFIG_9               0x54
#define MIKROE_BMA400_REG_ACTCH_CONFIG_0                  0x55
#define MIKROE_BMA400_REG_ACTCH_CONFIG_1                  0x56
#define MIKROE_BMA400_REG_TAP_CONFIG_0                    0x57
#define MIKROE_BMA400_REG_TAP_CONFIG_1                    0x58
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_0               0x59
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_1               0x5A
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_2               0x5B
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_3               0x5C
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_4               0x5D
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_5               0x5E
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_6               0x5F
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_7               0x60
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_8               0x61
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_9               0x62
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_10              0x63
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_11              0x64
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_12              0x65
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_13              0x66
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_14              0x67
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_15              0x68
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_16              0x69
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_17              0x6A
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_18              0x6B
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_19              0x6C
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_20              0x6D
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_21              0x6E
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_22              0x6F
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_23              0x70
#define MIKROE_BMA400_REG_STEP_CNT_CONFIG_24              0x71
#define MIKROE_BMA400_REG_IF_CONFIG                       0x7C
#define MIKROE_BMA400_REG_SELF_TEST                       0x7D
#define MIKROE_BMA400_REG_CMD                             0x7E

/** \} */

/**
 * \defgroup error_status_register Error Status Register
 * \{
 */
#define MIKROE_BMA400_ERROR_CMD                           0x02

/** \} */

/**
 * \defgroup status_register Status Register
 * \{
 */
#define  MIKROE_BMA400_STATUS_DATA_RDY_START              0x80
#define  MIKROE_BMA400_STATUS_RDY_CMD                     0x10
#define  MIKROE_BMA400_STATUS_NORMAL_MODE                 0x04
#define  MIKROE_BMA400_STATUS_LOW_POWER_MODE              0x02
#define  MIKROE_BMA400_STATUS_SLEEP_MODE                  0x04
#define  MIKROE_BMA400_STATUS_INT_ACTIVE_TRIGGERED        0x01
#define  MIKROE_BMA400_STATUS_INT_ACTIVE_NOT_TRIGGERED    0x00

/** \} */

/**
 * \defgroup sensor_status_flags_event  Sensor Status Flags Event
 * \{
 */
#define MIKROE_BMA400_EVENT_POR_DETECTED                  0x01

/** \} */

/**
 * \defgroup interrupt_status_0   Interrupt Status 0
 * \{
 */
#define MIKROE_BMA400_INT_STATUS_0_DATA_RDY_STATUS        0x80
#define MIKROE_BMA400_INT_STATUS_0_FIFO_WATERMARK         0x40
#define MIKROE_BMA400_INT_STATUS_0_FIFO_FULL              0x20
#define MIKROE_BMA400_INT_STATUS_0_IENG_OVERRUN_STATUS    0x10
#define MIKROE_BMA400_INT_STATUS_0_GEN2_INT_STATUS        0x08
#define MIKROE_BMA400_INT_STATUS_0_GEN1_INT_STATUS        0x04
#define MIKROE_BMA400_INT_STATUS_0_ORIENTCH_INT_STATUS    0x02
#define MIKROE_BMA400_INT_STATUS_0_WAKEUP_INT_STATUS      0x01

/** \} */

/**
 * \defgroup interrupt_status_1   Interrupt Status 1
 * \{
 */
#define MIKROE_BMA400_INT_STATUS_1_IENG_OVERRUN_STATUS    0x10
#define MIKROE_BMA400_INT_STATUS_1_DOUBLE_TAP             0x08
#define MIKROE_BMA400_INT_STATUS_1_SINGLE_TAP             0x04
#define MIKROE_BMA400_INT_STATUS_1_STEP_NO_SET            0x00
#define MIKROE_BMA400_INT_STATUS_1_STEP_SET               0x01
#define MIKROE_BMA400_INT_STATUS_1_STEP_DETECT            0x02
#define MIKROE_BMA400_INT_STATUS_1_STEP_NO_USED           0x03

/** \} */

/**
 * \defgroup interrupt_status_2   Interrupt Status 2
 * \{
 */
#define MIKROE_BMA400_INT_STATUS_2_IENG_OVERRUN_STATUS    0x10
#define MIKROE_BMA400_INT_STATUS_2_ACTCH_Z_INIT_STATUS    0x04
#define MIKROE_BMA400_INT_STATUS_2_ACTCH_Y_INIT_STATUS    0x02
#define MIKROE_BMA400_INT_STATUS_2_ACTCH_X_INIT_STATUS    0x01

/** \} */

/**
 * \defgroup step_status  Step Status
 * \{
 */
#define MIKROE_BMA400_STEP_STATUS_WALKING                 0x01
#define MIKROE_BMA400_STEP_STATUS_RUNNING                 0x02
#define MIKROE_BMA400_STEP_STATUS_NO_WALK_RUN             0x00

/** \} */

/**
 * \defgroup accel_config_0   Accel Config 0
 * \{
 */
#define MIKROE_BMA400_CFG_0_FILT1_BW_LOW_0_2X_ODR         0x80
#define MIKROE_BMA400_CFG_0_FILT1_BW_HIGH_0_4X_ODR        0x00
#define MIKROE_BMA400_CFG_0_SLEEP_MODE                    0x00
#define MIKROE_BMA400_CFG_0_LOW_POWER_MODE                0x01
#define MIKROE_BMA400_CFG_0_NORMAL_MODE                   0x02

/** \} */

/**
 * \defgroup accel_config_1   Accel Config 1
 * \{
 */
#define MIKROE_BMA400_CFG_1_ACC_RANGE_2g                  0x00
#define MIKROE_BMA400_CFG_1_ACC_RANGE_4g                  0x40
#define MIKROE_BMA400_CFG_1_ACC_RANGE_8g                  0x80
#define MIKROE_BMA400_CFG_1_ACC_RANGE_16g                 0xC0
#define MIKROE_BMA400_CFG_1_OSR_LOW_POWER                 0x00
#define MIKROE_BMA400_CFG_1_OSR_HIGH_POWER                0x30
#define MIKROE_BMA400_CFG_1_ODR_12p5_5                    0x00
#define MIKROE_BMA400_CFG_1_ODR_12p5_4                    0x01
#define MIKROE_BMA400_CFG_1_ODR_12p5_3                    0x02
#define MIKROE_BMA400_CFG_1_ODR_12p5_2                    0x03
#define MIKROE_BMA400_CFG_1_ODR_12p5_1                    0x04
#define MIKROE_BMA400_CFG_1_ODR_12p5                      0x05
#define MIKROE_BMA400_CFG_1_ODR_25                        0x06
#define MIKROE_BMA400_CFG_1_ODR_50                        0x07
#define MIKROE_BMA400_CFG_1_ODR_100                       0x08
#define MIKROE_BMA400_CFG_1_ODR_200                       0x09
#define MIKROE_BMA400_CFG_1_ODR_400                       0x0A
#define MIKROE_BMA400_CFG_1_ODR_800                       0x0B
#define MIKROE_BMA400_CFG_1_ODR_800_1                     0x0C
#define MIKROE_BMA400_CFG_1_ODR_800_2                     0x0D
#define MIKROE_BMA400_CFG_1_ODR_800_3                     0x0E
#define MIKROE_BMA400_CFG_1_ODR_800_4                     0x0F

/** \} */

/**
 * \defgroup accel_config_2   Accel Config 2
 * \{
 */
#define MIKROE_BMA400_CFG_2_DATA_SCR_ACC_FILT_1           0x00
#define MIKROE_BMA400_CFG_2_DATA_SCR_ACC_FILT_2           0x04
#define MIKROE_BMA400_CFG_2_DATA_SCR_ACC_FILT_LP          0x08

/** \} */

/**
 * \defgroup interrupt_config_0   Interrupt Config 0
 * \{
 */
#define MIKROE_BMA400_INT_CFG_0_DATA_RDY                  0x80
#define MIKROE_BMA400_INT_CFG_0_FIFO_WATERMARK            0x40
#define MIKROE_BMA400_INT_CFG_0_FIFO_FULL                 0x20
#define MIKROE_BMA400_INT_CFG_0_GEN2_INT_STATUS           0x08
#define MIKROE_BMA400_INT_CFG_0_GEN1_INT_STATUS           0x04
#define MIKROE_BMA400_INT_CFG_0_ORIENTCH_INT              0x02

/** \} */

/**
 * \defgroup interrupt_config_1   Interrupt Config 1
 * \{
 */
#define MIKROE_BMA400_INT_CFG_1_LATCH_MODE_NOLATCH        0x00
#define MIKROE_BMA400_INT_CFG_1_LATCH_MODE_LATCHING       0x80
#define MIKROE_BMA400_INT_CFG_1_ACTCH_INT_ENABLE          0x10
#define MIKROE_BMA400_INT_CFG_1_DOUBLE_TAP_ENABLE         0x08
#define MIKROE_BMA400_INT_CFG_1_SINGLE_TAP_ENABLE         0x04
#define MIKROE_BMA400_INT_CFG_1_STEP_INT_ENABLE           0x01

/** \} */

/**
 * \defgroup interrupt_map_1  Interrupt Map 1
 * \{
 */
#define MIKROE_BMA400_INT1_MAP_DATA_RDY_STATUS            0x80
#define MIKROE_BMA400_INT1_MAP_FIFO_WATERMARK             0x40
#define MIKROE_BMA400_INT1_MAP_FIFO_FULL                  0x20
#define MIKROE_BMA400_INT1_MAP_IENG_OVERRUN_STATUS        0x10
#define MIKROE_BMA400_INT1_MAP_GEN2_INT_STATUS            0x08
#define MIKROE_BMA400_INT1_MAP_GEN1_INT_STATUS            0x04
#define MIKROE_BMA400_INT1_MAP_ORIENTCH_INT_STATUS        0x02
#define MIKROE_BMA400_INT1_MAP_WAKEUP_INT_STATUS          0x01

/** \} */

/**
 * \defgroup interrupt_map_2  Interrupt Map 2
 * \{
 */
#define MIKROE_BMA400_INT2_MAP_DATA_RDY_STATUS            0x80
#define MIKROE_BMA400_INT2_MAP_FIFO_WATERMARK             0x40
#define MIKROE_BMA400_INT2_MAP_FIFO_FULL                  0x20
#define MIKROE_BMA400_INT2_MAP_IENG_OVERRUN_STATUS        0x10
#define MIKROE_BMA400_INT2_MAP_GEN2_INT_STATUS            0x08
#define MIKROE_BMA400_INT2_MAP_GEN1_INT_STATUS            0x04
#define MIKROE_BMA400_INT2_MAP_ORIENTCH_INT_STATUS        0x02
#define MIKROE_BMA400_INT2_MAP_WAKEUP_INT_STATUS          0x01

/** \} */

/**
 * \defgroup interrupt_map_1_2  Interrupt Map_1_2
 * \{
 */
#define MIKROE_BMA400_INT12_MAP_ACTCH_INT2                0x80
#define MIKROE_BMA400_INT12_MAP_TAP_INT2                  0x40
#define MIKROE_BMA400_INT12_MAP_STEP_INT2                 0x10
#define MIKROE_BMA400_INT12_MAP_ACTCH_INT1                0x08
#define MIKROE_BMA400_INT12_MAP_TAP_INT1                  0x04
#define MIKROE_BMA400_INT12_MAP_STEP_INT1                 0x01

/** \} */

/**
 * \defgroup interrupt_io_ctrl_1_2 Interrupt Io Ctrl 1 2
 * \{
 */
#define MIKROE_BMA400_INT12_CTRL_INT2_OPEN_DRAIN          0x40
#define MIKROE_BMA400_INT12_CTRL_INT1_OPEN_DRAIN          0x04
#define MIKROE_BMA400_INT12_CTRL_INT2_HIGH_ACTIVE         0x20
#define MIKROE_BMA400_INT12_CTRL_INT1_HIGH_ACTIVE         0x02

/** \} */

/**
 * \defgroup fifo_config_0    Fifo Config 0
 * \{
 */
#define MIKROE_BMA400_FIFO_CFG0_Z_AXIS_ENABLE             0x80
#define MIKROE_BMA400_FIFO_CFG0_Y_AXIS_ENABLE             0x40
#define MIKROE_BMA400_FIFO_CFG0_X_AXIS_ENABLE             0x20
#define MIKROE_BMA400_FIFO_CFG0_8bit_ENABLE               0x10
#define MIKROE_BMA400_FIFO_CFG0_DATA_SRC_ENABLE           0x08
#define MIKROE_BMA400_FIFO_CFG0_TIME_ENABLE               0x04
#define MIKROE_BMA400_FIFO_CFG0_STOP_NO_FULL_ENABLE       0x02
#define MIKROE_BMA400_FIFO_CFG0_AUTO_FLUSH_ENABLE         0x01

/** \} */

/**
 * \defgroup auto_low_pow_1_register  Auto Low Pow 1 Register
 * \{
 */
#define MIKROE_BMA400_FIFO_AUTO_IP_TIMEOUT_0              0x00
#define MIKROE_BMA400_FIFO_AUTO_IP_TIMEOUT_1              0x04
#define MIKROE_BMA400_FIFO_AUTO_IP_TIMEOUT_2              0x08
#define MIKROE_BMA400_FIFO_AUTO_IP_TIMEOUT_3              0x0C
#define MIKROE_BMA400_FIFO_AUTO_TRIG_GEN1_INT             0x02
#define MIKROE_BMA400_FIFO_AUTO_TRIG_DATA_RDY             0x01

/** \} */

/**
 * \defgroup auto_wakeup   Auto Wakeup
 * \{
 */
#define MIKROE_BMA400_WAKEUP_TIMEOUT_ENABLE               0x04
#define MIKROE_BMA400_WAKEUP_ENABLE                       0x02

/** \} */

/**
 * \defgroup wakeup_config_o    Wakeup Config 0
 * \{
 */
#define MIKROE_BMA400_WAKEUP_Z_AXIS_ENABLE                0x80
#define MIKROE_BMA400_WAKEUP_Y_AXIS_ENABLE                0x40
#define MIKROE_BMA400_WAKEUP_X_AXIS_ENABLE                0x20
#define MIKROE_BMA400_WAKEUP_NUMBER_OF_SIMPLE             0x10
#define MIKROE_BMA400_WAKEUP_REFU_MANUAL                  0x00
#define MIKROE_BMA400_WAKEUP_REFU_ONETIME                 0x01
#define MIKROE_BMA400_WAKEUP_REFU_EVERYTIME               0x02

/** \} */

/**
 * \defgroup orient_config_0    Orient config 0
 * \{
 */
#define MIKROE_BMA400_ORIENT_Z_AXIS_ENABLE                0x80
#define MIKROE_BMA400_ORIENT_Y_AXIS_ENABLE                0x40
#define MIKROE_BMA400_ORIENT_X_AXIS_ENABLE                0x20
#define MIKROE_BMA400_ORIENT_DATA_SRC_ENABLE              0x10
#define MIKROE_BMA400_ORIENT_REFU_MANUAL                  0x00
#define MIKROE_BMA400_ORIENT_REFU_ONETIME_2               0x04
#define MIKROE_BMA400_ORIENT_REFU_ONETIME_IP              0x08
#define MIKROE_BMA400_ORIENT_STABILITY_INACTIVE           0x00
#define MIKROE_BMA400_ORIENT_STABILITY_ENABLED_1          0x01
#define MIKROE_BMA400_ORIENT_STABILITY_ENABLED_2          0x02

/** \} */

/**
 * \defgroup generic_interrupt_1_config_0   Generic Interrupt 1 Config 0
 * \{
 */
#define MIKROE_BMA400_GEN1_CFG0_ACT_Z_ENABLE              0x80
#define MIKROE_BMA400_GEN1_CFG0_ACT_Y_ENABLE              0x40
#define MIKROE_BMA400_GEN1_CFG0_ACT_X_ENABLE              0x20
#define MIKROE_BMA400_GEN1_CFG0_DATA_ENABLE               0x10
#define MIKROE_BMA400_GEN1_CFG0_REFU_MANUAL               0x00
#define MIKROE_BMA400_GEN1_CFG0_REFU_ONETIME              0x04
#define MIKROE_BMA400_GEN1_CFG0_REFU_EVERYTIME            0x08
#define MIKROE_BMA400_GEN1_CFG0_HYST_24mg                 0x01
#define MIKROE_BMA400_GEN1_CFG0_HYST_48mg                 0x02
#define MIKROE_BMA400_GEN1_CFG0_HYST_96mg                 0x03
#define MIKROE_BMA400_GEN1_CFG0_HYST_NO_ACTIVE            0x00

/** \} */

/**
 * \defgroup generic_interrupt_1_config_1   Generic Interrupt 1 Config 1
 * \{
 */
#define MIKROE_BMA400_GEN1_CFG1_CRITERION_SEL_ACTIVE      0x20
#define MIKROE_BMA400_GEN1_CFG1_CRITERION_SEL_INACTIVE    0x00
#define MIKROE_BMA400_GEN1_CFG1_COMB_SEL_AND              0x10
#define MIKROE_BMA400_GEN1_CFG1_COMB_SEL_OR               0x00

/** \} */

/**
 * \defgroup generic_interrupt_2_consfig_1   Generic Interrupt 2 Consfig 1
 * \{
 */
#define MIKROE_BMA400_GEN2_CFG0_ACT_Z_ENABLE              0x80
#define MIKROE_BMA400_GEN2_CFG0_ACT_Y_ENABLE              0x40
#define MIKROE_BMA400_GEN2_CFG0_ACT_X_ENABLE              0x20
#define MIKROE_BMA400_GEN2_CFG0_DATA_ENABLE               0x10
#define MIKROE_BMA400_GEN2_CFG0_REFU_MANUAL               0x00
#define MIKROE_BMA400_GEN2_CFG0_REFU_ONETIME              0x04
#define MIKROE_BMA400_GEN2_CFG0_REFU_EVERYTIME            0x08
#define MIKROE_BMA400_GEN2_CFG0_HYST_24mg                 0x01
#define MIKROE_BMA400_GEN2_CFG0_HYST_48mg                 0x02
#define MIKROE_BMA400_GEN2_CFG0_HYST_96mg                 0x03
#define MIKROE_BMA400_GEN2_CFG0_HYST_NO_ACTIVE            0x00

/** \} */

/**
 * \defgroup generic_interrupt_2_config_1   Generic Interrupt 2 Config 1
 * \{
 */
#define MIKROE_BMA400_GEN2_CFG1_CRITERION_SEL_ACTIVE      0x20
#define MIKROE_BMA400_GEN2_CFG1_CRITERION_SEL_INACTIVE    0x00
#define MIKROE_BMA400_GEN2_CFG1_COMB_SEL_AND              0x10
#define MIKROE_BMA400_GEN2_CFG1_COMB_SEL_OR               0x00

/** \} */

/**
 * \defgroup activity_interrupt_config    Activity Interrupt config
 * \{
 */
#define MIKROE_BMA400_ACTCH_CFG0_Z_AXIS_ENABLE            0x80
#define MIKROE_BMA400_ACTCH_CFG0_Y_AXIS_ENABLE            0x40
#define MIKROE_BMA400_ACTCH_CFG0_X_AXIS_ENABLE            0x20
#define MIKROE_BMA400_ACTCH_CFG0_DATA_ENABLE              0x10
#define MIKROE_BMA400_ACTCH_CFG0_NPTS_POINT_32            0x00
#define MIKROE_BMA400_ACTCH_CFG0_NPTS_POINT_64            0x01
#define MIKROE_BMA400_ACTCH_CFG0_NPTS_POINT_128           0x02
#define MIKROE_BMA400_ACTCH_CFG0_NPTS_POINT_256           0x03
#define MIKROE_BMA400_ACTCH_CFG0_NPTS_POINT_512           0x04

/** \} */

/**
 * \defgroup tap_config_0    Tap Config 0
 * \{
 */
#define MIKROE_BMA400_TAP_CFG0_USE_Z_AXIS                 0x00
#define MIKROE_BMA400_TAP_CFG0_USE_Y_AXIS                 0x01
#define MIKROE_BMA400_TAP_CFG0_USE_X_AXIS                 0x02
#define MIKROE_BMA400_TAP_CFG0_TAP_SEL_HIGH               0x00
#define MIKROE_BMA400_TAP_CFG0_TAP_SEL_LOW                0x10

/** \} */

/**
 * \defgroup tap_config_1    Tap Config 1
 * \{
 */
#define MIKROE_BMA400_TAP_CFG1_QUIET_DT_SAMPLE_4          0x00
#define MIKROE_BMA400_TAP_CFG1_QUIET_DT_SAMPLE_8          0x10
#define MIKROE_BMA400_TAP_CFG1_QUIET_DT_SAMPLE_12         0x20
#define MIKROE_BMA400_TAP_CFG1_QUIET_DT_SAMPLE_16         0x30
#define MIKROE_BMA400_TAP_CFG1_QUIET_SAMPLE_60            0x00
#define MIKROE_BMA400_TAP_CFG1_QUIET_SAMPLE_80            0x04
#define MIKROE_BMA400_TAP_CFG1_QUIET_SAMPLE_100           0x08
#define MIKROE_BMA400_TAP_CFG1_QUIET_SAMPLE_120           0x0C
#define MIKROE_BMA400_TAP_CFG1_TICS_TH_SAMPLE_6           0x00
#define MIKROE_BMA400_TAP_CFG1_TICS_TH_SAMPLE_9           0x01
#define MIKROE_BMA400_TAP_CFG1_TICS_TH_SAMPLE_12          0x02
#define MIKROE_BMA400_TAP_CFG1_TICS_TH_SAMPLE_18          0x03

/** \} */

/**
 * \defgroup if_config    If Config
 * \{
 */
#define MIKROE_BMA400_IF_CONFIG_SPI_4_WIRE                0x00
#define MIKROE_BMA400_IF_CONFIG_SPI_3_WIRE                0x01

/** \} */

/**
 * \defgroup self_test    Self Test
 * \{
 */
#define MIKROE_BMA400_TEST_SIGN_ENABLE                    0x08
#define MIKROE_BMA400_TEST_Z_AXIS_ENABLE                  0x04
#define MIKROE_BMA400_TEST_Y_AXIS_ENABLE                  0x02
#define MIKROE_BMA400_TEST_X_AXIS_ENABLE                  0x01

/** \} */

/**
 * \defgroup command_register    Comand Register
 * \{
 */
#define MIKROE_BMA400_CMD_FIFO_FLUSH                      0xB0
#define MIKROE_BMA400_CMD_STEP_CNT_CLEAR                  0xB1
#define MIKROE_BMA400_CMD_SOFTWARE_RESET                  0xB6

/** \} */

/**
 * \defgroup axis    Axis
 * \{
 */
#define MIKROE_BMA400_X_AXIS                              0x04
#define MIKROE_BMA400_Y_AXIS                              0x06
#define MIKROE_BMA400_Z_AXIS                              0x08

/** \} */

/**
 * \defgroup flags    Flags
 * \{
 */
#define DEVICE_ERROR                                      0x01
#define DEVICE_OK                                         0x00

/** \} */

/** \} */   // End group macro

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_bma400_init(sl_i2cspm_t *i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the IC2SPM instance used by platform functions.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_bma400_set_i2csmp_instance(sl_i2cspm_t *i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function executes default configuration for click
 *
 * @param[in] mode
 *    Mode.
 * @param[in] range
 *    Range.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_bma400_default_cfg(uint8_t mode, uint8_t range);

/***************************************************************************//**
 * @brief
 *  Check whether a special BMA400 is present on the bus or not.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_bma400_present(void);

/***************************************************************************//**
 * @brief
 *    This function writes data to the desired register.
 *
 * @param[in] register_address
 *    Register address
 *
 * @param[in] write_data
 *    Data to be written.
 *
 * @return
 *    None
 ******************************************************************************/
sl_status_t mikroe_bma400_generic_write (uint8_t reg,
                                         uint8_t *data_buf,
                                         uint8_t len);

/***************************************************************************//**
 * @brief
 *    This function reads data from the desired register.
 *
 * @param[in] reg
 *    Register address.
 * @param[out] data_buf
 *    Output data buf.
 * @param[in] len
 *    Number of the bytes to be read.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if data_buf is NULL
 ******************************************************************************/
sl_status_t mikroe_bma400_generic_read (uint8_t reg,
                                        uint8_t *data_buf,
                                        uint8_t len);

/***************************************************************************//**
 * @brief
 *    This function write byte.
 *
 * @param[in] reg
 *    Register address.
 * @param[in] reg_data
 *    data.
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_bma400_write_byte (uint8_t reg, uint8_t reg_data);

/***************************************************************************//**
 * @brief
 *    This function read one byte data which is read from the register.
 *
 * @param[in] reg
 *    Register address.
 * @param[out] data
 *    Output data.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if data is NULL
 ******************************************************************************/
sl_status_t mikroe_bma400_read_byte (uint8_t reg, uint8_t *data);

/***************************************************************************//**
 * @brief
 *    This function gets axis.
 *
 * @param[in] axis
 *    Axis.
 * @param[out] data
 *    Output data.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if data is NULL
 ******************************************************************************/
sl_status_t mikroe_bma400_get_axis (uint8_t axis, int16_t *data);

/***************************************************************************//**
 * @brief
 *    This function gets the value read from the three time sensor register.
 *
 * @param[out] sensor_time
 *    The internal sensor time
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if sensor_time is NULL
 ******************************************************************************/
sl_status_t mikroe_bma400_sensor_time (uint32_t *sensor_time);

/***************************************************************************//**
 * @brief
 *    This function read temperature.
 *
 * @param[out] temperature
 *    Temperature.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if temperature is NULL
 ******************************************************************************/
sl_status_t mikroe_bma400_get_temperature (float *temperature);

/***************************************************************************//**
 * @brief
 *    This function reads data buffer from the desired register.
 *
 * @param[in] reg
 *    Register address
 *
 * @param[in] data_buf
 *    Read buffer to store data.
 *
 * @param[in] len
 *    Size of read buffer.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if data is NULL
 ******************************************************************************/
sl_status_t mikroe_bma400_read_data (uint8_t reg, uint16_t *data);

/***************************************************************************//**
 * @brief
 *    This function restarts device.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_bma400_soft_reset (void);

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* MIKROE_BMA400_H_ */
