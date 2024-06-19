/**-
 * @file    deca_sleep.c
 *
 * @brief   platform dependent sleep implementation
 *
 * @attention
 *
 * Copyright 2015 (c) Decawave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author Decawave
 */

#include "port.h"
#include "sl_udelay.h"

/* Wrapper function to be used by decadriver. Declared in deca_device_api.h */
void deca_sleep(unsigned int time_ms)
{
  sl_udelay_wait(time_ms * 1000);
}

/* Wrapper function to be used by decadriver. Declared in deca_device_api.h */
void deca_usleep(unsigned long time_us)
{
  sl_udelay_wait(time_us);
}
