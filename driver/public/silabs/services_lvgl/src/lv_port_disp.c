/**
 * @file lv_port_disp.c
 *
 */

/* Copy this file as "lv_port_disp.c"
 * and set this value to "1" to enable content */
#if 1

#include <stdbool.h>
#include "lv_port_disp.h"
#include "lvgl_display.h"

static lvgl_display_t *disp;

static void disp_init(void);
static void disp_flush(lv_disp_drv_t *disp_drv,
                       const lv_area_t *area,
                       lv_color_t *color_p);

// static void gpu_fill(lv_disp_drv_t *disp_drv,
//                      lv_color_t *dest_buf,
//                      lv_coord_t dest_width,
//                      const lv_area_t *fill_area,
//                      lv_color_t color);
static void flush_area_complete_callback(void *arg);

void lv_port_disp_init(void)
{
  /*-------------------------
  * Initialize your display
  * -----------------------*/
  disp_init();

  lvgl_display_init();
  disp = lvgl_display_get();

  /*-----------------------------
  * Create a buffer for drawing
  *----------------------------*/

  /**
   * LVGL requires a buffer where it internally draws the widgets.
   * Later this buffer will passed to your display driver's `flush_cb`
   * to copy its content to your display.
   * The buffer has to be greater than 1 display row
   *
   * There are 3 buffering configurations:
   * 1. Create ONE buffer:
   *      LVGL will draw the display's content here
   *      and writes it to your display
   *
   * 2. Create TWO buffer:
   *      + LVGL will draw the display's content to a buffer
   *        and writes it your display.
   *      + You should use DMA to write the buffer's content to the display.
   *      + It will enable LVGL to draw the next part of the screen
   *        to the other buffer while the data is being sent form
   *        the first buffer. It makes rendering and flushing parallel.
   *
   * 3. Double buffering
   *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
   *      This way LVGL will always provide the whole rendered screen in
   *      `flush_cb` and you only need to change the frame buffer's address.
   */

#if LV_BUFFER_MODE == 0
  static lv_disp_draw_buf_t draw_buf_dsc_1;
  static lv_color_t buf_1[LVGL_TFT_WIDTH * 20];   /* A buffer for 20 rows */
  /* Initialize the display buffer */
  lv_disp_draw_buf_init(&draw_buf_dsc_1,
                        buf_1,
                        NULL,
                        LVGL_TFT_WIDTH * 20);

#elif LV_BUFFER_MODE == 1
  static lv_disp_draw_buf_t draw_buf_dsc_2;

  /* A buffer for 20 rows */
  static lv_color_t buf_2_1[LVGL_TFT_WIDTH * 20];

  /* An other buffer for 20 rows */
  static lv_color_t buf_2_2[LVGL_TFT_WIDTH * 20];

  /* Initialize the display buffer */
  lv_disp_draw_buf_init(&draw_buf_dsc_2,
                        buf_2_1,
                        buf_2_2,
                        LVGL_TFT_WIDTH * 20);

#elif LV_BUFFER_MODE == 2

  /* Example for 3) also set disp_drv.full_refresh = 1 below */
  static lv_disp_draw_buf_t draw_buf_dsc_3;

  /* A screen sized buffer */
  static lv_color_t buf_3_1[LVGL_TFT_WIDTH * LVGL_TFT_HEIGHT];

  /* Another screen sized buffer */
  static lv_color_t buf_3_2[LVGL_TFT_WIDTH * LVGL_TFT_HEIGHT];

  /* Initialize the display buffer */
  lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
                        LVGL_TFT_HEIGHT * LVGL_TFT_WIDTH);
#endif

  /*-----------------------------------
  * Register the display in LVGL
  *----------------------------------*/

  static lv_disp_drv_t disp_drv;   /* Descriptor of a display driver */
  lv_disp_drv_init(&disp_drv);   /* Basic initialization */

  /* Set up the functions to access to your display */

  /* Set the resolution of the display */
  disp_drv.hor_res = disp->width;
  disp_drv.ver_res = disp->height;

  /* Used to copy the buffer's content to the display */
  disp_drv.flush_cb = disp_flush;

  /* Set a display buffer */
#if LV_BUFFER_MODE == 0
  disp_drv.draw_buf = &draw_buf_dsc_1;
#elif LV_BUFFER_MODE == 1
  disp_drv.draw_buf = &draw_buf_dsc_2;
#elif LV_BUFFER_MODE == 2
  disp_drv.draw_buf = &draw_buf_dsc_3;
#endif

  disp_drv.rotated = LV_DISPLAY_ROTATION;
  disp_drv.sw_rotate = LV_ENABLE_SOFTWARE_ROTATION;

  /* Required for Example 3) */
  // disp_drv.full_refresh = 1;

  /* Fill a memory array with a color if you have GPU.
   * Note that, in lv_conf.h you can enable GPUs that has built-in support in
   *   LVGL.
   * But if you have a different GPU you can use with this callback.*/
  // disp_drv.gpu_fill_cb = gpu_fill;

  /* Finally register the driver */
  lv_disp_drv_register(&disp_drv);
}

/* Initialize your display and the required peripherals. */
static void disp_init(void)
{
  /* You code here */
}

static volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process)
 * when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
  disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process)
 * when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
  disp_flush_enabled = false;
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation
 * in the background but 'lv_disp_flush_ready()'
 * has to be called when finished. */
static void disp_flush(lv_disp_drv_t *disp_drv,
                       const lv_area_t *area,
                       lv_color_t *color_p)
{
  if (disp_flush_enabled) {
    /* The most simple case (but also the slowest)
     * to put all pixels to the screen one-by-one */

    if (disp->driver->flush_area_rgb565) {
      disp->driver->flush_area_rgb565(area->x1, area->y1,
                                      area->x2, area->y2,
                                      (uint8_t *) color_p,
#if LV_COLOR_16_SWAP == 0
                                      false,
#else
                                      true,
#endif
                                      flush_area_complete_callback,
                                      disp_drv);
    }
  }
}

static void flush_area_complete_callback(void *arg)
{
  if (arg != NULL) {
    lv_disp_drv_t *pdisp_drv = (lv_disp_drv_t *)arg;
    lv_disp_flush_ready(pdisp_drv);
  }
}

/* OPTIONAL: GPU INTERFACE */

/* If your MCU has hardware accelerator (GPU)
 * then you can use it to fill a memory with a color */
// static void gpu_fill(lv_disp_drv_t *disp_drv,
//                      lv_color_t *dest_buf,
//                      lv_coord_t dest_width,
//                      const lv_area_t *fill_area,
//                      lv_color_t color)
// {
//    /* It's an example code which should be done by your GPU */
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /* Go to the first line */
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf += dest_width;    /* Go to the next line */
//    }
// }

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
