/**
 * @file      port.h
 *
 * @brief     port headers file to EFR32BG22
 *
 * @author    Decawave
 *
 * @attention Copyright 2017-2019 (c) Decawave Ltd, Dublin, Ireland.
 *            All rights reserved.
 *
 */

#ifndef PORT__H_
#define PORT__H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "error.h"
#include "port_hal.h"
#include "deca_spi.h"
#include "deca_device_api.h"

/* port functions prototypes */
error_e port_init_dw_chip(void);
error_e port_wakeup_dw3000_fast(void);
error_e port_wakeup_dw3000(void);
void wakeup_device_with_io(void);

void dw_irq_init(void);
void init_dw3000_irq(void);
void enable_dw3000_irq(void);
void disable_dw3000_irq(void);
void reset_DW3000(void);

void port_stop_all_UWB(void);

error_e port_disable_dw_irq_and_reset(int reset);

#ifdef __cplusplus
}
#endif

#endif /* PORT__H__ */
