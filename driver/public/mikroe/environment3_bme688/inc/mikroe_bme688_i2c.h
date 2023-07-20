/***************************************************************************//**
 * @file mikroe_bme688_i2c.h
 * @brief SCL BME688 Prototypes
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

#ifndef MIKROE_BME688_I2C_H_
#define MIKROE_BME688_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "environment3.h"
#include "sl_status.h"
#include "sl_i2cspm.h"

// -------------------------------------------------------------- PUBLIC MACROS

/**
 * \defgroup macros Macros
 * \{
 */

/** \} */

/**
 * @brief Environment 3 memory map summary.
 * @details The full list of registers.
 */
#define MIKROE_ENVIRONMENT3_REG_STATUS                 0x73
#define MIKROE_ENVIRONMENT3_REG_VARIANT_ID             0xF0
#define MIKROE_ENVIRONMENT3_REG_RESET                  0xE0
#define MIKROE_ENVIRONMENT3_REG_CHIP_ID                0xD0
#define MIKROE_ENVIRONMENT3_REG_CONFIG                 0x75
#define MIKROE_ENVIRONMENT3_REG_CTRL_MEAS              0x74
#define MIKROE_ENVIRONMENT3_REG_CTRL_HUM               0x72
#define MIKROE_ENVIRONMENT3_REG_CTRL_GAS_1             0x71
#define MIKROE_ENVIRONMENT3_REG_CTRL_GAS_0             0x70
#define MIKROE_ENVIRONMENT3_REG_GAS_WAIT_SHARED        0x6E
#define MIKROE_ENVIRONMENT3_REG_GAS_WAIT_0             0x64
#define MIKROE_ENVIRONMENT3_REG_RES_HEAT_0             0x5A
#define MIKROE_ENVIRONMENT3_REG_IDAC_HEAT_0            0x50
#define MIKROE_ENVIRONMENT3_REG_GAS_R_LSB_FIELD_2      0x4F
#define MIKROE_ENVIRONMENT3_REG_GAS_R_MSB_FIELD_2      0x4E
#define MIKROE_ENVIRONMENT3_REG_HUM_LSB_FIELD_2        0x48
#define MIKROE_ENVIRONMENT3_REG_HUM_MSB_FIELD_2        0x47
#define MIKROE_ENVIRONMENT3_REG_TEMP_XLSB_FIELD_2      0x46
#define MIKROE_ENVIRONMENT3_REG_TEMP_LSB_FIELD_2       0x45
#define MIKROE_ENVIRONMENT3_REG_TEMP_MSB_FIELD_2       0x44
#define MIKROE_ENVIRONMENT3_REG_PRESS_XLSB_FIELD_2     0x43
#define MIKROE_ENVIRONMENT3_REG_PRESS_LSB_FIELD_2      0x42
#define MIKROE_ENVIRONMENT3_REG_PRESS_MSB_FIELD_2      0x41
#define MIKROE_ENVIRONMENT3_REG_SUB_MEAS_INDEX_FIELD_2 0x40
#define MIKROE_ENVIRONMENT3_REG_MEAS_STATUS_FIELD_2    0x3F
#define MIKROE_ENVIRONMENT3_REG_GAS_R_LSB_FIELD_1      0x3E
#define MIKROE_ENVIRONMENT3_REG_GAS_R_MSB_FIELD_1      0x3D
#define MIKROE_ENVIRONMENT3_REG_HUM_LSB_FIELD_1        0x37
#define MIKROE_ENVIRONMENT3_REG_HUM_MSB_FIELD_1        0x36
#define MIKROE_ENVIRONMENT3_REG_TEMP_XLSB_FIELD_1      0x35
#define MIKROE_ENVIRONMENT3_REG_TEMP_LSB_FIELD_1       0x34
#define MIKROE_ENVIRONMENT3_REG_TEMP_MSB_FIELD_1       0x33
#define MIKROE_ENVIRONMENT3_REG_PRESS_XLSB_FIELD_1     0x32
#define MIKROE_ENVIRONMENT3_REG_PRESS_LSB_FIELD_1      0x31
#define MIKROE_ENVIRONMENT3_REG_PRESS_MSB_FIELD_1      0x30
#define MIKROE_ENVIRONMENT3_REG_SUB_MEAS_INDEX_FIELD_1 0x2F
#define MIKROE_ENVIRONMENT3_REG_MEAS_STATUS_FIELD_1    0x2E
#define MIKROE_ENVIRONMENT3_REG_GAS_R_LSB_FIELD_0      0x2D
#define MIKROE_ENVIRONMENT3_REG_GAS_R_MSB_FIELD_0      0x2C
#define MIKROE_ENVIRONMENT3_REG_HUM_LSB_FIELD_0        0x26
#define MIKROE_ENVIRONMENT3_REG_HUM_MSB_FIELD_0        0x25
#define MIKROE_ENVIRONMENT3_REG_TEMP_XLSB_FIELD_0      0x24
#define MIKROE_ENVIRONMENT3_REG_TEMP_LSB_FIELD_0       0x23
#define MIKROE_ENVIRONMENT3_REG_TEMP_MSB_FIELD_0       0x22
#define MIKROE_ENVIRONMENT3_REG_PRESS_XLSB_FIELD_0     0x21
#define MIKROE_ENVIRONMENT3_REG_PRESS_LSB_FIELD_0      0x20
#define MIKROE_ENVIRONMENT3_REG_PRESS_MSB_FIELD_0      0x1F
#define MIKROE_ENVIRONMENT3_REG_SUB_MEAS_INDEX_FIELD_0 0x1E
#define MIKROE_ENVIRONMENT3_REG_MEAS_STATUS_FIELD_0    0x1D
#define MIKROE_ENVIRONMENT3_REG_MEM_PAGE               0xF3
#define MIKROE_ENVIRONMENT3_REG_UNIQUE_ID              0x83
#define MIKROE_ENVIRONMENT3_REG_COEFF_1                0x8A
#define MIKROE_ENVIRONMENT3_REG_COEFF_2                0xE1
#define MIKROE_ENVIRONMENT3_REG_COEFF_3                0x00

/*! @} */ // environment3_reg

/**
 * @defgroup environment3_set Environment 3 Registers Settings
 * @brief Settings for registers of Environment 3 Click driver.
 */

/**
 * @addtogroup environment3_set
 * @{
 */

/**
 * @brief Environment 3 chip ID.
 * @details Unique chip ID value.
 */
#define MIKROE_ENVIRONMENT3_CHIP_ID                0x61

/**
 * @brief Environment 3 reset settings.
 * @details Reset setting.
 */
#define MIKROE_ENVIRONMENT3_SOFT_RESET             0xB6

/**
 * @brief Environment 3 errors summary.
 * @details The list of errors.
 */
#define MIKROE_ENVIRONMENT3_E_NULL_PTR             -2
#define MIKROE_ENVIRONMENT3_E_COM_FAIL             -3
#define MIKROE_ENVIRONMENT3_E_INVALID_LENGTH       -4
#define MIKROE_ENVIRONMENT3_W_DEFINE_OP_MODE       1
#define MIKROE_ENVIRONMENT3_W_NO_NEW_DATA          2

/**
 * @brief Environment 3 register mask summary.
 * @details The list of register masks.
 */
#define MIKROE_ENVIRONMENT3_NBCONV_MASK            0x0F
#define MIKROE_ENVIRONMENT3_FILTER_MASK            0x1C
#define MIKROE_ENVIRONMENT3_ODR3_MASK              0x80
#define MIKROE_ENVIRONMENT3_ODR20_MASK             0xE0
#define MIKROE_ENVIRONMENT3_OST_MASK               0xE0
#define MIKROE_ENVIRONMENT3_OSP_MASK               0x1C
#define MIKROE_ENVIRONMENT3_OSH_MASK               0x07
#define MIKROE_ENVIRONMENT3_HCTRL_MASK             0x08
#define MIKROE_ENVIRONMENT3_RUN_GAS_MASK           0x30
#define MIKROE_ENVIRONMENT3_MODE_MASK              0x03
#define MIKROE_ENVIRONMENT3_RHRANGE_MASK           0x30
#define MIKROE_ENVIRONMENT3_RSERROR_MASK           0xF0
#define MIKROE_ENVIRONMENT3_NEW_DATA_MASK          0x80
#define MIKROE_ENVIRONMENT3_GAS_INDEX_MASK         0x0F
#define MIKROE_ENVIRONMENT3_GAS_RANGE_MASK         0x0F
#define MIKROE_ENVIRONMENT3_GASM_VALID_MASK        0x20
#define MIKROE_ENVIRONMENT3_HEAT_STAB_MASK         0x10
#define MIKROE_ENVIRONMENT3_MEM_PAGE_MASK          0x10
#define MIKROE_ENVIRONMENT3_BIT_H1_DATA_MASK       0x0F
#define MIKROE_ENVIRONMENT3_HUM_REG_SHIFT_VAL_MASK 0x04
#define MIKROE_ENVIRONMENT3_SPI_READ_MASK          0x80
#define MIKROE_ENVIRONMENT3_SPI_WRITE_MASK         0x7F

/**
 * @brief Environment 3 register bit position summary.
 * @details The list of bit position masks.
 */
#define MIKROE_ENVIRONMENT3_FILTER_POS             2
#define MIKROE_ENVIRONMENT3_OST_POS                5
#define MIKROE_ENVIRONMENT3_OSP_POS                2
#define MIKROE_ENVIRONMENT3_ODR3_POS               7
#define MIKROE_ENVIRONMENT3_ODR20_POS              5
#define MIKROE_ENVIRONMENT3_RUN_GAS_POS            4
#define MIKROE_ENVIRONMENT3_HCTRL_POS              3

/**
 * @brief Environment 3 enable/disable macros.
 * @details Enable/disable macros.
 */
#define MIKROE_ENVIRONMENT3_ENABLE                 0x01
#define MIKROE_ENVIRONMENT3_DISABLE                0x00
#define MIKROE_ENVIRONMENT3_ENABLE_HEATER          0x00
#define MIKROE_ENVIRONMENT3_DISABLE_HEATER         0x01
#define MIKROE_ENVIRONMENT3_ENABLE_GAS_MEAS_H      0x02
#define MIKROE_ENVIRONMENT3_ENABLE_GAS_MEAS_L      0x01
#define MIKROE_ENVIRONMENT3_DISABLE_GAS_MEAS       0x00

/**
 * @brief Environment 3 variant ID macros.
 * @details Variant ID macros.
 */
#define MIKROE_ENVIRONMENT3_VARIANT_GAS_LOW        0x00
#define MIKROE_ENVIRONMENT3_VARIANT_GAS_HIGH       0x01

/**
 * @brief Environment 3 oversampling setting macros.
 * @details Oversampling setting macros.
 */
#define MIKROE_ENVIRONMENT3_OS_NONE                0
#define MIKROE_ENVIRONMENT3_OS_1X                  1
#define MIKROE_ENVIRONMENT3_OS_2X                  2
#define MIKROE_ENVIRONMENT3_OS_4X                  3
#define MIKROE_ENVIRONMENT3_OS_8X                  4
#define MIKROE_ENVIRONMENT3_OS_16X                 5

/**
 * @brief Environment 3 IIR Filter settings.
 * @details IIR Filter settings.
 */
#define MIKROE_ENVIRONMENT3_FILTER_OFF             0
#define MIKROE_ENVIRONMENT3_FILTER_SIZE_1          1
#define MIKROE_ENVIRONMENT3_FILTER_SIZE_3          2
#define MIKROE_ENVIRONMENT3_FILTER_SIZE_7          3
#define MIKROE_ENVIRONMENT3_FILTER_SIZE_15         4
#define MIKROE_ENVIRONMENT3_FILTER_SIZE_31         5
#define MIKROE_ENVIRONMENT3_FILTER_SIZE_63         6
#define MIKROE_ENVIRONMENT3_FILTER_SIZE_127        7

/**
 * @brief Environment 3 ODR/Standby time macros.
 * @details ODR/Standby time macros.
 */
#define MIKROE_ENVIRONMENT3_ODR_0_59_MS            0
#define MIKROE_ENVIRONMENT3_ODR_62_5_MS            1
#define MIKROE_ENVIRONMENT3_ODR_125_MS             2
#define MIKROE_ENVIRONMENT3_ODR_250_MS             3
#define MIKROE_ENVIRONMENT3_ODR_500_MS             4
#define MIKROE_ENVIRONMENT3_ODR_1000_MS            5
#define MIKROE_ENVIRONMENT3_ODR_10_MS              6
#define MIKROE_ENVIRONMENT3_ODR_20_MS              7
#define MIKROE_ENVIRONMENT3_ODR_NONE               8

/**
 * @brief Environment 3 operating mode selection settings.
 * @details Operating mode selection setting.
 */
#define MIKROE_ENVIRONMENT3_MODE_SLEEP             0
#define MIKROE_ENVIRONMENT3_MODE_FORCED            1

/**
 * @brief Environment 3 SPI page settings.
 * @details SPI memory page setting.
 */
#define MIKROE_ENVIRONMENT3_MEM_PAGE_0             0x10
#define MIKROE_ENVIRONMENT3_MEM_PAGE_1             0x00

/**
 * @brief Environment 3 buffer length macros.
 * @details Buffer length.
 */
#define MIKROE_ENVIRONMENT3_LEN_COEFF_ALL          42
#define MIKROE_ENVIRONMENT3_LEN_COEFF_1            23
#define MIKROE_ENVIRONMENT3_LEN_COEFF_2            14
#define MIKROE_ENVIRONMENT3_LEN_COEFF_3            5
#define MIKROE_ENVIRONMENT3_LEN_FIELD              17
#define MIKROE_ENVIRONMENT3_LEN_CONFIG             5
#define MIKROE_ENVIRONMENT3_LEN_INTERLEAVE_BUFF    20

/**
 * @brief Environment 3 coefficient index macros.
 * @details Coefficient index setting.
 */
#define MIKROE_ENVIRONMENT3_IDX_T2_LSB             0
#define MIKROE_ENVIRONMENT3_IDX_T2_MSB             1
#define MIKROE_ENVIRONMENT3_IDX_T3                 2
#define MIKROE_ENVIRONMENT3_IDX_P1_LSB             4
#define MIKROE_ENVIRONMENT3_IDX_P1_MSB             5
#define MIKROE_ENVIRONMENT3_IDX_P2_LSB             6
#define MIKROE_ENVIRONMENT3_IDX_P2_MSB             7
#define MIKROE_ENVIRONMENT3_IDX_P3                 8
#define MIKROE_ENVIRONMENT3_IDX_P4_LSB             10
#define MIKROE_ENVIRONMENT3_IDX_P4_MSB             11
#define MIKROE_ENVIRONMENT3_IDX_P5_LSB             12
#define MIKROE_ENVIRONMENT3_IDX_P5_MSB             13
#define MIKROE_ENVIRONMENT3_IDX_P7                 14
#define MIKROE_ENVIRONMENT3_IDX_P6                 15
#define MIKROE_ENVIRONMENT3_IDX_P8_LSB             18
#define MIKROE_ENVIRONMENT3_IDX_P8_MSB             19
#define MIKROE_ENVIRONMENT3_IDX_P9_LSB             20
#define MIKROE_ENVIRONMENT3_IDX_P9_MSB             21
#define MIKROE_ENVIRONMENT3_IDX_P10                22
#define MIKROE_ENVIRONMENT3_IDX_H2_MSB             23
#define MIKROE_ENVIRONMENT3_IDX_H2_LSB             24
#define MIKROE_ENVIRONMENT3_IDX_H1_LSB             24
#define MIKROE_ENVIRONMENT3_IDX_H1_MSB             25
#define MIKROE_ENVIRONMENT3_IDX_H3                 26
#define MIKROE_ENVIRONMENT3_IDX_H4                 27
#define MIKROE_ENVIRONMENT3_IDX_H5                 28
#define MIKROE_ENVIRONMENT3_IDX_H6                 29
#define MIKROE_ENVIRONMENT3_IDX_H7                 30
#define MIKROE_ENVIRONMENT3_IDX_T1_LSB             31
#define MIKROE_ENVIRONMENT3_IDX_T1_MSB             32
#define MIKROE_ENVIRONMENT3_IDX_GH2_LSB            33
#define MIKROE_ENVIRONMENT3_IDX_GH2_MSB            34
#define MIKROE_ENVIRONMENT3_IDX_GH1                35
#define MIKROE_ENVIRONMENT3_IDX_GH3                36
#define MIKROE_ENVIRONMENT3_IDX_RES_HEAT_VAL       37
#define MIKROE_ENVIRONMENT3_IDX_RES_HEAT_RANGE     39
#define MIKROE_ENVIRONMENT3_IDX_RANGE_SW_ERR       41

/**
 * @brief Environment 3 device address setting.
 * @details Specified setting for device slave address selection of
 * Environment 3 Click driver.
 */
#define MIKROE_ENVIRONMENT3_DEVICE_SLAVE_ADDR_0    0x76
#define MIKROE_ENVIRONMENT3_DEVICE_SLAVE_ADDR_1    0x77

/**
 * @brief Environment 3 field data structure.
 * @details Field data structure definition.
 */
typedef struct
{
  uint8_t status;
  uint8_t gas_index;
  uint8_t meas_index;
  uint8_t res_heat;
  uint8_t idac;
  uint8_t gas_wait;
  float temperature;
  float pressure;
  float humidity;
  float gas_resistance;
} mikroe_environment3_field_data_t;

/**
 * @brief Environment 3 calibration data structure.
 * @details Calibration data structure definition.
 */
typedef struct
{
  uint16_t par_h1;
  uint16_t par_h2;
  int8_t par_h3;
  int8_t par_h4;
  int8_t par_h5;
  uint8_t par_h6;
  int8_t par_h7;
  int8_t par_gh1;
  int16_t par_gh2;
  int8_t par_gh3;
  uint16_t par_t1;
  int16_t par_t2;
  int8_t par_t3;
  uint16_t par_p1;
  int16_t par_p2;
  int8_t par_p3;
  int16_t par_p4;
  int16_t par_p5;
  int8_t par_p6;
  int8_t par_p7;
  int16_t par_p8;
  int16_t par_p9;
  uint8_t par_p10;
  float t_fine;
  uint8_t res_heat_range;
  int8_t res_heat_val;
  int8_t range_sw_err;
} mikroe_environment3_calibration_data_t;

/**
 * @brief Environment 3 TPH settings structure.
 * @details Temperature, pressure, humidity settings structure definition.
 */
typedef struct
{
  uint8_t os_hum;
  uint8_t os_temp;
  uint8_t os_pres;
  uint8_t filter;
  uint8_t odr;
} mikroe_environment3_tph_settings_t;

/**
 * @brief Environment 3 gas settings structure.
 * @details gas settings structure definition.
 */
typedef struct
{
  uint8_t enable;
  uint16_t heater_temp;
  uint16_t heater_dur;
} mikroe_environment3_gas_settings_t;

/** \} */ // End types group

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
sl_status_t mikroe_environment3_init(sl_i2cspm_t *i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    Environment 3 data writing function.
 *
 * @param[in] reg
 *    Start register address.
 *
 * @param[in] data_in
 *    Data to be written.
 *
 * @param[in] len
 *    Number of bytes to be written.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL if write failed.
 ******************************************************************************/
sl_status_t mikroe_environment3_generic_write(uint8_t reg,
                                              uint8_t *data_in,
                                              uint8_t len);

/***************************************************************************//**
 * @brief
 *    Environment 3 data reading function.
 *
 * @param[in] reg
 *    Start register address.
 *
 * @param[out] data_out
 *    Output read data.
 *
 * @param[in] len
 *    Number of bytes to be read.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL if read failed.
 ******************************************************************************/
sl_status_t mikroe_environment3_generic_read(uint8_t reg,
                                             uint8_t *data_out,
                                             uint8_t len);

/***************************************************************************//**
 * @brief
 *    Environment 3 single data writing function.
 *
 * @param[in] reg
 *    Register address.
 *
 * @param[in] data_in
 *    Data to be written.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL if write failed.
 ******************************************************************************/
sl_status_t mikroe_environment3_write_byte(uint8_t reg,
                                           uint8_t data_in);

/***************************************************************************//**
 * @brief
 *    Environment 3 single data reading function.
 *
 * @param[in] reg
 *    Start register address.
 *
 * @return
 *    Read data byte.
 ******************************************************************************/
uint8_t mikroe_environment3_read_byte(uint8_t reg);

/***************************************************************************//**
 * @brief
 *    Environment 3 check device ID function.
 *
 * @return
 *    SL_STATUS_OK if ID correct,
 *    SL_STATUS_FAIL if Wrong ID read.
 ******************************************************************************/
sl_status_t mikroe_environment3_check_id(void);

/***************************************************************************//**
 * @brief
 *    Environment 3 get device ID function.
 *
 * @return
 *    Device ID.
 ******************************************************************************/
uint8_t mikroe_environment3_get_device_id(void);

/***************************************************************************//**
 * @brief
 *    Environment 3 soft reset function.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL if fails.
 ******************************************************************************/
sl_status_t mikroe_environment3_soft_reset(void);

/***************************************************************************//**
 * @brief
 *    Environment 3 get all data function.
 *
 * @param[out] temp
 *    Temperature value in Celsius.
 *
 * @param[out] hum
 *    Temperature value in Percents.
 *
 * @param[out] pres
 *    Pressure value in mBar.
 *
 * @param[out] gas
 *    Gas resistance value in Ohms.
 *
 * @return
 *    2 - No new data.
 *    0 - Success,
 *   -1 - Error.
 ******************************************************************************/
int8_t mikroe_environment3_get_all_data(float *temp,
                                        float *hum,
                                        float *pres,
                                        uint32_t *gas);

/***************************************************************************//**
 * @brief
 *    Environment 3 get temperature data function.
 *
 * @return
 *    2 - No new data.
 *   -1 - Error.
 *   other: Temperature value in Celsius.
 ******************************************************************************/
float mikroe_environment3_get_temperature(void);

/***************************************************************************//**
 * @brief
 *    Environment 3 get humidity data function.
 *
 * @return
 *    2 - No new data.
 *   -1 - Error.
 *   other: Humidity value in Percents.
 ******************************************************************************/
float mikroe_environment3_get_humidity(void);

/***************************************************************************//**
 * @brief
 *    Environment 3 get pressure data function.
 *
 * @return
 *    2 - No new data.
 *   -1 - Error.
 *   other: Pressure value in mBar.
 ******************************************************************************/
float mikroe_environment3_get_pressure(void);

/***************************************************************************//**
 * @brief
 *    Environment 3 get gas resistance data function.
 *
 * @return
 *    2 - No new data.
 *   -1 - Error.
 *   other: Gas resistance value in Ohms.
 ******************************************************************************/
float mikroe_environment3_get_gas_resistance(void);

/***************************************************************************//**
 * @brief
 *    Environment 3 read data from register address function.
 *
 * @param[in] reg
 *    Start register address.
 *
 * @param[out] data_out
 *    Output read data.
 *
 * @param[out] pres
 *    Pressure value in mBar.
 *
 * @param[in] len
 *    Number of bytes to be read.
 *
 * @return
 *   -2 - Communication fail.
 *    0 - Success,
 *   -1 - Error.
 ******************************************************************************/
int8_t mikroe_environment3_get_regs(uint8_t reg, uint8_t *data_out,
                                    uint16_t len);

/***************************************************************************//**
 * @brief
 *    Environment 3 write data to registers function.
 *
 * @param[in] reg
 *    Array of registers addresses.
 *
 * @param[in] data_in
 *    Array of data to be written.
 *
 * @param[in] len
 *    Number of bytes to be written.
 *
 * @return
 *   -3 - Invalid length.
 *   -2 - Communication fail.
 *    0 - Success,
 *   -1 - Error.
 ******************************************************************************/
int8_t mikroe_environment3_set_regs(uint8_t *reg, uint8_t *data_in,
                                    uint8_t len);

/***************************************************************************//**
 * @brief
 *    Environment 3 enable heater function.
 *
 * @param[in] state
 *    - Enable, 0 - Disable.
 *
 * @return
 *   -2 - Communication fail.
 *    0 - Success,
 *   -1 - Error.
 ******************************************************************************/
int8_t mikroe_environment3_enable_heater(uint8_t state);

/***************************************************************************//**
 * @brief
 *    Environment 3 set heater configuration function.
 *
 * @param[in] conf
 *    Gas setting object.
 *
 *    See #mikroe_environment3_gas_settings_t gas setting object definition for
 *   detailed explanation.
 *
 * @return
 *   -2 - Communication fail.
 *    0 - Success,
 *   -1 - Error.
 ******************************************************************************/
int8_t mikroe_environment3_set_heater_conf(
  mikroe_environment3_gas_settings_t *conf);

/***************************************************************************//**
 * @brief
 *    Environment 3 set sensor configuration function.
 *
 * @param[in] conf
 *    THP setting object.
 *
 *    See #mikroe_environment3_tph_settings_t TPH setting object definition for
 *   detailed explanation.
 *
 * @return
 *   -2 - Communication fail.
 *    0 - Success,
 *   -1 - Error.
 ******************************************************************************/
int8_t mikroe_environment3_set_sensor_conf(
  mikroe_environment3_tph_settings_t *conf);

/***************************************************************************//**
 * @brief
 *    Environment 3 set operating mode function.
 *
 * @param[in] op_mode
 *    0 - SLEEP, 1 - FORCED.
 *
 * @return
 *   -2 - Communication fail.
 *    0 - Success,
 *   -1 - Error.
 ******************************************************************************/
int8_t mikroe_environment3_set_operating_mode(uint8_t op_mode);

/***************************************************************************//**
 * @brief
 *    Environment 3 get operating mode function.
 *
 * @return
 *   -1 - Error.
 *   0 - SLEEP, 1 - FORCED.
 ******************************************************************************/
int8_t mikroe_environment3_get_operating_mode(void);

#endif /* MIKROE_BME688_I2C_H_ */
