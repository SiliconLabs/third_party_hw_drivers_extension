/***************************************************************************//**
 * @file dma_master_s2.c
 * @brief DMA callback handling
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

#include "dma_master.h"

#include <em_core.h>
#include <em_cmu.h>
#include <em_device.h>
#include <em_ldma.h>

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
  ldma_callback_t cb;
  void *param;
} ldma_callback_entry_t;

static ldma_callback_entry_t ldma_callbacks[DMA_CHAN_COUNT];

void dma_master_init(void)
{
  LDMA_Init_t ldma = LDMA_INIT_DEFAULT;

  CMU_ClockEnable(cmuClock_LDMA, true);
  CMU_ClockEnable(cmuClock_LDMAXBAR, true);

  ldma.ldmaInitIrqPriority = 0;
  LDMA_Init(&ldma);
}

void LDMA_IRQHandler(void)
{
  /* Get all pending and enabled interrupts. */
  uint32_t pending = LDMA_IntGetEnabled();

  /* Loop on an LDMA error to enable debugging. */
  if (pending & LDMA_IF_ERROR) {
    while (1) {}
  }

  /* Iterate over all LDMA channels. */
  while (pending)
  {
    uint32_t ch, mask;

    ch = __builtin_ffs(pending) - 1;
    mask = 0x1 << ch;
    pending &= ~mask;

    LDMA_IntClear(mask);

    /* Perform more actions here, execute callbacks, and so on. */
    if (ldma_callbacks[ch].cb) {
      ldma_callbacks[ch].cb(ch, ldma_callbacks[ch].param);
    }
  }
}

void LDMA_RegisterCallback(int ch, ldma_callback_t callback, void *param)
{
  ldma_callbacks[ch].cb = callback;
  ldma_callbacks[ch].param = param;
}

void LDMA_UnregisterCallback(int ch)
{
  ldma_callbacks[ch].cb = NULL;
  ldma_callbacks[ch].param = NULL;
}

void LDMA_UpdateCallbackParam(int ch, void *param)
{
  ldma_callbacks[ch].param = param;
}
