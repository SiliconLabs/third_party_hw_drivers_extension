/***************************************************************************//**
* @file sparkfun_mlx90640_config.h
* @brief MLX90640 Driver Configuration
********************************************************************************
* # License
* <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
********************************************************************************
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
*******************************************************************************/
#ifndef SPARKFUN_MLX90640_CONFIG_H_
#define SPARKFUN_MLX90640_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>
// <h> Connection
// <o MLX90640_I2C_ADDRESS> I2C Address <0x1..0x7E:0x1><f.h>
// <i> Default: 0x33
#define MLX90640_I2C_ADDRESS                        0x33
// </h>

// <h> User-Settings

// <q MLX90640_CONFIG_ENABLE_LOG> Enable log
// <i> Default: 0
#define MLX90640_CONFIG_ENABLE_LOG                  0

// </h>
// <<< end of configuration section >>>

#ifdef __cplusplus
}
#endif

#endif /* SPARKFUN_MLX90640_CONFIG_H_ */
