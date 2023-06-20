#include "mikroe_ssd1351.h"
#include "mikroe_ssd1351_config.h"
#include "third_party_hw_drivers_helpers.h"
#include "stdio.h"

static oledc_t oledc;
static oledc_cfg_t oledc_cfg;
static bool initialized = false;

// -----------------------------------------------------------------------------
// Public Function Definition
// -----------------------------------------------------------------------------

/***************************************************************************//**
 *    Initialization function.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_init(SPIDRV_Handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  THIRD_PARTY_HW_DRV_RETCODE_INIT();

  // Configure default spi instance
  oledc.spi.handle = spi_instance;

  // Call basic setup functions
  oledc_cfg_setup(&oledc_cfg);
  oledc_cfg.dc = hal_gpio_pin_name(SSD1351_DC_PORT, SSD1351_DC_PIN);
  oledc_cfg.en = hal_gpio_pin_name(SSD1351_EN_PORT, SSD1351_EN_PIN);
  oledc_cfg.rw = hal_gpio_pin_name(SSD1351_RW_PORT, SSD1351_RW_PIN);

#if defined(SSD1351_RST_PORT) && defined(SSD1351_RST_PIN)
  oledc_cfg.rst = hal_gpio_pin_name(SSD1351_RST_PORT, SSD1351_RST_PIN);
#endif

  THIRD_PARTY_HW_DRV_RETCODE_TEST(oledc_init(&oledc, &oledc_cfg));

  initialized = true;
  return THIRD_PARTY_HW_DRV_RETCODE_VALUE;
}

/***************************************************************************//**
 *    This function sets the SPI instance used by platform functions.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_set_spi_instance(SPIDRV_Handle_t spi_instance)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  oledc.spi.handle = spi_instance;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Default Configuration function.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_default_cfg(void)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_default_cfg(&oledc);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Enable/Disable display.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_enable(bool state)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_enable(&oledc, state);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Hardware Reset.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_reset(void)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_reset(&oledc);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Command Send with more argument.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_more_arg_commands(uint8_t command,
                                             uint8_t *args,
                                             uint16_t args_len)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_more_arg_commands(&oledc, command, args, args_len);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Command Send with one argument.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_one_arg_commands(uint8_t command, uint8_t args)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_one_arg_commands(&oledc, command, args);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Fill Screen.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_fill_screen(uint16_t color)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_fill_screen(&oledc, color);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Draw Rectangle.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_rectangle(uint8_t col_off,
                                     uint8_t row_off,
                                     uint8_t col_end,
                                     uint8_t row_end,
                                     uint16_t color)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_rectangle(&oledc, col_off, row_off, col_end, row_end, color);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Draw BMP Image.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_image(const uint8_t *img,
                                 uint8_t col_off,
                                 uint8_t row_off)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_image(&oledc, img, col_off, row_off);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Draw Text.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_text(uint8_t *text, uint16_t x, uint16_t y)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_text(&oledc, text, x, y);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Font Setup.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_set_font(const uint8_t *font_s, uint16_t color)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_set_font(&oledc, font_s, color);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Set Contrast.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_set_contrast(uint8_t contrast)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  uint8_t oledc_contrast[3] = { 0x8A, contrast, 0x8A };
  oledc_more_arg_commands(&oledc, OLEDC_CONTRAST, oledc_contrast, 3);
  return SL_STATUS_OK;
}
