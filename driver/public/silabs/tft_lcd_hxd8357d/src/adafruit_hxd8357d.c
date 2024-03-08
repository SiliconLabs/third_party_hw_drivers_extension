/***************************************************************************//**
 * @file adafruit_hxd8357d.c
 * @brief Adafruit HXD8357D TFT LCD with Touchscreen Source File
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
// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include "em_cmu.h"
#include "em_gpio.h"
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "sl_component_catalog.h"
#include "em_usart.h"
#include "adafruit_hxd8357d_spi_usart_config.h"
#include "adafruit_hxd8357d.h"

/******************************************************************************
*                       Macros
******************************************************************************/

#define MADCTL_MY  0x80 ///< Bottom to top
#define MADCTL_MX  0x40 ///< Right to left
#define MADCTL_MV  0x20 ///< Reverse Mode
#define MADCTL_ML  0x10 ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH  0x04 ///< LCD refresh right to left

#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))

#define SPI_CS_LOW() \
  GPIO_PinOutClear(ADAFRUIT_HXD8357D_CS_PORT, ADAFRUIT_HXD8357D_CS_PIN);
#define SPI_CS_HIGH() \
  GPIO_PinOutSet(ADAFRUIT_HXD8357D_CS_PORT, ADAFRUIT_HXD8357D_CS_PIN);

#define SPI_DC_LOW() \
  GPIO_PinOutClear(HXD8357D_SPI_DC_PORT, HXD8357D_SPI_DC_PIN);
#define SPI_DC_HIGH() \
  GPIO_PinOutSet(HXD8357D_SPI_DC_PORT, HXD8357D_SPI_DC_PIN);

/******************************************************************************
*                       Static variables
******************************************************************************/
static int _width;  // screen dimensions
static int _height;

/***************************************************************************//**
 * @brief
 *  Adafruit_SPITFT Send Command handles complete sending of commands and
 *  data.
 *
 * @param[in] command
 *  The Command Byte.
 * @param[in] data
 *  A pointer to the Data bytes to send.
 * @param[in] len
 *  The number of bytes we should send.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_INVALID_PARAMETER if NULL pointer or 'len' equals to zero.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_spi_send_command(uint8_t command,
                                                      uint8_t *data,
                                                      uint8_t len)
{
  SPI_CS_LOW();
  SPI_DC_LOW();
  USART_SpiTransfer(ADAFRUIT_HXD8357D_PERIPHERAL, command);
  SPI_DC_HIGH();
  while (len--) {
    USART_SpiTransfer(ADAFRUIT_HXD8357D_PERIPHERAL, *data);
    data++;
  }
  SPI_CS_HIGH();

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Write a single command byte to the display.
 *
 * @param[in] cmd
 *  8-bit command to write.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_spi_write_command(uint8_t cmd)
{
  SPI_DC_LOW();
  USART_SpiTransfer(ADAFRUIT_HXD8357D_PERIPHERAL, cmd);
  SPI_DC_HIGH();

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Write a single command word to the display.
 *
 * @param[in] cmd
 *  16-bit command to write.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_spi_write16(uint16_t data)
{
  uint8_t bytes[2];

  bytes[0] = data >> 8;
  bytes[1] = data & 0x00FF;

  USART_SpiTransfer(ADAFRUIT_HXD8357D_PERIPHERAL, bytes[0]);
  USART_SpiTransfer(ADAFRUIT_HXD8357D_PERIPHERAL, bytes[1]);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Issue a series of pixels, all the same color.
 *
 * @param[in] color
 *  16-bit pixel color in '565' RGB format.
 * @param[in] len
 *  Number of pixels to draw.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_write_color(uint16_t color, uint32_t len)
{
  while (len--) {
    adafruit_hxd8357d_spi_write16(color);
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Issue a series of pixels from memory to the display.
 *
 * @param[in] colors
 *  Pointer to array of 16-bit pixel values in '565' RGB format.
 * @param[in] len
 *  Number of elements in 'colors' array.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_INVALID_PARAMETER if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_write_pixels(uint16_t *colors,
                                                  uint32_t len)
{
  if ((!len) || (colors == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  while (len--) {
    adafruit_hxd8357d_spi_write16(*colors);
    colors++;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  A lower-level version of adafruit_hxd8357d_fill_rect(). This version
 *  requires all inputs are in-bounds, that width and height are positive,
 *  and no part extends offscreen. NO EDGE CLIPPING OR REJECTION IS PERFORMED.
 *  If higher-level graphics primitives are written to handle their own
 *  clipping earlier in the drawing process, this can avoid unnecessary
 *  function calls and repeated clipping operations in the lower-level
 *  functions.
 *
 * @param[in] x
 *  Horizontal position of first corner. MUST BE WITHIN SCREEN BOUNDS.
 * @param[in] y
 *  Vertical position of first corner. MUST BE WITHIN SCREEN BOUNDS.
 * @param[in] w
 *  Rectangle width in pixels. MUST BE POSITIVE AND NOT EXTEND OFF SCREEN.
 * @param[in] h
 *  Rectangle height in pixels. MUST BE POSITIVE AND NOT EXTEND OFF SCREEN.
 * @param[in] color
 *  16-bit fill color in '565' RGB format.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_write_fill_rect_preclipped(int16_t x,
                                                                int16_t y,
                                                                int16_t w,
                                                                int16_t h,
                                                                uint16_t color)
{
  adafruit_hxd8357d_set_addr_window(x, y, w, h);
  adafruit_hxd8357d_write_color(color, (uint32_t)w * h);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Draw a filled rectangle to the display.
 *
 * @param[in] x
 *  Horizontal position of first corner.
 * @param[in] y
 *  Vertical position of first corner.
 * @param[in] w
 *  Rectangle width in pixels (positive = right of first corner,
 *  negative = left of first corner).
 * @param[in] h
 *  Rectangle height in pixels (positive = below first corner,
 *  negative = above first corner)
 * @param[in] color
 *  16-bit fill color in '565' RGB format.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_fill_rect(int16_t x,
                                               int16_t y,
                                               int16_t w,
                                               int16_t h,
                                               uint16_t color)
{
  if (w && h) {
    if (w < 0) {
      x += w + 1;
      w = -w;
    }
    if (x < HXD8357D_TFTWIDTH) {
      if (h < 0) {
        y += h + 1;
        h = -h;
      }
      if (y < HXD8357D_TFTHEIGHT) {
        int16_t x2 = x + w - 1;
        if (x2 >= 0) {
          int16_t y2 = y + h - 1;
          if (y2 >= 0) {
            if (x < 0) {
              x = 0;
              w = x2 + 1;
            }
            if (y < 0) {
              y = 0;
              h = y2 + 1;
            }
            if (x2 >= HXD8357D_TFTWIDTH) {
              w = HXD8357D_TFTWIDTH - x;
            }
            if (y2 >= HXD8357D_TFTHEIGHT) {
              h = HXD8357D_TFTHEIGHT - y;
            }
            SPI_CS_LOW();
            adafruit_hxd8357d_write_fill_rect_preclipped(x, y, w, h, color);
            SPI_CS_HIGH();
          }
        }
      }
    }
  }

  return SL_STATUS_OK;
}

static void spi_usart_init(void)
{
  USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;

  usartInit.msbf = true;
  usartInit.clockMode = usartClockMode0;
  usartInit.master = true;
  usartInit.baudrate = ADAFRUIT_HXD8357D_SPI_BITRATE;

#if defined(_CMU_HFPERCLKEN0_MASK)
  CMU_ClockEnable(cmuClock_HFPER, true);
#endif
  CMU_ClockEnable(cmuClock_GPIO, true);
#if ADAFRUIT_HXD8357D_PERIPHERAL_NO == 0
  CMU_ClockEnable(cmuClock_USART0, true);
#elif ADAFRUIT_HXD8357D_PERIPHERAL_NO == 1
  CMU_ClockEnable(cmuClock_USART1, true);
#elif ADAFRUIT_HXD8357D_PERIPHERAL_NO == 2
  CMU_ClockEnable(cmuClock_USART2, true);
#endif

  usartInit.databits = usartDatabits8;
  USART_InitSync(ADAFRUIT_HXD8357D_PERIPHERAL, &usartInit);

#if defined(USART_ROUTEPEN_TXPEN)
  ADAFRUIT_HXD8357D_PERIPHERAL->ROUTELOC0 =
    (ADAFRUIT_HXD8357D_PERIPHERAL->ROUTELOC0
     & ~(_USART_ROUTELOC0_TXLOC_MASK
         | _USART_ROUTELOC0_RXLOC_MASK
         | _USART_ROUTELOC0_CLKLOC_MASK))
    | (ADAFRUIT_HXD8357D_TX_LOC  << _USART_ROUTELOC0_TXLOC_SHIFT)
    | (ADAFRUIT_HXD8357D_RX_LOC  << _USART_ROUTELOC0_RXLOC_SHIFT)
    | (ADAFRUIT_HXD8357D_CLK_LOC << _USART_ROUTELOC0_CLKLOC_SHIFT);

  ADAFRUIT_HXD8357D_PERIPHERAL->ROUTEPEN = USART_ROUTEPEN_TXPEN
                                           | USART_ROUTEPEN_RXPEN
                                           | USART_ROUTEPEN_CLKPEN
                                           | USART_ROUTEPEN_CSPEN;
#else
  GPIO->USARTROUTE[ADAFRUIT_HXD8357D_PERIPHERAL_NO].ROUTEEN =
    GPIO_USART_ROUTEEN_TXPEN
    | GPIO_USART_ROUTEEN_RXPEN
    | GPIO_USART_ROUTEEN_CLKPEN;
  GPIO->USARTROUTE[ADAFRUIT_HXD8357D_PERIPHERAL_NO].TXROUTE =
    ((uint32_t)ADAFRUIT_HXD8357D_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
    | ((uint32_t)ADAFRUIT_HXD8357D_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[ADAFRUIT_HXD8357D_PERIPHERAL_NO].RXROUTE =
    ((uint32_t)ADAFRUIT_HXD8357D_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
    | ((uint32_t)ADAFRUIT_HXD8357D_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[ADAFRUIT_HXD8357D_PERIPHERAL_NO].CLKROUTE =
    ((uint32_t)ADAFRUIT_HXD8357D_CLK_PORT << _GPIO_USART_CLKROUTE_PORT_SHIFT)
    | ((uint32_t)ADAFRUIT_HXD8357D_CLK_PIN << _GPIO_USART_CLKROUTE_PIN_SHIFT);
#endif

  GPIO_PinModeSet(ADAFRUIT_HXD8357D_TX_PORT,
                  ADAFRUIT_HXD8357D_TX_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(ADAFRUIT_HXD8357D_RX_PORT,
                  ADAFRUIT_HXD8357D_RX_PIN,
                  gpioModeInput,
                  0);
  GPIO_PinModeSet(ADAFRUIT_HXD8357D_CLK_PORT,
                  ADAFRUIT_HXD8357D_CLK_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(ADAFRUIT_HXD8357D_CS_PORT,
                  ADAFRUIT_HXD8357D_CS_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(HXD8357D_SPI_DC_PORT,
                  HXD8357D_SPI_DC_PIN,
                  gpioModePushPull,
                  0);
}

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 * Initialize the Adafruit 3.5" TFT LCD with Touchscreen.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_spi_usart_init(void)
{
  static uint8_t init_cmd[] = {
    HXD8357D_SWRESET,
    0x80 + 100 / 5,     // Soft reset, then delay 10 ms
    HXD8357D_SETC,
    3,
    0xFF,
    0x83,
    0x57,
    0xFF,
    0x80 + 500 / 5,     // No command, just delay 300 ms
    HXD8357D_SETRGB,
    4,
    0x80,
    0x00,
    0x06,
    0x06,     // 0x80 enables SDO pin (0x00 disables)
    HXD8357D_SETCOM,
    1,
    0x25,     // -1.52V
    HXD8357D_SETOSC,
    1,
    0x68,     // Normal mode 70Hz, Idle mode 55 Hz
    HXD8357D_SETPANEL,
    1,
    0x05,     // BGR, Gate direction swapped
    HXD8357D_SETPWR1,
    6,
    0x00,     // Not deep standby
    0x15,     // BT
    0x1C,     // VSPR
    0x1C,     // VSNR
    0x83,     // AP
    0xAA,     // FS
    HXD8357D_SETSTBA,
    6,
    0x50,     // OPON normal
    0x50,     // OPON idle
    0x01,     // STBA
    0x3C,     // STBA
    0x1E,     // STBA
    0x08,     // GEN
    HXD8357D_SETCYC,
    7,
    0x02,     // NW 0x02
    0x40,     // RTN
    0x00,     // DIV
    0x2A,     // DUM
    0x2A,     // DUM
    0x0D,     // GDON
    0x78,     // GDOFF
    HXD8357D_SETGAMMA,
    34,
    0x02,
    0x0A,
    0x11,
    0x1d,
    0x23,
    0x35,
    0x41,
    0x4b,
    0x4b,
    0x42,
    0x3A,
    0x27,
    0x1B,
    0x08,
    0x09,
    0x03,
    0x02,
    0x0A,
    0x11,
    0x1d,
    0x23,
    0x35,
    0x41,
    0x4b,
    0x4b,
    0x42,
    0x3A,
    0x27,
    0x1B,
    0x08,
    0x09,
    0x03,
    0x00,
    0x01,
    HXD8357D_COLMOD,
    1,
    0x55,     // 16 bit
    HXD8357D_MADCTL,
    1,
    0xC0,
    HXD8357D_TEON,
    1,
    0x00,     // TW off
    HXD8357D_TEARLINE,
    2,
    0x00,
    0x02,
    HXD8357D_SLPOUT,
    0x80 + 150 / 5,     // Exit Sleep, then delay 150 ms
    HXD8357D_DISPON,
    0x80 + 50 / 5,     // Main screen turn on, delay 50 ms
    0,                 // END OF COMMAND LIST
  };

  spi_usart_init();

  sl_sleeptimer_delay_millisecond(150);

  uint8_t cmd, x, num_args;
  uint8_t *addr = init_cmd;
  while ((cmd = pgm_read_byte(addr++)) > 0) {
    x = pgm_read_byte(addr++);
    num_args = x & 0x7F;
    if (cmd != 0xff) {
      if (x & 0x80) {
        adafruit_hxd8357d_spi_send_command(cmd, NULL, 0);
      } else {
        adafruit_hxd8357d_spi_send_command(cmd, addr, num_args);
        addr += num_args;
      }
    }
    if (x & 0x80) {
      sl_sleeptimer_delay_millisecond(150);
    }
  }

  _width = HXD8357D_TFTWIDTH; // Screen dimensions for default rotation 0
  _height = HXD8357D_TFTHEIGHT;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Set display rotation.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_set_rotation(uint8_t rotate)
{
  static uint8_t rotation = 0;
  rotation = rotate & 3; // can't be higher than 3
  switch (rotation) {
    case 0:
      rotate = MADCTL_MX | MADCTL_MY | MADCTL_RGB;
      _width = HXD8357D_TFTWIDTH;
      _height = HXD8357D_TFTHEIGHT;
      break;
    case 1:
      rotate = MADCTL_MV | MADCTL_MY | MADCTL_RGB;
      _width = HXD8357D_TFTHEIGHT;
      _height = HXD8357D_TFTWIDTH;
      break;
    case 2:
      rotate = MADCTL_RGB;
      _width = HXD8357D_TFTWIDTH;
      _height = HXD8357D_TFTHEIGHT;
      break;
    case 3:
      rotate = MADCTL_MX | MADCTL_MV | MADCTL_RGB;
      _width = HXD8357D_TFTHEIGHT;
      _height = HXD8357D_TFTWIDTH;
      break;
  }

  return adafruit_hxd8357d_spi_send_command(HXD8357D_MADCTL, &rotate, 1);
}

/**************************************************************************//**
 * Enable/Disable display color inversion.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_invert_display(bool invert)
{
  return adafruit_hxd8357d_spi_send_command(
    invert ? HXD8357D_INVON : HXD8357D_INVOFF, NULL, 0);
}

/**************************************************************************//**
 * Set the "address window" - the rectangle we will write to RAM.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_set_addr_window(uint16_t x1, uint16_t y1,
                                              uint16_t w, uint16_t h)
{
  static uint16_t old_x1 = 0xffff, old_x2 = 0xffff;
  static uint16_t old_y1 = 0xffff, old_y2 = 0xffff;
  uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);

  if ((x1 != old_x1) || (x2 != old_x2)) {
    // Column address set
    adafruit_hxd8357d_spi_write_command(HXD8357D_CASET);
    adafruit_hxd8357d_spi_write16(x1);
    adafruit_hxd8357d_spi_write16(x2);
    old_x1 = x1;
    old_x2 = x2;
  }
  if ((y1 != old_y1) || (y2 != old_y2)) {
    // Row address set
    adafruit_hxd8357d_spi_write_command(HXD8357D_PASET);
    adafruit_hxd8357d_spi_write16(y1);
    adafruit_hxd8357d_spi_write16(y2);
    old_y1 = y1;
    old_y2 = y2;
  }
  // Write to RAM
  return adafruit_hxd8357d_spi_write_command(HXD8357D_RAMWR);
}

/**************************************************************************//**
 * Draw a single pixel to the display at requested coordinates.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x >= 0) && (x < HXD8357D_TFTWIDTH) && (y >= 0)
      && (y < HXD8357D_TFTHEIGHT)) {
    // THEN set up transaction (if needed) and draw...
    SPI_CS_LOW();
    adafruit_hxd8357d_set_addr_window(x, y, 1, 1);
    adafruit_hxd8357d_spi_write16(color);
    SPI_CS_HIGH();

    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * Fill the screen completely with one color.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_fill_screen(uint16_t color)
{
  return adafruit_hxd8357d_fill_rect(0,
                                     0,
                                     HXD8357D_TFTWIDTH,
                                     HXD8357D_TFTHEIGHT,
                                     color);
}

/**************************************************************************//**
 * Draw a 16-bit image (565 RGB) at the specified (x,y) position.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_draw_rgb_bitmap(int16_t x,
                                              int16_t y,
                                              uint16_t *color,
                                              int16_t w,
                                              int16_t h)
{
  int16_t x2;
  int16_t y2;
  int16_t bx1 = 0;
  int16_t by1 = 0;
  int16_t save_w = w;

  if ((x >= HXD8357D_TFTWIDTH)
      || (y >= HXD8357D_TFTHEIGHT)
      || ((x2 = (x + w - 1)) < 0)
      || ((y2 = (y + h - 1)) < 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  if (x < 0) {
    w += x;
    bx1 = -x;
    x = 0;
  }
  if (y < 0) {
    h += y;
    by1 = -y;
    y = 0;
  }
  if (x2 >= HXD8357D_TFTWIDTH) {
    w = HXD8357D_TFTWIDTH - x;
  }
  if (y2 >= HXD8357D_TFTHEIGHT) {
    h = HXD8357D_TFTHEIGHT - y;
  }
  color += by1 * save_w + bx1;
  SPI_CS_LOW();
  adafruit_hxd8357d_set_addr_window(x, y, w, h);
  while (h--) {
    adafruit_hxd8357d_write_pixels(color, w);
    color += save_w;
  }
  SPI_CS_HIGH();

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Fill the screen area  with color. After the process
 * is complete call the user callback to notify the higher layer.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_flush_area_rgb565(int16_t x1, int16_t y1,
                                                int16_t x2, int16_t y2,
                                                uint8_t *pcolor,
                                                bool color_swap,
                                                void (*callback)(void *arg),
                                                void *callback_arg)
{
  sl_status_t sc = SL_STATUS_FAIL;

  if ((pcolor == NULL) || (callback == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if ((x1 >= 0) && (x1 < HXD8357D_TFTWIDTH)
      && (x2 >= 0) && (x2 < HXD8357D_TFTWIDTH)
      && (y1 >= 0) && (y1 < HXD8357D_TFTHEIGHT)
      && (y2 >= 0) && (y2 < HXD8357D_TFTHEIGHT)
      && (x2 >= x1) && (y2 >= y1)) {
    uint16_t *p_color = (uint16_t *)pcolor;

    for (int16_t y = y1; y <= y2; y++) {
      for (int16_t x = x1; x <= x2; x++) {
        uint16_t color;

        if (color_swap) {
          color = *p_color >> 8 | ((uint16_t)(*p_color & 0xff) << 8);
        } else {
          color = *p_color;
        }
        sc = adafruit_hxd8357d_draw_pixel(x, y, color);
        if (SL_STATUS_OK != sc) {
          goto end;
        }
        p_color++;
      }
    }
  }
  end:
  callback(callback_arg);
  return sc;
}
