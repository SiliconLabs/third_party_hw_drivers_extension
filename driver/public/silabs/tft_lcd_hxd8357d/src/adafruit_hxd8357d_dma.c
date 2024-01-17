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
#include "spidrv.h"
#include "adafruit_hxd8357d_spi_dma_config.h"
#include "sl_spidrv_hxd8357d_config.h"
#include "adafruit_hxd8357d.h"

// -----------------------------------------------------------------------------
//                       Macros
// -----------------------------------------------------------------------------

#define MADCTL_MY               0x80 ///< Bottom to top
#define MADCTL_MX               0x40 ///< Right to left
#define MADCTL_MV               0x20 ///< Reverse Mode
#define MADCTL_ML               0x10 ///< LCD refresh Bottom to top
#define MADCTL_RGB              0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR              0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH               0x04 ///< LCD refresh right to left

#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))
#define _swap_int16_t(a, b) \
  {                         \
    int16_t t = a;          \
    a = b;                  \
    b = t;                  \
  }

#define BYTE_PER_PIXEL          (2)       /* 2 Bytes color data per pixel */
#define MAX_XFER_PIXEL_COUNT    ((DMADRV_MAX_XFER_COUNT) / BYTE_PER_PIXEL)

#define SPI_CS_LOW(handle) \
  GPIO_PinOutClear(handle->portCs, handle->pinCs);
#define SPI_CS_HIGH(handle) \
  GPIO_PinOutSet(handle->portCs, handle->pinCs);

#define SPI_DC_LOW() \
  GPIO_PinOutClear(HXD8357D_SPI_DC_PORT, HXD8357D_SPI_DC_PIN);
#define SPI_DC_HIGH() \
  GPIO_PinOutSet(HXD8357D_SPI_DC_PORT, HXD8357D_SPI_DC_PIN);

static void hxd8357d_flush_area_transmit_callback(
  struct SPIDRV_HandleData *handle,
  Ecode_t transferStatus,
  int itemsTransferred);

static SPIDRV_Handle_t gspi_handle = NULL;

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
static bool color_swap_enabled = false;
static uint32_t gtotal_pixel = 0;
static uint32_t gpixel_transmit = 0;
static uint32_t gpixel_transmit_counter = 0;
static uint8_t *pColorBuffer = NULL;
static void (*flush_area_callback)(void *arg) = NULL;
static void *flush_area_callback_arg = NULL;

static uint16_t dma_buffer[(DMADRV_MAX_XFER_COUNT + 1) / 2];
static uint8_t *pColorSwap = NULL;

static int _width;
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
 *  SL_STATUS_INVALID_PARAMETER or SL_STATUS_IO if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_spi_send_command(uint8_t command,
                                                      const uint8_t *data,
                                                      uint8_t len)
{
  Ecode_t retVal;

  SPI_CS_LOW(gspi_handle);
  SPI_DC_LOW();
  retVal = SPIDRV_MTransmitB(gspi_handle, &command, 1);
  SPI_DC_HIGH();
  if (ECODE_OK != retVal) {
    goto error;
  }

  retVal = SPIDRV_MTransmitB(gspi_handle, data, len);
  if (ECODE_OK != retVal) {
    goto error;
  }

  SPI_CS_HIGH(gspi_handle);
  return SL_STATUS_OK;
  error:
  SPI_CS_HIGH(gspi_handle);
  return SL_STATUS_IO;
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
 *  SL_STATUS_IO if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_spi_write_command(uint8_t cmd)
{
  SPI_DC_LOW();
  Ecode_t retVal = SPIDRV_MTransmitB(gspi_handle, &cmd, 1);
  SPI_DC_HIGH();

  return ECODE_OK == retVal ? SL_STATUS_OK : SL_STATUS_IO;
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
 *  SL_STATUS_IO if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_spi_write16(uint16_t data)
{
  uint8_t bytes[2];

  bytes[0] = data >> 8;
  bytes[1] = data & 0x00FF;

  Ecode_t retVal = SPIDRV_MTransmitB(gspi_handle, bytes, 2);

  return ECODE_OK == retVal ? SL_STATUS_OK : SL_STATUS_IO;
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
 *  SL_STATUS_IO if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_write_color(uint16_t color, uint32_t len)
{
  uint32_t i, n, size;
  Ecode_t retVal;

  if (!len) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  for (n = 0; n < len; n += MAX_XFER_PIXEL_COUNT) {
    if (n + MAX_XFER_PIXEL_COUNT <= len) {
      size = MAX_XFER_PIXEL_COUNT;
    } else {
      size = len - n;
    }

    for (i = 0; i < size; i++) {
      dma_buffer[i] = color >> 8 | ((uint16_t)(color & 0xff) << 8);
    }
    retVal = SPIDRV_MTransmitB(gspi_handle, dma_buffer, size * 2);
    if (ECODE_OK != retVal) {
      goto error;
    }
  }

  return SL_STATUS_OK;
  error:
  return SL_STATUS_IO;
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
 *  SL_STATUS_INVALID_PARAMETER or SL_STATUS_IO if the process is failed.
 ******************************************************************************/
static sl_status_t adafruit_hxd8357d_write_pixels(uint16_t *colors,
                                                  uint32_t len)
{
  uint32_t i, n, size;
  Ecode_t retVal;

  if (!len) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  for (n = 0; n < len; n += MAX_XFER_PIXEL_COUNT) {
    if (n + MAX_XFER_PIXEL_COUNT <= len) {
      size = MAX_XFER_PIXEL_COUNT;
    } else {
      size = len - n;
    }

    for (i = 0; i < size; i++) {
      dma_buffer[i] = *colors >> 8 | ((uint16_t)(*colors & 0xff) << 8);
      colors++;
    }
    retVal = SPIDRV_MTransmitB(gspi_handle, dma_buffer, size * 2);
    if (ECODE_OK != retVal) {
      goto error;
    }
  }

  return SL_STATUS_OK;
  error:
  return SL_STATUS_IO;
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
            SPI_CS_LOW(gspi_handle);
            adafruit_hxd8357d_write_fill_rect_preclipped(x, y, w, h, color);
            SPI_CS_HIGH(gspi_handle);
          }
        }
      }
    }
  }

  return SL_STATUS_OK;
}

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 * Initialize the Adafruit 3.5" TFT LCD with Touchscreen.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_spi_dma_init(SPIDRV_Handle_t spidrv_handle)
{
  gspi_handle = spidrv_handle;

  GPIO_PinModeSet(SL_SPIDRV_HXD8357D_CS_PORT,
                  SL_SPIDRV_HXD8357D_CS_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(HXD8357D_SPI_DC_PORT,
                  HXD8357D_SPI_DC_PIN,
                  gpioModePushPull,
                  0);

  static const uint8_t init_cmd[] = {
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

  sl_sleeptimer_delay_millisecond(150);

  uint8_t cmd, x, numArgs;
  const uint8_t *addr = init_cmd;
  while ((cmd = pgm_read_byte(addr++)) > 0) { // '0' command ends list
    x = pgm_read_byte(addr++);
    numArgs = x & 0x7F;
    if (cmd != 0xFF) { // '255' is ignored
      if (x & 0x80) {  // If high bit set, numArgs is a delay time
        adafruit_hxd8357d_spi_send_command(cmd, NULL, 0);
      } else {
        adafruit_hxd8357d_spi_send_command(cmd, addr, numArgs);
        addr += numArgs;
      }
    }
    if (x & 0x80) {       // If high bit set...
      // numArgs is actually a delay time (5ms units)
      sl_sleeptimer_delay_millisecond(numArgs * 5);
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
  uint16_t tmp[2];
  Ecode_t retVal;

  if ((x1 != old_x1) || (x2 != old_x2)) {
    // Column address set
    adafruit_hxd8357d_spi_write_command(HXD8357D_CASET);
    tmp[0] = x1 >> 8 | ((uint16_t)(x1 & 0xff) << 8);
    tmp[1] = x2 >> 8 | ((uint16_t)(x2 & 0xff) << 8);
    retVal = SPIDRV_MTransmitB(gspi_handle, tmp, 4);
    if (ECODE_OK != retVal) {
      goto error;
    }
    old_x1 = x1;
    old_x2 = x2;
  }
  if ((y1 != old_y1) || (y2 != old_y2)) {
    // Row address set
    adafruit_hxd8357d_spi_write_command(HXD8357D_PASET);
    tmp[0] = y1 >> 8 | ((uint16_t)(y1 & 0xff) << 8);
    tmp[1] = y2 >> 8 | ((uint16_t)(y2 & 0xff) << 8);
    retVal = SPIDRV_MTransmitB(gspi_handle, tmp, 4);
    if (ECODE_OK != retVal) {
      goto error;
    }
    old_y1 = y1;
    old_y2 = y2;
  }
  // Write to RAM
  return adafruit_hxd8357d_spi_write_command(HXD8357D_RAMWR);
  error:
  return SL_STATUS_IO;
}

/**************************************************************************//**
 * Draw a single pixel to the display at requested coordinates.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x >= 0) && (x < HXD8357D_TFTWIDTH) && (y >= 0)
      && (y < HXD8357D_TFTHEIGHT)) {
    // THEN set up transaction (if needed) and draw...
    SPI_CS_LOW(gspi_handle);
    adafruit_hxd8357d_set_addr_window(x, y, 1, 1);
    adafruit_hxd8357d_spi_write16(color);
    SPI_CS_HIGH(gspi_handle);

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
  SPI_CS_LOW(gspi_handle);
  adafruit_hxd8357d_set_addr_window(x, y, w, h);
  while (h--) {
    adafruit_hxd8357d_write_pixels(color, w);
    color += save_w;
  }
  SPI_CS_HIGH(gspi_handle);

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Fill the screen area  with color using SPI transmit DMA. After the process
 * is complete call the user callback to notify the higher layer.
 *****************************************************************************/
sl_status_t adafruit_hxd8357d_flush_area_rgb565(int16_t x1, int16_t y1,
                                                int16_t x2, int16_t y2,
                                                uint8_t *pcolor,
                                                bool color_swap,
                                                void (*callback)(void *arg),
                                                void *callback_arg)
{
  if ((pcolor != NULL) && (callback != NULL)
      && (x1 >= 0) && (x1 < HXD8357D_TFTWIDTH)
      && (x2 >= 0) && (x2 < HXD8357D_TFTWIDTH)
      && (y1 >= 0) && (y1 < HXD8357D_TFTHEIGHT)
      && (y2 >= 0) && (y2 < HXD8357D_TFTHEIGHT)
      && (x2 >= x1) && (y2 >= y1)) {
    uint16_t width = x2 - x1 + 1;
    uint16_t hight = y2 - y1 + 1;

    SPI_CS_LOW(gspi_handle);
    adafruit_hxd8357d_set_addr_window(x1, y1, width, hight);

    gtotal_pixel = width * hight;
    gpixel_transmit = 0;
    gpixel_transmit_counter = 0;
    color_swap_enabled = color_swap;
    flush_area_callback = callback;
    flush_area_callback_arg = callback_arg;

    if (gpixel_transmit_counter < gtotal_pixel) {
      uint32_t pixel_remaining = gtotal_pixel - gpixel_transmit_counter;
      gpixel_transmit = pixel_remaining > MAX_XFER_PIXEL_COUNT
                        ? MAX_XFER_PIXEL_COUNT : pixel_remaining;

      if (!color_swap_enabled) {
        uint16_t i;
        pColorSwap = pcolor;
        uint8_t colorData_H;
        uint8_t colorData_L;

        for (i = 0; i < gpixel_transmit; i++) {
          colorData_H =
            pColorSwap[((gpixel_transmit_counter + i) *  BYTE_PER_PIXEL) + 1];
          colorData_L =
            pColorSwap[((gpixel_transmit_counter + i) *  BYTE_PER_PIXEL)];

          /* Swap 2 bytes color data before sending to TFT LCD */
          dma_buffer[i] = colorData_H | (uint16_t)colorData_L << 8;
        }

        pColorBuffer = (uint8_t *)dma_buffer;
      } else {
        pColorBuffer = (uint8_t *)pcolor;
      }

      /* Start transmit data, the process is continued in
       * adafruit_hxd8357d_flush_area_transmit_callback function
       */

      SPIDRV_MTransmit(gspi_handle,
                       (uint8_t *)pColorBuffer,
                       gpixel_transmit * BYTE_PER_PIXEL,
                       hxd8357d_flush_area_transmit_callback);

      if (color_swap_enabled) {
        pColorBuffer += (gpixel_transmit * BYTE_PER_PIXEL);
      }

      gpixel_transmit_counter += gpixel_transmit;
    }

    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

static void hxd8357d_flush_area_transmit_callback(
  struct SPIDRV_HandleData *handle,
  Ecode_t transferStatus,
  int itemsTransferred)
{
  (void)itemsTransferred;

  if (handle == gspi_handle) {
    if (transferStatus == ECODE_OK) {
      if (gpixel_transmit_counter < gtotal_pixel) {
        /* Continue transmit the data */
        uint32_t pixel_remaining = gtotal_pixel - gpixel_transmit_counter;
        gpixel_transmit = pixel_remaining > MAX_XFER_PIXEL_COUNT
                          ? MAX_XFER_PIXEL_COUNT : pixel_remaining;

        if (!color_swap_enabled) {
          uint16_t i;
          uint8_t colorData_H;
          uint8_t colorData_L;

          for (i = 0; i < gpixel_transmit; i++) {
            colorData_H =
              pColorSwap[((gpixel_transmit_counter + i) *  BYTE_PER_PIXEL) + 1];
            colorData_L =
              pColorSwap[((gpixel_transmit_counter + i) *  BYTE_PER_PIXEL)];

            /* Swap 2 bytes color data before sending to TFT LCD */
            dma_buffer[i] = colorData_H | (uint16_t)colorData_L << 8;
          }

          pColorBuffer = (uint8_t *)dma_buffer;
        }
        SPIDRV_MTransmit(gspi_handle,
                         (uint8_t *)pColorBuffer,
                         gpixel_transmit * BYTE_PER_PIXEL,
                         hxd8357d_flush_area_transmit_callback);

        if (color_swap_enabled) {
          pColorBuffer += (gpixel_transmit * BYTE_PER_PIXEL);
        }

        gpixel_transmit_counter += gpixel_transmit;
      } else {
        /* After the process is complete call the user callback to
         * notify the higher layer.
         */
        SPI_CS_HIGH(gspi_handle);

        if (flush_area_callback != NULL) {
          flush_area_callback(flush_area_callback_arg);
        }
      }
    }
  }
}
