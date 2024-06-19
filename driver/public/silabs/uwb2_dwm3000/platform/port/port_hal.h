/**
 * @file      port_hal.h
 *
 * @brief     port Hardware Abstraction Layer headers file
 *
 * @author    Decawave
 *
 * @attention Copyright 2017-2019 (c) Decawave Ltd, Dublin, Ireland.
 *            All rights reserved.
 *
 */

#include "uwb2_dwm3000_config.h"

#define _HAL_CAT2EXP(a, b)           a ## b
#define HAL_CAT2(a, b)               _HAL_CAT2EXP(a, b)
#define _HAL_CAT3EXP(a, b, c)        a ## b ## c
#define HAL_CAT3(a, b, c)            _HAL_CAT3EXP(a, b, c)

// UWB/DW3000
#define HAL_UWB_IRQ_PORT              DWM3000_INT_PORT
#define HAL_UWB_IRQ_PIN               DWM3000_INT_PIN
#define HAL_UWB_IRQ_IT_NBR            DWM3000_INT_PIN

#define HAL_UWB_RST_PORT              DWM3000_RESET_PORT
#define HAL_UWB_RST_PIN               DWM3000_RESET_PIN
#define HAL_UWB_RST_IT_NBR            DWM3000_RESET_PIN

#if defined(DWM3000_WAKE_PORT) && defined(DWM3000_WAKE_PIN)
#define HAL_UWB_WAKEUP_PORT           DWM3000_WAKE_PORT
#define HAL_UWB_WAKEUP_PIN            DWM3000_WAKE_PIN
#define HAL_UWB_WAKEUP_IT_NBR         DWM3000_WAKE_PIN
#endif

#define HAL_UWB_EXTON_PORT            DWM3000_ON_PORT
#define HAL_UWB_EXTON_PIN             DWM3000_ON_PIN
#define HAL_UWB_EXTON_IT_NBR          DWM3000_ON_PIN

// SPI
#define HAL_SPI_TYPE                  DWM3000_SPI_TYPE
#define HAL_SPI_NUMBER                DWM3000_SPI_NUMBER
#define HAL_SPI_PERIPHERAL            HAL_CAT2(HAL_SPI_TYPE, HAL_SPI_NUMBER)

#define HAL_SPI_CS_PORT               DWM3000_CS_PORT
#define HAL_SPI_CS_PIN                DWM3000_CS_PIN

#define HAL_SPI_CLK_PORT              DWM3000_CLK_PORT
#define HAL_SPI_CLK_PIN               DWM3000_CLK_PIN

#define HAL_SPI_MISO_PORT             DWM3000_MISO_PORT
#define HAL_SPI_MISO_PIN              DWM3000_MISO_PIN

#define HAL_SPI_MOSI_PORT             DWM3000_MOSI_PORT
#define HAL_SPI_MOSI_PIN              DWM3000_MOSI_PIN

#define HAL_SPI_FREQ_MIN              DWM3000_FREQ_MIN
#define HAL_SPI_FREQ_MAX              DWM3000_FREQ_MAX
