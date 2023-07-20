/***************************************************************************//**
 * @file mirkroe_mic2.h
 * @brief SCL ad8318 Prototypes
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

#ifndef MIKROE_MIC2_H_
#define MIKROE_MIC2_H_

#include "em_cmu.h"
#include "sl_i2cspm.h"
#include "sl_status.h"

#if defined(_SILICON_LABS_32B_SERIES_1)
#include "em_adc.h"
typedef ADC_TypeDef adc_t;
#elif defined(_SILICON_LABS_32B_SERIES_2)
#include "em_iadc.h"
typedef IADC_TypeDef adc_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Analog data type
 */
typedef uint16_t mikroe_mic2_data_t;

/**
 * @brief Config Object Initialization function.
 *
 * @description This function initializes click configuration structure to init
 *   state.
 * @note All used pins will be set to unconnected state.
 */
void mikroe_mic2_cfg_setup(void);

/**
 * @brief Set I2c instance function.
 *
 * @param i2cspm_instance I2C handle instance.
 *
 * @return status of function.
 * @description This function initializes all necessary pins and peripherals
 *   used for this click.
 */
sl_status_t mikroe_mic2_set_i2cspm_instance(sl_i2cspm_t *i2cspm_instance);

/**
 * @brief Initialization function.
 *
 * @param i2cspm_instance I2C handle instance
 * @param adc ADC or IADC handle
 * @return status of function.
 *
 * @description This function initializes all necessary pins and peripherals
 *   used for this click.
 */
sl_status_t mikroe_mic2_init(sl_i2cspm_t *i2cspm_instance,
                             adc_t *adc);

/**
 * @brief Set potenciometer value.
 *
 * @param ptt_value  potentiometer value.
 *
 * @description This function set the value of digital potencionetar.
 */
void mikroe_mic2_set_potentiometer(uint8_t ptt_value);

/**
 * @brief Generic read function.
 *
 * @param data data store adc value.
 * @return status of function.
 *
 * @description This function read ADC data.
 */
sl_status_t mikroe_mic2_generic_read(mikroe_mic2_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_MIC2_H_ */
