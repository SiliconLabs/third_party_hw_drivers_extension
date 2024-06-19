/**
 * @file      deca_spi.c
 *
 * @brief     SPI functions to interface to DW3000 chip's from EFR32.
 *
 * @author    Decawave
 *
 * @attention Copyright 2017-2019 (c) Decawave Ltd, Dublin, Ireland.
 *            All rights reserved.
 *
 */

#include "port.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_eusart.h"
#include "em_usart.h"
#include "spidrv.h"
#if defined (SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif
#include "deca_spi.h"
#include <deca_device_api.h>

#ifndef SPI_USE_SPIDRV_API
#define SPI_USE_SPIDRV_API     0
#endif
#define SPI_GPIO_SLOW_SLEWRATE 5
#define SPI_GPIO_FAST_SLEWRATE 7
#define SPI_DUMMY_TX_VALUE     0xFF

#define EUSART_SpiTransfer     EUSART_Spi_TxRx
#define spi_tx_rx(data) HAL_CAT2(HAL_SPI_TYPE, _SpiTransfer)(HAL_SPI_PERIPHERAL, \
                                                             data)

static SPIDRV_HandleData_t spi_driver_handle;
static volatile bool spi_lock;

/*!
 *
 * -----------------------------------------------------------------------------
 * Function: openspi()
 *
 * Low level abstract function to open and initialise access to the SPI device.
 * returns 0 for success, or -1 for error
 */
int openspi(void)
{
  SPIDRV_Init_t spi_init = {
    .port = HAL_SPI_PERIPHERAL,
    .portTx = HAL_SPI_MOSI_PORT,
    .portRx = HAL_SPI_MISO_PORT,
    .portClk = HAL_SPI_CLK_PORT,
    .portCs = HAL_SPI_CS_PORT,
    .pinTx = HAL_SPI_MOSI_PIN,
    .pinRx = HAL_SPI_MISO_PIN,
    .pinClk = HAL_SPI_CLK_PIN,
    .pinCs = HAL_SPI_CS_PIN,
    .bitRate = HAL_SPI_FREQ_MIN,
    .frameLength = 8,
    .dummyTxValue = SPI_DUMMY_TX_VALUE,
    .type = spidrvMaster,
    .bitOrder = spidrvBitOrderMsbFirst,
    .clockMode = spidrvClockMode0,
    .csControl = spidrvCsControlApplication,
  };

  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_SlewrateSet(spi_init.portClk,
                   SPI_GPIO_SLOW_SLEWRATE,
                   SPI_GPIO_SLOW_SLEWRATE);
  GPIO_SlewrateSet(spi_init.portTx,
                   SPI_GPIO_SLOW_SLEWRATE,
                   SPI_GPIO_SLOW_SLEWRATE);
  GPIO_PinModeSet(spi_init.portCs, spi_init.pinCs, gpioModePushPull, 1);
  return SPIDRV_Init(&spi_driver_handle, &spi_init);
}

/*!
 *
 * -----------------------------------------------------------------------------
 * Function: closespi()
 *
 * Low level abstract function to close the the SPI device.
 * returns 0 for success, or -1 for error
 */
int closespi(void)
{
  return SPIDRV_DeInit(&spi_driver_handle);
}

static void port_spi_set_bitrate(uint32_t goal_bitrate)
{
  uint32_t required_slewrate =
    (goal_bitrate
     >= 10000000) ? SPI_GPIO_FAST_SLEWRATE : SPI_GPIO_SLOW_SLEWRATE;
  GPIO_SlewrateSet(spi_driver_handle.initData.portClk,
                   required_slewrate,
                   required_slewrate);
  GPIO_SlewrateSet(spi_driver_handle.initData.portTx,
                   required_slewrate,
                   required_slewrate);
  SPIDRV_SetBitrate(&spi_driver_handle, goal_bitrate);
}

/* @fn  set_dw_spi_slow_rate
 * @brief sets slow SPI clock speed for the DW chip
 *        left for compatibility.
 * */
void set_dw_spi_slow_rate(void)
{
  port_spi_set_bitrate(HAL_SPI_FREQ_MIN);
}

/* @fn      set_dw_spi_fast_rate
 * @brief   sets High SPI clock speed for the DW chip
 * */
void set_dw_spi_fast_rate(void)
{
  port_spi_set_bitrate(HAL_SPI_FREQ_MAX);
}

static inline void spi_transfer_open(void)
{
  // TODO: not thread safe at all... (but same as other Vendors' implementation)
  while (spi_lock) {}
  spi_lock = true;

#if (!SPI_USE_SPIDRV_API) && defined (SL_CATALOG_POWER_MANAGER_PRESENT)
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
#endif
  GPIO_PinOutClear(spi_driver_handle.initData.portCs,
                   spi_driver_handle.initData.pinCs);
}

static inline void spi_transfer_close(void)
{
  GPIO_PinOutSet(spi_driver_handle.initData.portCs,
                 spi_driver_handle.initData.pinCs);
#if (!SPI_USE_SPIDRV_API) && defined (SL_CATALOG_POWER_MANAGER_PRESENT)
  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
#endif
  spi_lock = false;
}

static inline Ecode_t spi_blocking_transmit(const uint8_t *buffer, int count)
{
#if SPI_USE_SPIDRV_API
  return SPIDRV_MTransmitB(&spi_driver_handle, buffer, count);
#else
  for (int i = 0; i < count; i++) {
    spi_tx_rx(buffer[i]);
  }
  return ECODE_OK;
#endif
}

static inline Ecode_t spi_blocking_receive(uint8_t *buffer, int count)
{
#if SPI_USE_SPIDRV_API
  return SPIDRV_MReceiveB(&spi_driver_handle, buffer, count);
#else
  for (int i = 0; i < count; i++) {
    buffer[i] = spi_tx_rx(SPI_DUMMY_TX_VALUE);
  }
  return ECODE_OK;
#endif
}

int readfromspi(uint16_t headerLength,
                uint8_t *headerBuffer,
                uint16_t readLength,
                uint8_t *readBuffer)
{
  Ecode_t sc = 0;

  spi_transfer_open();
  if (headerBuffer && headerLength) {
    sc |= spi_blocking_transmit(headerBuffer, headerLength);
  }
  if (!sc && readBuffer && readLength) {
    sc |= spi_blocking_receive(readBuffer, readLength);
  }
  spi_transfer_close();

  return sc;
}

static int spi_write(uint16_t headerLength,
                     const uint8_t *headerBuffer,
                     uint16_t bodyLength,
                     const uint8_t *bodyBuffer,
                     uint8_t *crc8)
{
  Ecode_t sc = 0;

  spi_transfer_open();
  if (headerBuffer && headerLength) {
    sc |= spi_blocking_transmit(headerBuffer, headerLength);
  }
  if (!sc && bodyBuffer && bodyLength) {
    sc |= spi_blocking_transmit(bodyBuffer, bodyLength);
  }
  if (crc8) {
    sc |= spi_blocking_transmit(crc8, sizeof(*crc8));
  }
  spi_transfer_close();

  return sc;
}

int writetospi(uint16_t headerLength,
               const uint8_t *headerBuffer,
               uint16_t bodyLength,
               const uint8_t *bodyBuffer)
{
  return spi_write(headerLength, headerBuffer, bodyLength, bodyBuffer, NULL);
}

int writetospiwithcrc(uint16_t headerLength,
                      const uint8_t *headerBuffer,
                      uint16_t bodyLength,
                      const uint8_t *bodyBuffer,
                      uint8_t crc8)
{
  return spi_write(headerLength, headerBuffer, bodyLength, bodyBuffer, &crc8);
}
