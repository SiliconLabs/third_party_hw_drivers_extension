/***************************************************************************//**
 * @file system.h
 * @brief Low-level support functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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

#ifndef SYSTEM__H
#define SYSTEM__H

#include <em_emu.h>

#define likely(x) \
  __builtin_expect((x), 1)
#define unlikely(x) \
  __builtin_expect((x), 0)

#define READ_ONCE(var) \
  (*(const volatile typeof(var) *) & (var))

#define WRITE_ONCE(var, val)                   \
  do                                           \
  {                                            \
    *(volatile typeof(var) *) & (var) = (val); \
  } while (0)

#define __CORE_ENTER_CRITICAL() \
  {                             \
    irqState = __get_PRIMASK(); \
    __disable_irq();            \
  }

#define __CORE_EXIT_CRITICAL() \
  {                            \
    if (irqState == 0U)        \
    {                          \
      __enable_irq();          \
    }                          \
  }

#define __CORE_YIELD_CRITICAL()       \
  {                                   \
    if ((__get_PRIMASK() & 1U) != 0U) \
    {                                 \
      __enable_irq();                 \
      __ISB();                        \
      __disable_irq();                \
    }                                 \
  }

#endif // __SYSTEM_H
