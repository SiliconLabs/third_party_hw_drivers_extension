/***************************************************************************//**
 * @file drv_pwm.h
 * @brief mikroSDK 2.0 Click Peripheral Drivers
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

#ifndef _DRV_PWM_H_
#define _DRV_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "drv_name.h"
#include "hal_gpio.h"

typedef const void *mikroe_pwm_handle_t; ///< Created PWM handle type

typedef enum {
  PWM_SUCCESS = 0, PWM_ERROR = (-1)
} pwm_err_t;

typedef struct {
  pin_name_t pin;
  uint32_t freq_hz;
} pwm_config_t;

typedef struct {
  mikroe_pwm_handle_t handle;
  hal_gpio_t pin;
  pwm_config_t config;
} pwm_t;

void pwm_configure_default(pwm_config_t *config);
err_t pwm_open(pwm_t *obj, pwm_config_t *config);
err_t pwm_set_freq(pwm_t *obj, uint32_t freq_hz);
err_t pwm_start(pwm_t *obj);
err_t pwm_set_duty(pwm_t *obj, float duty_ratio);
err_t pwm_stop(pwm_t *obj);
void pwm_close(pwm_t *obj);

#ifdef __cplusplus
}
#endif

#endif // _DRV_PWM_H_
// ------------------------------------------------------------------------- END
