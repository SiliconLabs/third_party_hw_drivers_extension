/***************************************************************************//**
 * @file glib.h
 * @brief Silicon Labs Graphics Library
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef GLIB_H
#define GLIB_H

/* C-header files */
#include <sl_status.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* GLIB header files */

#ifdef __cplusplus
extern "C" {
#endif

/** GLIB Base error code */
#define ECODE_GLIB_BASE                         0x00000000

/* Error codes */
/** Successful call */
#define GLIB_OK                                 0x00000000

/** Function did not draw */
#define GLIB_ERROR_NOTHING_TO_DRAW              (ECODE_GLIB_BASE | (1 << 0))

/** Invalid char */
#define GLIB_ERROR_INVALID_CHAR                 (ECODE_GLIB_BASE | (1 << 1))

/** Coordinates out of bounds */
#define GLIB_OUT_OF_BOUNDS                      (ECODE_GLIB_BASE | (1 << 2))

/** Invalid coordinates (ex. xMin > xMax) */
#define GLIB_ERROR_INVALID_REGION               (ECODE_GLIB_BASE | (1 << 3))

/** Invalid argument */
#define GLIB_ERROR_INVALID_ARGUMENT             (ECODE_GLIB_BASE | (1 << 4))

/** Out of memory */
#define GLIB_ERROR_OUT_OF_MEMORY                (ECODE_GLIB_BASE | (1 << 5))

/** File not supported */
#define GLIB_ERROR_FILE_NOT_SUPPORTED           (ECODE_GLIB_BASE | (1 << 6))

/** General IO Error */
#define GLIB_ERROR_IO                           (ECODE_GLIB_BASE | (1 << 7))

/** Invalid file */
#define GLIB_ERROR_INVALID_FILE                 (ECODE_GLIB_BASE | (1 << 8))

/** Driver not initialized correctly */
#define GLIB_ERROR_DRIVER_NOT_INITIALIZED       (ECODE_GLIB_BASE | (1 << 9))

#define GLIB_WHITE                              0xFFFF ///< 255, 255, 255
#define GLIB_BLACK                              0x0000 ///<   0,   0,   0

typedef uint32_t   glib_status_t;

/// Font data stored PER GLYPH
typedef struct {
  uint16_t bitmap_offset; ///< Pointer into GFXfont->bitmap
  uint8_t width;         ///< Bitmap dimensions in pixels
  uint8_t height;        ///< Bitmap dimensions in pixels
  uint8_t x_advance;      ///< Distance to advance cursor (x axis)
  int8_t x_offset;        ///< X dist from cursor pos to UL corner
  int8_t y_offset;        ///< Y dist from cursor pos to UL corner
} glib_gfx_glyph_t;

/// Data stored for FONT AS A WHOLE
typedef struct {
  uint8_t *bitmap;  ///< Glyph bitmaps, concatenated
  glib_gfx_glyph_t *glyph;  ///< Glyph array
  uint16_t first;   ///< ASCII extents (first char)
  uint16_t last;    ///< ASCII extents (last char)
  uint8_t y_advance; ///< Newline distance (y axis)
} glib_gfx_font_t;

/** @brief GLIB Drawing Context
 *  (Multiple instances of glib_context_t can exist)
 */
typedef struct __glib_context_t{
  int16_t width;        ///< This is the 'raw' display width - never changes
  int16_t height;       ///< This is the 'raw' display height - never changes
  uint16_t text_color;    ///< Text color

  uint16_t bg_color;    ///< Text background color

  int16_t cursor_x;     ///< x location to start print()ing text
  int16_t cursor_y;     ///< y location to start print()ing text
  uint8_t textsize_x;   ///< Desired magnification in X-axis of text to print()
  uint8_t textsize_y;   ///< Desired magnification in Y-axis of text to print()
  uint8_t rotation;     ///< Display rotation (0 thru 3)
  bool wrap;            ///< If set, 'wrap' text at right edge of display
  bool cp437;           ///< If set, use correct CP437 charset (default is off)
  glib_gfx_font_t *font;     ///< Font definition
} glib_context_t;

/***************************************************************************//**
 *    @brief
 *      Initialization function for the glib.
 *
 *    @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_init(glib_context_t *g_context);

/***************************************************************************//**
 *     @brief
 *     Clears the display with the background color of the glib_context_t
 *
 *     @param g_context
 *     Pointer to a GLIB_Context_t which holds the background color.
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_clear(glib_context_t *g_context);

/***************************************************************************//**
 *  @brief      Set rotation setting for display
 *  @param  x   0 thru 3 corresponding to 4 cardinal rotations
 ******************************************************************************/
glib_status_t glib_set_rotation(glib_context_t *g_context, uint8_t x);

/***************************************************************************//**
 *     @brief
 *     Fill the display with the color
 *
 *     @param g_context
 *     Pointer to a GLIB_Context_t which holds the background color.
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_fill(glib_context_t *g_context, uint16_t color);

/***************************************************************************//**
 *     @brief
 *     Draws a pixel at x, y using foregroundColor defined in the
 *   glib_context_t.
 *
 *     @param g_context
 *     Pointer to a glib_context_t which holds the foreground color.
 *     @param x
 *     X-coordinate
 *     @param y
 *     Y-coordinate
 *
 *     @param color  16-bit pixel color.
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_draw_pixel(glib_context_t *g_context,
                              int16_t x, int16_t y,
                              uint16_t color);

/***************************************************************************//**
 *    @brief    Write a line.  Bresenham's algorithm - thx wikpedia
 *     @param    x0  Start point x coordinate
 *     @param    y0  Start point y coordinate
 *     @param    x1  End point x coordinate
 *     @param    y1  End point y coordinate
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_line(glib_context_t *g_context,
                             int16_t x0, int16_t y0,
                             int16_t x1, int16_t y1,
                             uint16_t color);

/***************************************************************************//**
 *    @brief    Draw a circle outline
 *     @param    x0   Center-point x coordinate
 *     @param    y0   Center-point y coordinate
 *     @param    r   Radius of circle
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_circle(glib_context_t *g_context,
                               int16_t x0, int16_t y0, int16_t r,
                               uint16_t color);

/***************************************************************************//**
 *     @brief    Quarter-circle drawer, used to do circles and roundrects
 *     @param    x0   Center-point x coordinate
 *     @param    y0   Center-point y coordinate
 *     @param    r   Radius of circle
 *     @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of
 *    the circle we're doing
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_circle_helper(glib_context_t *g_context,
                                      int16_t x0, int16_t y0, int16_t r,
                                      uint8_t cornername, uint16_t color);

/***************************************************************************//**
 *    @brief    Draw a circle with filled color
 *     @param    x0   Center-point x coordinate
 *     @param    y0   Center-point y coordinate
 *     @param    r   Radius of circle
 *     @param    color 16-bit 5-6-5 Color to fill with
 ******************************************************************************/
glib_status_t glib_fill_circle(glib_context_t *g_context,
                               int16_t x0, int16_t y0, int16_t r,
                               uint16_t color);

/***************************************************************************//**
 *     @brief  Quarter-circle drawer with fill, used for circles and roundrects
 *     @param  x0       Center-point x coordinate
 *     @param  y0       Center-point y coordinate
 *     @param  r        Radius of circle
 *     @param  corners  Mask bits indicating which quarters we're doing
 *     @param  delta    Offset from center-point, used for round-rects
 *     @param  color    16-bit 5-6-5 Color to fill with
 ******************************************************************************/
glib_status_t glib_fill_circle_helper(glib_context_t *g_context,
                                      int16_t x0, int16_t y0, int16_t r,
                                      uint8_t corners, int16_t delta,
                                      uint16_t color);

/***************************************************************************//**
 *    @brief   Draw a rectangle with no fill color
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    w   Width in pixels
 *     @param    h   Height in pixels
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_rect(glib_context_t *g_context,
                             int16_t x, int16_t y, int16_t w, int16_t h,
                             uint16_t color);

/***************************************************************************//**
 *    @brief    Fill a rectangle completely with one color. Update in subclasses
 *   if
 *    desired!
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    w   Width in pixels
 *     @param    h   Height in pixels
 *    @param    color 16-bit 5-6-5 Color to fill with
 ******************************************************************************/
glib_status_t glib_fill_rect(glib_context_t *g_context,
                             int16_t x, int16_t y,
                             int16_t w, int16_t h,
                             uint16_t color);

/***************************************************************************//**
 *    @brief   Draw a rounded rectangle with no fill color
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    w   Width in pixels
 *     @param    h   Height in pixels
 *     @param    r   Radius of corner rounding
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_round_rect(glib_context_t *g_context,
                                   int16_t x, int16_t y, int16_t w, int16_t h,
                                   int16_t r, uint16_t color);

/***************************************************************************//**
 *    @brief   Draw a rounded rectangle with fill color
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    w   Width in pixels
 *     @param    h   Height in pixels
 *     @param    r   Radius of corner rounding
 *     @param    color 16-bit 5-6-5 Color to draw/fill with
 ******************************************************************************/
glib_status_t glib_fill_round_rect(glib_context_t *g_context,
                                   int16_t x, int16_t y, int16_t w, int16_t h,
                                   int16_t r, uint16_t color);

/***************************************************************************//**
 *    @brief   Draw a triangle with no fill color
 *     @param    x0  Vertex #0 x coordinate
 *     @param    y0  Vertex #0 y coordinate
 *     @param    x1  Vertex #1 x coordinate
 *     @param    y1  Vertex #1 y coordinate
 *     @param    x2  Vertex #2 x coordinate
 *     @param    y2  Vertex #2 y coordinate
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_triangle(glib_context_t *g_context,
                                 int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                 int16_t x2, int16_t y2, uint16_t color);

/***************************************************************************//**
 *    @brief     Draw a triangle with color-fill
 *     @param    x0  Vertex #0 x coordinate
 *     @param    y0  Vertex #0 y coordinate
 *     @param    x1  Vertex #1 x coordinate
 *     @param    y1  Vertex #1 y coordinate
 *     @param    x2  Vertex #2 x coordinate
 *     @param    y2  Vertex #2 y coordinate
 *     @param    color 16-bit 5-6-5 Color to fill/draw with
 ******************************************************************************/
glib_status_t glib_fill_triangle(glib_context_t *g_context,
                                 int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                 int16_t x2, int16_t y2, uint16_t color);

/***************************************************************************//**
 *     @brief
 *     Draws a char using the font supplied with the library.
 *
 *     @param g_context
 *     Pointer to the glib_context_t
 *
 *     @param my_char
 *     Char to be drawn
 *
 *     @param x
 *     Start x-coordinate for the char (Upper left corner)
 *
 *     @param y
 *     Start y-coordinate for the char (Upper left corner)
 *
 *     @param color  16-bit pixel color.
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_draw_char(glib_context_t *g_context,
                             int16_t x, int16_t y,
                             unsigned char c,
                             uint16_t color, uint16_t bg,
                             uint8_t size_x, uint8_t size_y);

/***************************************************************************//**
 *     @brief
 *     Draws a string using the font supplied with the library.
 *
 *     @param g_context
 *     Pointer to a glib_context_t
 *
 *     @param str
 *     Pointer to the string that is drawn
 *
 *     @param x0
 *     Start x-coordinate for the string (Upper left corner)
 *
 *     @param y0
 *     Start y-coordinate for the string (Upper left corner)
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_draw_string(glib_context_t *g_context, const char *str,
                               int16_t x0, int16_t y0);

/***************************************************************************//**
 *     @brief   Set text font color with custom background color
 *
 *     @param   c   16-bit 5-6-5 Color to draw text with
 *
 *     @param   bg  16-bit 5-6-5 Color to draw background/fill with
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_set_text_color(glib_context_t *g_context,
                                  uint16_t c,
                                  uint16_t bg);

/***************************************************************************//**
 *     @brief
 *     Set new font for the library. Note that GLIB defines a default font in
 *   glib.c.
 *     Redefine GLIB_DEFAULT_FONT to change the default font.
 *
 *     @param g_context
 *     Pointer to the glib_context_t
 *
 *     @param pFont
 *     Pointer to the new font
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_set_font(glib_context_t *g_context,
                            const glib_gfx_font_t *f);

/***************************************************************************//**
 *     @brief
 *     Draws a rectangle from x1, y1 to x2, y2
 *
 *     @param g_context
 *     Pointer to a glib_context_t in which the line is drawn.
 *
 *     @param x1
 *     Start x-coordinate
 *
 *     @param y1
 *     Start y-coordinate
 *
 *     @param x2
 *     End x-coordinate
 *
 *     @param y2
 *     End y-coordinate
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_draw_rectangle(glib_context_t *g_context,
                                  uint8_t x1,
                                  uint8_t y1,
                                  uint8_t x2,
                                  uint8_t y2);

/***************************************************************************//**
 *     @brief
 *     Update the display device with contents of active glib_frame_buffer.
 *
 *     @return
 *     Returns GLIB_OK is successful, error otherwise.
 ******************************************************************************/
glib_status_t glib_update_display(void);

/***************************************************************************//**
 *    @brief      Draw a RAM-resident 1-bit image at the specified (x,y)
 *   position,
 *    using the specified foreground (for set bits) and background (unset bits)
 *    colors.
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    bitmap  byte array with monochrome bitmap
 *     @param    w   Width of bitmap in pixels
 *     @param    h   Height of bitmap in pixels
 *     @param    color 16-bit 5-6-5 Color to draw pixels with
 *     @param    bg 16-bit 5-6-5 Color to draw background with
 ******************************************************************************/
glib_status_t glib_draw_bitmap(glib_context_t *g_context,
                               int16_t x, int16_t y, uint8_t *bitmap, int16_t w,
                               int16_t h, uint16_t color, uint16_t bg);

/***************************************************************************//**
 *    @brief      Draw PROGMEM-resident XBitMap Files (*.xbm), exported from
 *   GIMP.
 *    Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
 *    C Array can be directly used with this function.
 *    There is no RAM-resident version of this function; if generating bitmaps
 *    in RAM, use the format defined by drawBitmap() and call that instead.
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    bitmap  byte array with monochrome bitmap
 *     @param    w   Width of bitmap in pixels
 *     @param    h   Height of bitmap in pixels
 *     @param    color 16-bit 5-6-5 Color to draw pixels with
 ******************************************************************************/
glib_status_t glib_draw_xbitmap(glib_context_t *g_context,
                                int16_t x, int16_t y, const uint8_t *bitmap,
                                int16_t w, int16_t h, uint16_t color);

/***************************************************************************//**
 *    @brief   Draw a RAM-resident 8-bit image (grayscale) at the specified
 *   (x,y)
 *    pos. Specifically for 8-bit display devices such as IS31FL3731; no color
 *    reduction/expansion is performed.
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    bitmap  byte array with grayscale bitmap
 *     @param    w   Width of bitmap in pixels
 *     @param    h   Height of bitmap in pixels
 ******************************************************************************/
glib_status_t glib_draw_grayscale_bitmap(glib_context_t *g_context,
                                         int16_t x, int16_t y, uint8_t *bitmap,
                                         int16_t w, int16_t h);

/***************************************************************************//**
 *    @brief
 *      Set a inversion color to glib.
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_set_invert_color(void);

/***************************************************************************//**
 *    @brief
 *      Set a normal color to glib.
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_set_normal_color(void);

/***************************************************************************//**
 *    @brief
 *      Set a contrast to glib.
 *
 *    @param[in] value
 *      Value to set contrast. Select 1 out of 256 contrast steps.
 *      Contrast increases as the value increases.
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_set_contrast(uint8_t value);

/***************************************************************************//**
 *    @brief
 *      Set a Right Horizontal Scroll to glib.
 *
 *    @param[in] start_page_addr
 *      Start page address
 *
 *    @param[in] end_page_addr
 *      End page address
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_scroll_right(uint8_t start_page_addr, uint8_t end_page_addr);

/***************************************************************************//**
 *    @brief
 *      Set a Left Horizontal Scroll to glib.
 *
 *    @param[in] start_page_addr
 *      Start page address
 *
 *    @param[in] end_page_addr
 *      End page address
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_scroll_left(uint8_t start_page_addr, uint8_t end_page_addr);

/***************************************************************************//**
 *    @brief
 *      Set a Vertical and Right Horizontal Scroll to glib.
 *
 *    @param[in] start_page_addr
 *      Start page address
 *
 *    @param[in] end_page_addr
 *      End page address
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_scroll_diag_right(uint8_t start_page_addr,
                                     uint8_t end_page_addr);

/***************************************************************************//**
 *    @brief
 *      Set a Vertical and Left Horizontal Scroll to glib.
 *
 *    @param[in] start_page_addr
 *      Start page address
 *
 *    @param[in] end_page_addr
 *      End page address
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_scroll_diag_left(uint8_t start_page_addr,
                                    uint8_t end_page_addr);

/***************************************************************************//**
 *    @brief
 *      Stop scroll to glib.
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_stop_scroll(void);

/***************************************************************************//**
 *    @brief
 *      Set the display ON/OFF to glib.
 *
 *    @param[in] on
 *      State of OLED
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_enable_display(bool on);

#ifdef __cplusplus
}
#endif

#endif
