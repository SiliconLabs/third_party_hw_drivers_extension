/***************************************************************************//**
 * @file sparkfun_ak9753_config.h
 * @brief AK9753 Configuration
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
 *    misrepresented as being the original software.s
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

#ifndef SPARKFUN_AK9753_CONFIG_H_
#define SPARKFUN_AK9753_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup ak9753_config AK9753 Configuration */

/***************************************************************************//**
 * @addtogroup ak9753_config
 * @brief  AK9753 Configuration
 * @details
 * @{
 ******************************************************************************/

/************************************************/
/**\name    I2C ADDRESS DEFINITION              */
/************************************************/
#define Address1    0x64
#define Address2    0x65
#define Address3    0x66

/************************************************/
/* Address is changeable via CAD0/CAD1          */
/* Allowed settings are:                        */
/* 00 (0x64 default)                            */
/* 01 (0x65)                                    */
/* 10 (0x66)                                    */
/* 11 Not allowed - used for switch mode        */
/************************************************/

// <<< Use Configuration Wizard in Context Menu >>>

// <h>Address settings

// <o SPARKFUN_AK9753_ADDR> Address of AK9753 sensor.
// <Address1=> 0x64
// <Address2=> 0x65
// <Address3=> 0x66
// <i> Default: Address1
#define SPARKFUN_AK9753_ADDR             Address1

// </h>

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

/*
 * GPIO port/pin used for INT interrupt pin. This should typically
 * be a port A/B pin so that it can detect a falling edge while in
 * EM2/3. If the application firmware does not need to use EM2/3,
 * then a port C/D pin can be used.
 */
// <gpio optional=true> SPARKFUN_AK9753_CONFIG_INT
// $[SPARKFUN_AK9753_CONFIG_INT]
#define SPARKFUN_AK9753_CONFIG_INT_PORT   gpioPortB
#define SPARKFUN_AK9753_CONFIG_INT_PIN    0
// [SPARKFUN_AK9753_CONFIG_INT]$

/*
 * GPIO port/pin used for PDN power down output pin.
 */
// <gpio optional=true> SPARKFUN_AK9753_CONFIG_PDN
// $[SPARKFUN_AK9753_CONFIG_PDN]
#define SPARKFUN_AK9753_CONFIG_PDN_PORT   gpioPortB
#define SPARKFUN_AK9753_CONFIG_PDN_PIN    1
// [SPARKFUN_AK9753_CONFIG_PDN]$

// <<< sl:end pin_tool >>>

/** @} (end addtogroup ak9753_config) */

#ifdef __cplusplus
}
#endif

#endif /* SPARKFUN_AK9753_CONFIG_H_ */
