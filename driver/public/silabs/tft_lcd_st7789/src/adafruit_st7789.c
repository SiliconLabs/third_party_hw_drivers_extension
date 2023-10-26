/***************************************************************************//**
 * @file adafruit_st7789.c
 * @brief Adafruit ST7789 Color TFT Display Driver source file.
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
#include "adafruit_st7789_spi_usart_config.h"
#include "adafruit_st7789.h"

// -----------------------------------------------------------------------------
//                       Macros
// -----------------------------------------------------------------------------
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))

#define SPI_CS_LOW() \
  GPIO_PinOutClear(ADAFRUIT_ST7789_CS_PORT, ADAFRUIT_ST7789_CS_PIN);
#define SPI_CS_HIGH() \
  GPIO_PinOutSet(ADAFRUIT_ST7789_CS_PORT, ADAFRUIT_ST7789_CS_PIN);

#define SPI_DC_LOW() \
  GPIO_PinOutClear(ST7789_SPI_DC_PORT, ST7789_SPI_DC_PIN);
#define SPI_DC_HIGH() \
  GPIO_PinOutSet(ST7789_SPI_DC_PORT, ST7789_SPI_DC_PIN);

static uint16_t _colstart = 0, _colstart2 = 0;
static uint16_t _rowstart = 0, _rowstart2 = 0;
static uint16_t _xstart = 0, _ystart = 0;
static uint16_t _width = ST7789_TFTWIDTH, _height = ST7789_TFTHEIGHT;

static sl_status_t adafruit_st7789_spi_send_command(uint8_t command,
                                                    uint8_t *data,
                                                    uint8_t len);
static sl_status_t adafruit_st7789_spi_write_command(uint8_t cmd);
static sl_status_t adafruit_st7789_spi_write16(uint16_t data);
static sl_status_t adafruit_st7789_spi_write32(uint32_t data);
static sl_status_t adafruit_st7789_write_color(uint16_t color, uint32_t len);
static sl_status_t adafruit_st7789_write_pixels(uint16_t *colors, uint32_t len);
static sl_status_t adafruit_st7789_write_fill_rect_preclipped(int16_t x,
                                                              int16_t y,
                                                              int16_t w,
                                                              int16_t h,
                                                              uint16_t color);
static sl_status_t adafruit_st7789_fill_rect(int16_t x,
                                             int16_t y,
                                             int16_t w,
                                             int16_t h,
                                             uint16_t color);

/***************************************************************************//**
 * @brief
 *  Adafruit_SPI TFT Send Command handles complete sending of commands and
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
 *  SL_STATUS_FAIL or SL_STATUS_INVALID_PARAMETER if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_st7789_spi_send_command(uint8_t command,
                                                    uint8_t *data,
                                                    uint8_t len)
{
  sl_status_t stt = SL_STATUS_OK;

  SPI_CS_LOW();
  SPI_DC_LOW();
  USART_Tx(ADAFRUIT_ST7789_PERIPHERAL, command);
  SPI_DC_HIGH();

  if (len != 0) {
    if (NULL != data) {
      while (len--) {
        USART_Tx(ADAFRUIT_ST7789_PERIPHERAL, *data);
        data++;
      }
    } else {
      stt = SL_STATUS_INVALID_PARAMETER;
    }
  }
  SPI_CS_HIGH();

  return stt;
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
static sl_status_t adafruit_st7789_spi_write_command(uint8_t cmd)
{
  SPI_DC_LOW();
  USART_Tx(ADAFRUIT_ST7789_PERIPHERAL, cmd);
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
static sl_status_t adafruit_st7789_spi_write16(uint16_t data)
{
  uint8_t bytes[2];

  bytes[0] = data >> 8;
  bytes[1] = data & 0x00FF;

  USART_Tx(ADAFRUIT_ST7789_PERIPHERAL, bytes[0]);
  USART_Tx(ADAFRUIT_ST7789_PERIPHERAL, bytes[1]);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  Write a double command word to the display.
 *
 * @param[in] cmd
 *  32-bit command to write.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_st7789_spi_write32(uint32_t data)
{
  uint8_t bytes[4];

  bytes[0] = (data >> 24);
  bytes[1] = (data >> 16) & 0x00FF;
  bytes[2] = (data >> 8) & 0x0000FF;
  bytes[3] = data & 0x000000FF;

  USART_Tx(ADAFRUIT_ST7789_PERIPHERAL, bytes[0]);
  USART_Tx(ADAFRUIT_ST7789_PERIPHERAL, bytes[1]);
  USART_Tx(ADAFRUIT_ST7789_PERIPHERAL, bytes[2]);
  USART_Tx(ADAFRUIT_ST7789_PERIPHERAL, bytes[3]);

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
static sl_status_t adafruit_st7789_write_color(uint16_t color, uint32_t len)
{
  while (len--) {
    adafruit_st7789_spi_write16(color);
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
 *  SL_STATUS_INVALID_PARAMETER if (len = 0) or (colors == NULL)
 ******************************************************************************/
static sl_status_t adafruit_st7789_write_pixels(uint16_t *colors, uint32_t len)
{
  if ((!len) || (colors == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  while (len--) {
    adafruit_st7789_spi_write16(*colors);
    colors++;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  A lower-level version of ADAFRUIT_ST7789_fill_rect(). This version
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
static sl_status_t adafruit_st7789_write_fill_rect_preclipped(int16_t x,
                                                              int16_t y,
                                                              int16_t w,
                                                              int16_t h,
                                                              uint16_t color)
{
  adafruit_st7789_set_addr_window(x, y, w, h);
  adafruit_st7789_write_color(color, (uint32_t)w * h);

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
static sl_status_t adafruit_st7789_fill_rect(int16_t x,
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
    if (x < _width) {
      if (h < 0) {
        y += h + 1;
        h = -h;
      }
      if (y < _height) {
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
            if (x2 >= _width) {
              w = _width - x;
            }
            if (y2 >= _height) {
              h = _height - y;
            }
            SPI_CS_LOW();
            adafruit_st7789_write_fill_rect_preclipped(x, y, w, h, color);
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
  usartInit.baudrate = ADAFRUIT_ST7789_SPI_BITRATE;

#if defined(_CMU_HFPERCLKEN0_MASK)
  CMU_ClockEnable(cmuClock_HFPER, true);
#endif
  CMU_ClockEnable(cmuClock_GPIO, true);
#if ADAFRUIT_ST7789_PERIPHERAL_NO == 0
  CMU_ClockEnable(cmuClock_USART0, true);
#elif ADAFRUIT_ST7789_PERIPHERAL_NO == 1
  CMU_ClockEnable(cmuClock_USART1, true);
#elif ADAFRUIT_ST7789_PERIPHERAL_NO == 2
  CMU_ClockEnable(cmuClock_USART2, true);
#endif

  usartInit.databits = usartDatabits8;
  USART_InitSync(ADAFRUIT_ST7789_PERIPHERAL, &usartInit);

#if defined(USART_ROUTEPEN_TXPEN)
  ADAFRUIT_ST7789_PERIPHERAL->ROUTELOC0 =
    (ADAFRUIT_ST7789_PERIPHERAL->ROUTELOC0
     & ~(_USART_ROUTELOC0_TXLOC_MASK
         | _USART_ROUTELOC0_RXLOC_MASK
         | _USART_ROUTELOC0_CLKLOC_MASK))
    | (ADAFRUIT_ST7789_TX_LOC  << _USART_ROUTELOC0_TXLOC_SHIFT)
    | (ADAFRUIT_ST7789_RX_LOC  << _USART_ROUTELOC0_RXLOC_SHIFT)
    | (ADAFRUIT_ST7789_CLK_LOC << _USART_ROUTELOC0_CLKLOC_SHIFT);

  ADAFRUIT_ST7789_PERIPHERAL->ROUTEPEN = USART_ROUTEPEN_TXPEN
                                         | USART_ROUTEPEN_RXPEN
                                         | USART_ROUTEPEN_CLKPEN
                                         | USART_ROUTEPEN_CSPEN;
#else
  GPIO->USARTROUTE[ADAFRUIT_ST7789_PERIPHERAL_NO].ROUTEEN =
    GPIO_USART_ROUTEEN_TXPEN
    | GPIO_USART_ROUTEEN_RXPEN
    | GPIO_USART_ROUTEEN_CLKPEN;
  GPIO->USARTROUTE[ADAFRUIT_ST7789_PERIPHERAL_NO].TXROUTE =
    ((uint32_t)ADAFRUIT_ST7789_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
    | ((uint32_t)ADAFRUIT_ST7789_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[ADAFRUIT_ST7789_PERIPHERAL_NO].RXROUTE =
    ((uint32_t)ADAFRUIT_ST7789_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
    | ((uint32_t)ADAFRUIT_ST7789_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[ADAFRUIT_ST7789_PERIPHERAL_NO].CLKROUTE =
    ((uint32_t)ADAFRUIT_ST7789_CLK_PORT << _GPIO_USART_CLKROUTE_PORT_SHIFT)
    | ((uint32_t)ADAFRUIT_ST7789_CLK_PIN << _GPIO_USART_CLKROUTE_PIN_SHIFT);
#endif

  GPIO_PinModeSet(ADAFRUIT_ST7789_TX_PORT,
                  ADAFRUIT_ST7789_TX_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(ADAFRUIT_ST7789_RX_PORT,
                  ADAFRUIT_ST7789_RX_PIN,
                  gpioModeInput,
                  0);
  GPIO_PinModeSet(ADAFRUIT_ST7789_CLK_PORT,
                  ADAFRUIT_ST7789_CLK_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(ADAFRUIT_ST7789_CS_PORT,
                  ADAFRUIT_ST7789_CS_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(ST7789_SPI_DC_PORT,
                  ST7789_SPI_DC_PIN,
                  gpioModePushPull,
                  0);
}

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 * Initialize the Adafruit ST7789 Color TFT Display Driver.
 *****************************************************************************/
sl_status_t adafruit_st7789_spi_usart_init(void)
{
  static uint8_t init_cmd[] = {
    9,                              //  9 commands in list:
    ST7789_SWRESET, ST_CMD_DELAY,   //  1: Software reset, no args, w/delay
    150,                            //     ~150 ms delay
    ST7789_SLPOUT, ST_CMD_DELAY,    //  2: Out of sleep mode, no args, w/delay
    10,                             //      10 ms delay
    ST7789_COLMOD, 1 + ST_CMD_DELAY,//  3: Set color mode, 1 arg + delay:
    0x55,                           //     16-bit color
    10,                             //     10 ms delay
    ST7789_MADCTL, 1,               //  4: Mem access ctrl (directions), 1 arg:
    0x08,                           //     Row/col addr, bottom-top refresh
    ST7789_CASET, 4,                //  5: Column addr set, 4 args, no delay:
    0x00,
    0,                              //     XSTART = 0
    (ST7789_TFTWIDTH >> 8),
    (ST7789_TFTWIDTH & 0xFF),       //     XEND = ST7789_TFTWIDTH
    ST7789_RASET, 4,                //  6: Row addr set, 4 args, no delay:
    0x00,
    0,                              //     YSTART = 0
    (ST7789_TFTHEIGHT >> 8),
    (ST7789_TFTHEIGHT & 0xFF),      //     YEND = ST7789_TFTHEIGHT
    ST7789_INVOFF, ST_CMD_DELAY,    //  7: INVOFF
    10,
    ST7789_NORON, ST_CMD_DELAY,     //  8: Normal display on, no args, w/delay
    10,                             //     10 ms delay
    ST7789_DISPON, ST_CMD_DELAY,    //  9: Main screen turn on, no args, delay
    10
  };

  spi_usart_init();

  uint8_t numCommands, cmd, numArgs;
  uint16_t ms;
  uint8_t *addr = init_cmd;

  numCommands = pgm_read_byte(addr++); // Number of commands to follow
  while (numCommands--) {              // For each command...
    cmd = pgm_read_byte(addr++);       // Read command
    numArgs = pgm_read_byte(addr++);   // Number of args to follow
    ms = numArgs & ST_CMD_DELAY;       // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;          // Mask out delay bit
    adafruit_st7789_spi_send_command(cmd, addr, numArgs);
    addr += numArgs;

    if (ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if (ms == 255) {
        ms = 500; // If 255, delay for 500 ms
      }
      sl_sleeptimer_delay_millisecond(ms);
    }
  }

  _rowstart = _rowstart2 = (int)((320 - ST7789_TFTHEIGHT) / 2);
  _colstart = (int)((240 - ST7789_TFTWIDTH + 1) / 2);
  _colstart2 = (int)((240 - ST7789_TFTWIDTH) / 2);

  adafruit_st7789_set_rotation(adafruit_st7789_rotation_none);

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Get the current display width. This value will be changed when the screen is
 * rotated.
 *****************************************************************************/
uint16_t adafruit_st7789_get_display_width(void)
{
  return _width;
}

/**************************************************************************//**
 * Get the current display height. This value will be changed when the screen is
 * rotated.
 *****************************************************************************/
uint16_t adafruit_st7789_get_display_height(void)
{
  return _height;
}

/**************************************************************************//**
 * Enable/Disable display color inversion.
 *****************************************************************************/
sl_status_t adafruit_st7789_invert_display(bool invert)
{
  return adafruit_st7789_spi_send_command(
    invert ? ST7789_INVON : ST7789_INVOFF, NULL, 0);
}

/**************************************************************************//**
 * Enable/Disable display on or off
 *****************************************************************************/
sl_status_t adafruit_st7789_enable_display(bool enable)
{
  return adafruit_st7789_spi_send_command(
    enable ? ST7789_DISPON : ST7789_DISPOFF, NULL, 0);
}

/**************************************************************************/

/*!
 *    @brief  Change whether TE pin output is on or off
 *    @param  enable True if you want the TE pin ON, false OFF
 */

/**************************************************************************/
sl_status_t adafruit_st7789_enable_tearing(bool enable)
{
  return adafruit_st7789_spi_send_command(
    enable ? ST7789_TEON : ST7789_TEOFF, NULL, 0);
}

/**************************************************************************/

/*!
 *    @brief  Change whether sleep mode is on or off
 *    @param  enable True if you want sleep mode ON, false OFF
 */

/**************************************************************************/
sl_status_t adafruit_st7789_enableSleep(bool enable)
{
  return adafruit_st7789_spi_send_command(
    enable ? ST7789_SLPIN : ST7789_SLPOUT, NULL, 0);
}

/**************************************************************************//**
 * Set the "address window" - the rectangle we will write to RAM.
 *****************************************************************************/
sl_status_t adafruit_st7789_set_addr_window(uint16_t x, uint16_t y,
                                            uint16_t w, uint16_t h)
{
  x += _xstart;
  y += _ystart;
  uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
  uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);

  adafruit_st7789_spi_write_command(ST7789_CASET); // Column addr set
  adafruit_st7789_spi_write32(xa);

  adafruit_st7789_spi_write_command(ST7789_RASET); // Row addr set
  adafruit_st7789_spi_write32(ya);

  // Write to RAM
  return adafruit_st7789_spi_write_command(ST7789_RAMWR);
}

/**************************************************************************//**
 * Set the rotation of TFT
 *****************************************************************************/
sl_status_t adafruit_st7789_set_rotation(adafruit_st7789_rotation_e rotation)
{
  uint8_t madctl = 0;

  uint8_t _rotation = rotation % 4; // can't be higher than 3

  switch (_rotation) {
    case adafruit_st7789_rotation_none:
      madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB;
      _xstart = _colstart;
      _ystart = _rowstart;
      _width = ST7789_TFTWIDTH;
      _height = ST7789_TFTHEIGHT;
      break;
    case adafruit_st7789_rotation_90:
      madctl = ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
      _xstart = _rowstart;
      _ystart = _colstart2;
      _width = ST7789_TFTHEIGHT;
      _height = ST7789_TFTWIDTH;
      break;
    case adafruit_st7789_rotation_180:
      madctl = ST7789_MADCTL_RGB;
      _xstart = _colstart2;
      _ystart = _rowstart2;
      _width = ST7789_TFTWIDTH;
      _height = ST7789_TFTHEIGHT;
      break;
    case adafruit_st7789_rotation_270:
      madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
      _xstart = _rowstart2;
      _ystart = _colstart;
      _width = ST7789_TFTHEIGHT;
      _height = ST7789_TFTWIDTH;
      break;
  }

  return adafruit_st7789_spi_send_command(ST7789_MADCTL, &madctl, 1);
}

/**************************************************************************//**
 * Draw a single pixel to the display at requested coordinates.
 *****************************************************************************/
sl_status_t adafruit_st7789_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x >= 0) && (x < _width) && (y >= 0)
      && (y < _height)) {
    // THEN set up transaction (if needed) and draw...
    SPI_CS_LOW();
    adafruit_st7789_set_addr_window(x, y, 1, 1);
    adafruit_st7789_spi_write16(color);
    SPI_CS_HIGH();

    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * Fill the screen completely with one color.
 *****************************************************************************/
sl_status_t adafruit_st7789_fill_screen(uint16_t color)
{
  return adafruit_st7789_fill_rect(0,
                                   0,
                                   _width,
                                   _height,
                                   color);
}

/**************************************************************************//**
 * Draw a 16-bit image (565 RGB) at the specified (x,y) position.
 *****************************************************************************/
sl_status_t adafruit_st7789_draw_rgb_bitmap(int16_t x,
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

  if ((x >= _width)
      || (y >= _height)
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
  if (x2 >= _width) {
    w = _width - x;
  }
  if (y2 >= _height) {
    h = _height - y;
  }
  color += by1 * save_w + bx1;
  SPI_CS_LOW();
  adafruit_st7789_set_addr_window(x, y, w, h);
  while (h--) {
    adafruit_st7789_write_pixels(color, w);
    color += save_w;
  }
  SPI_CS_HIGH();

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Fill the screen area  with color. After the process
 * is complete call the user callback to notify the higher layer.
 *****************************************************************************/
sl_status_t adafruit_st7789_flush_area_rgb565(int16_t x1, int16_t y1,
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

  if ((x1 >= 0) && (x1 < _width)
      && (x2 >= 0) && (x2 < _width)
      && (y1 >= 0) && (y1 < _height)
      && (y2 >= 0) && (y2 < _height)
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
        sc = adafruit_st7789_draw_pixel(x, y, color);
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
