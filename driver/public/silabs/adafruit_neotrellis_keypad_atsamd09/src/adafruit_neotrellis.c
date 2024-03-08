#include "adafruit_neotrellis.h"
#include "adafruit_neotrellis_config.h"
#include "sl_sleeptimer.h"
#include "sl_udelay.h"

#define TRELLIS_MAX_DEVICE_SUPPORT 31
#define NUM_NEOTRELLIS_DEVICE      (NEOTRELLIS_NUM_COLUMN_BOARDS \
                                    * NEOTRELLIS_NUM_ROW_BOARDS)

static neotrellis_t trellis_dev[NUM_NEOTRELLIS_DEVICE];

sl_status_t adafruit_neotrellis_init(sl_i2cspm_t *i2cspm,
                                     uint8_t *trellis_addr,
                                     uint8_t addr_list_len)
{
  sl_status_t sc;

  if ((i2cspm == NULL) || (trellis_addr == NULL)
      || (addr_list_len < NUM_NEOTRELLIS_DEVICE)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  for (uint8_t i = 0; i < addr_list_len; i++) {
    if ((trellis_addr[i] < 0x2E) || (trellis_addr[i] > 0x4D)) {
      return SL_STATUS_FAIL;
    }
  }

  for (int i = 0; i < NUM_NEOTRELLIS_DEVICE; i++) {
    sc = adafruit_neopixel_init(&(trellis_dev[i].pixel),
                                NEO_TRELLIS_NUM_KEYS,
                                NEO_TRELLIS_NEOPIX_PIN,
                                NEO_GRB + NEO_KHZ800,
                                trellis_addr[i],
                                i2cspm);

    if (sc != SL_STATUS_OK) {
      return SL_STATUS_FAIL;
    }

    for (int keycount = 0; keycount < NEO_TRELLIS_NUM_KEYS; keycount++) {
      trellis_dev[i].callbacks[keycount] = NULL;
    }

    uint8_t enable_interrupt = 0x01;
    // enable keypad interrupt
    sc = adafruit_seesaw_i2c_register_write(trellis_dev->pixel.adafruit_seesaw,
                                            SEESAW_KEYPAD_BASE,
                                            SEESAW_KEYPAD_INTENSET,
                                            &enable_interrupt, 1);
    if (sc != SL_STATUS_OK) {
      return SL_STATUS_FAIL;
    }
  }

  return SL_STATUS_OK;
}

void adafruit_neotrellis_registerCallback(uint16_t num,
                                          TrellisCallback (*cb)(keyEvent))
{
  uint8_t x = num % (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);
  uint8_t y = num / (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);

  adafruit_neotrellis_registerCallback_xy(x, y, cb);
}

void adafruit_neotrellis_registerCallback_xy(uint16_t x,
                                             uint16_t y,
                                             TrellisCallback (*cb)(keyEvent))
{
  uint8_t index = (y / NEO_TRELLIS_NUM_ROWS * NEOTRELLIS_NUM_COLUMN_BOARDS)
                  + (x / NEO_TRELLIS_NUM_COLS);
  int xkey = NEO_TRELLIS_X(x);
  int ykey = NEO_TRELLIS_Y(y % NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS);
  int key = NEO_TRELLIS_XY(xkey, ykey);

  printf("register_Callback: index %d key %d\n", index, key);

  trellis_dev[index].callbacks[key] = cb;
}

void adafruit_neotrellis_unregisterCallback(uint16_t num)
{
  uint8_t x = num % (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);
  uint8_t y = num / (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);

  adafruit_neotrellis_unregisterCallback_xy(x, y);
}

void adafruit_neotrellis_unregisterCallback_xy(uint16_t x, uint16_t y)
{
  uint8_t index = (y / NEO_TRELLIS_NUM_ROWS * NEOTRELLIS_NUM_COLUMN_BOARDS)
                  + (x / NEO_TRELLIS_NUM_COLS);
  int xkey = NEO_TRELLIS_X(x);
  int ykey = NEO_TRELLIS_Y(y % NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS);
  int key = NEO_TRELLIS_XY(xkey, ykey);

  trellis_dev[index].callbacks[key] = NULL;
}

sl_status_t adafruit_neotrellis_activateKey(uint16_t num,
                                            uint8_t edge,
                                            bool enable)
{
  uint8_t x = num % (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);
  uint8_t y = num / (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);

  return adafruit_neotrellis_activateKey_xy(x, y, edge, enable);
}

sl_status_t adafruit_neotrellis_activateKey_xy(uint16_t x, uint16_t y,
                                               uint8_t edge,
                                               bool enable)
{
  uint8_t index = (y / NEO_TRELLIS_NUM_ROWS * NEOTRELLIS_NUM_COLUMN_BOARDS)
                  + (x / NEO_TRELLIS_NUM_COLS);
  int xkey = NEO_TRELLIS_X(x);
  int ykey = NEO_TRELLIS_Y(y % NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS);
  int key = NEO_TRELLIS_KEY(NEO_TRELLIS_XY(xkey, ykey));
  keyState ks;

  ks.bit.STATE = enable;
  ks.bit.ACTIVE = (1 << edge);
  uint8_t cmd[] = { key, ks.reg };

  return adafruit_seesaw_i2c_register_write(
    trellis_dev[index].pixel.adafruit_seesaw,
    SEESAW_KEYPAD_BASE,
    SEESAW_KEYPAD_EVENT,
    cmd,
    2);
}

sl_status_t adafruit_neotrellis_setPixelColor_xy(uint16_t x,
                                                 uint16_t y,
                                                 uint32_t color)
{
  uint8_t index = (y / NEO_TRELLIS_NUM_ROWS * NEOTRELLIS_NUM_COLUMN_BOARDS)
                  + (x / NEO_TRELLIS_NUM_COLS);
  int xkey = NEO_TRELLIS_X(x);
  int ykey = NEO_TRELLIS_Y(y % NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS);
  int key = NEO_TRELLIS_XY(xkey, ykey);
  printf("pixel color: index %d key %d\n", index, key);
  return adafruit_neopixel_set_pixelColor(&(trellis_dev[index].pixel),
                                          key,
                                          color);
}

sl_status_t adafruit_neotrellis_setPixelColor(uint16_t num, uint32_t color)
{
  uint8_t x = num % (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);
  uint8_t y = num / (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);

  return adafruit_neotrellis_setPixelColor_xy(x, y, color);
}

void adafruit_neotrellis_show()
{
  for (uint8_t i = 0; i < NUM_NEOTRELLIS_DEVICE; i++) {
    adafruit_neopixel_show(&(trellis_dev[i].pixel));
  }
}

void adafruit_neotrellis_read()
{
  for (int n = 0; n < NUM_NEOTRELLIS_DEVICE; n++) {
    int8_t count = adafruit_seesaw_get_keypad_count(
      trellis_dev[n].pixel.adafruit_seesaw);
    sl_udelay_wait(500);
    if (count > 0) {
      count = count + 2;
      keyEventRaw e[count];
      adafruit_seesaw_read_keypad(trellis_dev[n].pixel.adafruit_seesaw, e,
                                  count);
      for (int i = 0; i < count; i++) {
        // call any callbacks associated with the key
        e[i].bit.NUM = NEO_TRELLIS_SEESAW_KEY(e[i].bit.NUM);
        if ((e[i].bit.NUM < NEO_TRELLIS_NUM_KEYS)
            && (trellis_dev[n].callbacks[e[i].bit.NUM] != NULL)) {
          // update the event with the multitrellis number
          keyEvent evt = { { e[i].bit.EDGE, e[i].bit.NUM } };
          int x = NEO_TRELLIS_X(e[i].bit.NUM);
          int y = NEO_TRELLIS_Y(e[i].bit.NUM);

          x = x + (n % NEOTRELLIS_NUM_COLUMN_BOARDS) * NEO_TRELLIS_NUM_COLS;
          y = y + (n / NEOTRELLIS_NUM_COLUMN_BOARDS) * NEO_TRELLIS_NUM_ROWS;

          evt.bit.NUM = (y * NEO_TRELLIS_NUM_COLS
                         * NEOTRELLIS_NUM_COLUMN_BOARDS) + x;

          trellis_dev[n].callbacks[e[i].bit.NUM](evt);
        }
      }
    }
  }
}
