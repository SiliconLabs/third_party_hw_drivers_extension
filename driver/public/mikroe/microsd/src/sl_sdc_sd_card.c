/***************************************************************************//**
 * @file sl_sdc_sd_card.c
 * @brief Storage Device Controls SD Card
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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
#include "sl_sdc_sd_card.h"
#include "mikroe_microsd_config.h"

typedef struct {
  uint8_t mosiPort;
  uint8_t mosiPin;
  uint8_t misoPort;
  uint8_t misoPin;
  uint8_t clkPort;
  uint8_t clkPin;
  uint8_t csPort;
  uint8_t csPin;
} SPI_Pins_t;

static Ecode_t GetSpiPins(SPIDRV_Handle_t handle, SPI_Pins_t *pins);

// SPI bit rate controls
// Set slow clock for card initialization (100k-400k)
#define FCLK_SLOW() \
  sdc_platform_set_bit_rate(sdc_spi_handle, MIKROE_MICROSD_MMC_SLOW_CLOCK)
// Set fast clock for generic read/write
#if MIKROE_MICROSD_MMC_FAST_CLOCK == 0
#define FCLK_FAST()                                              \
  if (sdc_spi_handle) {                                          \
    sdc_platform_set_bit_rate(sdc_spi_handle,                    \
                              sdc_spi_handle->initData.bitRate); \
  }
#else
#define FCLK_SLOW()   sdc_platform_set_bit_rate(sdc_spi_handle, \
                                                MIKROE_MICROSD_MMC_FAST_CLOCK)
#endif

// Socket controls
// MMC CS = L
#define CS_LOW()      // CS controlled by the SPI driver
// MMC CS = H
#define CS_HIGH()     // CS controlled by the SPI driver

#if defined(MIKROE_MICROSD_MMC_CD_PORT) && defined(MIKROE_MICROSD_MMC_CD_PORT)
// Card detected      (yes:true, no:false, default:true)
#define MMC_CD        (!GPIO_PinInGet(MIKROE_MICROSD_MMC_CD_PORT, \
                                      MIKROE_MICROSD_MMC_CD_PIN))
#endif

// Definitions for MMC/SDC command
#define CMD0          (0)     // GO_IDLE_STATE
#define CMD1          (1)     // SEND_OP_COND (MMC)
#define ACMD41        (0x80 + 41) // SEND_OP_COND (SDC)
#define CMD8          (8)     // SEND_IF_COND
#define CMD9          (9)     // SEND_CSD
#define CMD10         (10)    // SEND_CID
#define CMD12         (12)    // STOP_TRANSMISSION
#define ACMD13        (0x80 + 13) // SD_STATUS (SDC)
#define CMD16         (16)    // SET_BLOCKLEN
#define CMD17         (17)    // READ_SINGLE_BLOCK
#define CMD18         (18)    // READ_MULTIPLE_BLOCK
#define CMD23         (23)    // SET_BLOCK_COUNT (MMC)
#define ACMD23        (0x80 + 23) // SET_WR_BLK_ERASE_COUNT (SDC)
#define CMD24         (24)    // WRITE_BLOCK
#define CMD25         (25)    // WRITE_MULTIPLE_BLOCK
#define CMD32         (32)    // ERASE_ER_BLK_START
#define CMD33         (33)    // ERASE_ER_BLK_END
#define CMD38         (38)    // ERASE
#define CMD48         (48)    // READ_EXTR_SINGLE
#define CMD49         (49)    // WRITE_EXTR_SINGLE
#define CMD55         (55)    // APP_CMD
#define CMD58         (58)    // READ_OCR

static volatile DSTATUS sd_card_status = STA_NOINIT; // Disk status
static BYTE sd_card_type; // Card type flags
static volatile UINT sd_card_timer_1, sd_card_timer_2; // 1kHz decrement timer

static SPIDRV_Handle_t sdc_spi_handle = NULL;
static sl_sleeptimer_timer_handle_t disk_timerproc_timer_handle;
static void disk_timerproc_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                                          void *data);

static bool wait_ready(UINT wt);
static void deselect (void);
static bool select(void);
static bool rcvr_datablock(BYTE *buff, UINT btr);

#if FF_FS_READONLY == 0
static bool xmit_datablock(const BYTE *buff, BYTE token);

#endif
static BYTE send_cmd(BYTE cmd, DWORD arg);

/***************************************************************************//**
 * @brief
 *   Wait for card ready.
 *
 * @param[in] wt
 *   Timeout [ms]
 *
 * @return 1:Ready, 0:Timeout
 ******************************************************************************/
static bool wait_ready(UINT wt)
{
  BYTE data;

  sd_card_timer_2 = wt;
  do {
    sdc_xchg_spi(sdc_spi_handle, 0xff, &data);
    // This loop takes a time. Insert rot_rdq() here for multitask envilonment.
  } while (data != 0xff && sd_card_timer_2);  // Wait for card goes ready or
                                              //   timeout

  return (data == 0xff) ? 1 : 0;
}

/***************************************************************************//**
 * @brief
 *   Deselect card and release SPI.
 *
 ******************************************************************************/
static void deselect(void)
{
  BYTE data;

  CS_HIGH();
  sdc_xchg_spi(sdc_spi_handle, 0xff, &data); // Dummy clock (force DO hi-z for
                                             //   multiple slave SPI)
}

/***************************************************************************//**
 * @brief
 *   Select card and wait for ready.
 *
 * @return 1:OK, 0:Timeout
 ******************************************************************************/
static bool select(void)
{
  BYTE data;

  CS_LOW();
  // Dummy clock (force DO enabled)
  sdc_xchg_spi(sdc_spi_handle, 0xff, &data);

  if (wait_ready(500)) {
    return 1;  // Wait for card ready
  }

  deselect();
  return 0; // Timeout
}

/***************************************************************************//**
 * @brief
 *   Receive a data packet from MMC.
 *
 * @param[in] buff
 *   Pointer to Data buffer to store received data
 *
 * @param[in] btr
 *   Byte count (must be multiple of 4)
 *
 * @return 1:OK, 0:Failed
 ******************************************************************************/
static bool rcvr_datablock(BYTE *buff, UINT btr)
{
  BYTE token;

  sd_card_timer_1 = 100;
  do { // Wait for data packet in timeout of 100ms
    sdc_xchg_spi(sdc_spi_handle, 0xff, &token);
  } while ((token == 0xff) && sd_card_timer_1);

  // If not valid data token, return with error
  if (token != 0xfe) {
    return 0;
  }

  sdc_rcvr_spi_multi(sdc_spi_handle, buff, btr); // Receive the data block into
                                                 //   buffer
  // Discard 2 byte-CRC.
  // Refer to http://elm-chan.org/docs/mmc/mmc_e.html#dataxfer for details"
  sdc_xchg_spi(sdc_spi_handle, 0xff, &token);
  sdc_xchg_spi(sdc_spi_handle, 0xff, &token);

  return 1;
}

/***************************************************************************//**
 * @brief
 *   Send a data packet to MMC.
 *
 * @param[in] buff
 *   Pointer to 512 byte data block to be transmitted
 *
 * @param[in] token
 *   Data token
 *
 * @return 1:OK, 0:Failed
 ******************************************************************************/
#if FF_FS_READONLY == 0
static bool xmit_datablock(const BYTE *buff, BYTE token)
{
  BYTE data;

  if (!wait_ready(500)) {
    return 0;
  }

  sdc_xchg_spi(sdc_spi_handle, token, &data);      // Xmit a token
  if (token != 0xfd) {             // Not StopTran token
    sdc_xmit_spi_multi(sdc_spi_handle, buff, 512); // Xmit the data block to the
                                                   //   MMC
    // Discard 2 byte-CRC.
    // Refer to http://elm-chan.org/docs/mmc/mmc_e.html#dataxfer for details"
    sdc_xchg_spi(sdc_spi_handle, 0xff, &data);
    sdc_xchg_spi(sdc_spi_handle, 0xff, &data);
    sdc_xchg_spi(sdc_spi_handle, 0xff, &data);     // Receive a data response
    // If not accepted, return with error
    if ((data & 0x1F) != 0x05) {
      return 0;
    }
  }

  return 1;
}

#endif

/***************************************************************************//**
 * @brief
 *   Send a command packet to MMC.
 *
 * @param[in] cmd
 *   Command index
 *
 * @param[in] arg
 *   Argument
 *
 * @return Status of Disk Functions
 ******************************************************************************/
static BYTE send_cmd(BYTE cmd, DWORD arg)
{
  BYTE n, data;

  // ACMD<n> is the command sequense of CMD55-CMD<n>
  if (cmd & 0x80) {
    cmd &= 0x7f;
    data = send_cmd(CMD55, 0);
    if (data > 1) {
      return data;
    }
  }

  // Select the card and wait for ready except to stop multiple block read
  if (cmd != CMD12) {
    deselect();
    if (!select()) {
      return 0xff;
    }
  }

  // Send command packet
  sdc_xchg_spi(sdc_spi_handle, 0x40 | cmd, &data); // Start + Command index
  sdc_xchg_spi(sdc_spi_handle, ((BYTE)(arg >> 24)), &data); // Argument[31..24]
  sdc_xchg_spi(sdc_spi_handle, ((BYTE)(arg >> 16)), &data); // Argument[23..16]
  sdc_xchg_spi(sdc_spi_handle, ((BYTE)(arg >> 8)), &data);  // Argument[15..8]
  sdc_xchg_spi(sdc_spi_handle, (BYTE)(arg), &data);         // Argument[7..0]

  n = 0x01;           // Dummy CRC + Stop
  if (cmd == CMD0) {
    n = 0x95;         // Valid CRC for CMD0(0) + Stop
  }
  if (cmd == CMD8) {
    n = 0x87;         // Valid CRC for CMD8(0x1AA) + Stop
  }
  sdc_xchg_spi(sdc_spi_handle, n, &data);

  // Receive command response
  if (cmd == CMD12) {
    // Skip a stuff byte on stop to read
    sdc_xchg_spi(sdc_spi_handle, 0xff, &data);
  }
  n = 10;             // Wait for a valid response in timeout of 10 attempts
  do {
    sdc_xchg_spi(sdc_spi_handle, 0xff, &data);
  } while ((data & 0x80) && --n);

  return data;     // Return with the response value
}

/***************************************************************************//**
 * Inidialize an SD Card.
 ******************************************************************************/
DSTATUS sd_card_disk_initialize(void)
{
  BYTE n, cmd, ty, ocr[4], data;

  for (sd_card_timer_1 = 10; sd_card_timer_1;) {  // Wait for 10ms
  }
  if (sd_card_status & STA_NODISK) {
    return sd_card_status; // Is card existing in the soket?
  }

  FCLK_SLOW();
  for (n = 10; n; n--) {
    sdc_xchg_spi(sdc_spi_handle, 0xff, &data);  // Send 80 dummy clocks
  }

  ty = 0;
  if (send_cmd(CMD0, 0) == 1) {       // Put the card SPI mode
    sd_card_timer_1 = 1000;           // Initialization timeout = 1 sec
    if (send_cmd(CMD8, 0x1aa) == 1) { // Is the card SDv2?
      for (n = 0; n < 4; n++) {
        // Get 32 bit return value of R7 resp
        sdc_xchg_spi(sdc_spi_handle, 0xff, &ocr[n]);
      }

      // Is the card supports vcc of 2.7-3.6V?
      if ((ocr[2] == 0x01) && (ocr[3] == 0xaa)) {
        // Wait for end of initialization with ACMD41(HCS)
        while (sd_card_timer_1 && send_cmd(ACMD41, 1UL << 30)) {
        }

        // Check CCS bit in the OCR
        if (sd_card_timer_1 && (send_cmd(CMD58, 0) == 0)) {
          for (n = 0; n < 4; n++) {
            sdc_xchg_spi(sdc_spi_handle, 0xff, &ocr[n]);
          }
          ty = (ocr[0] & 0x40) ? CT_SDC2 | CT_BLOCK : CT_SDC2;  // Card id SDv2
        }
      }
    } else {  // Not SDv2 card
      if (send_cmd(ACMD41, 0) <= 1) {   // SDv1 or MMC?
        ty = CT_SDC1;
        cmd = ACMD41;     // SDv1 (ACMD41(0))
      } else {
        ty = CT_MMC3;
        cmd = CMD1;       // MMCv3 (CMD1(0))
      }

      // Wait for end of initialization
      while (sd_card_timer_1 && send_cmd(cmd, 0)) {
      }

      // Set block length: 512
      if ((!sd_card_timer_1) || (send_cmd(CMD16, 512) != 0)) {
        ty = 0;
      }
    }
  }
  sd_card_type = ty;   // Card type
  deselect();

  if (ty) {        // OK
    FCLK_FAST();   // Set fast clock
    sd_card_status &= ~STA_NOINIT;  // Clear STA_NOINIT flag
  } else {         // Failed
    sd_card_status = STA_NOINIT;
  }

  return sd_card_status;
}

/***************************************************************************//**
 * Get SD Card Status.
 ******************************************************************************/
DSTATUS sd_card_disk_status(void)
{
  return sd_card_status;
}

/***************************************************************************//**
 * Read Sector(s) from SD Card.
 ******************************************************************************/
dresult_t sd_card_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
  DWORD sect = (DWORD)sector;

  // Check parameter
  if (!count) {
    return RES_PARERR;
  }

  // Check if drive is ready
  if (sd_card_status & STA_NOINIT) {
    return RES_NOTRDY;
  }

  // LBA ot BA conversion (byte addressing cards)
  if (!(sd_card_type & CT_BLOCK)) {
    sect *= 512;
  }

  // Single block read
  if (count == 1) {
    if ((send_cmd(CMD17, sect) == 0)  // READ_SINGLE_BLOCK
        && rcvr_datablock(buff, 512)) {
      count = 0;
    }
  } else { // Multiple block read
    if (send_cmd(CMD18, sect) == 0) { // READ_MULTIPLE_BLOCK
      do {
        if (!rcvr_datablock(buff, 512)) {
          break;
        }
        buff += 512;
      } while (--count);
      send_cmd(CMD12, 0); // STOP_TRANSMISSION
    }
  }
  deselect();

  return count ? RES_ERROR : RES_OK;
}

/***************************************************************************//**
 * Write Sector(s) to SD Card.
 ******************************************************************************/
#if FF_FS_READONLY == 0
dresult_t sd_card_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
  DWORD sect = (DWORD)sector;

  if (sd_card_status & STA_NOINIT) { // Check drive status
    return RES_NOTRDY;
  }
  if (sd_card_status & STA_PROTECT) { // Check write protect
    return RES_WRPRT;
  }

  // LBA ==> BA conversion (byte addressing cards)
  if (!(sd_card_type & CT_BLOCK)) {
    sect *= 512;
  }

  if (count == 1) { // Single sector write
    if ((send_cmd(CMD24, sect) == 0)  // WRITE_BLOCK
        && xmit_datablock(buff, 0xFE)) {
      count = 0;
    }
  } else { // Multiple sector write
    if (sd_card_type & CT_SDC) {
      send_cmd(ACMD23, count); // Predefine number of sectors
    }
    if (send_cmd(CMD25, sect) == 0) { // WRITE_MULTIPLE_BLOCK
      do {
        if (!xmit_datablock(buff, 0xFC)) {
          break;
        }
        buff += 512;
      } while (--count);

      if (!xmit_datablock(0, 0xFD)) { // STOP_TRAN token
        count = 1;
      }
    }
  }
  deselect();

  return count ? RES_ERROR : RES_OK;
}

#endif

/***************************************************************************//**
 * This function is called to control device specific features
 * and miscellaneous functions other than generic read/write.
 ******************************************************************************/
dresult_t sd_card_disk_ioctl(BYTE cmd, void *buff)
{
  dresult_t res;
  BYTE n;
  BYTE csd[16];
  BYTE *ptr = buff;
  BYTE data;
  DWORD csize;
#ifdef FF_USE_TRIM
  LBA_t *range;
  DWORD st;
  DWORD ed;
#endif

  if (sd_card_status & STA_NOINIT) {
    return RES_NOTRDY;
  }

  res = RES_ERROR;
  switch (cmd) {
    // Make sure that no pending write process.
    // Do not remove this or written sector might not left updated.
    case CTRL_SYNC:
      if (select()) {
        res = RES_OK;
      }
      deselect();
      break;

    // Get number of sectors on the disk (DWORD)
    case GET_SECTOR_COUNT:
      if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
        if ((csd[0] >> 6) == 1) { // SDC ver 2.00
          csize = csd[9]
                  + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
          *(LBA_t *)buff = csize << 10;
        } else {          // SDC ver 1.XX or MMC
          n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
          csize =
            (csd[8] >>
             6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
          *(LBA_t *)buff = csize << (n - 9);
        }
        res = RES_OK;
      }
      deselect();
      break;

    // Get erase block size in unit of sector (DWORD)
    case GET_BLOCK_SIZE:
      if (sd_card_type & CT_SDC2) {         // SDv2?
        if (send_cmd(ACMD13, 0) == 0) { // Read SD status
          sdc_xchg_spi(sdc_spi_handle, 0xff, &data);
          if (rcvr_datablock(csd, 16)) {// Read partial block
            for (n = 64 - 16; n; n--) {
              // Purge trailing data
              sdc_xchg_spi(sdc_spi_handle, 0xff, &data);
            }
            *(DWORD *)buff = 16UL << (csd[10] >> 4);
            res = RES_OK;
          }
        }
      } else {                          // SDv1 or MMCv3
        if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) { // Read CSD
          if (sd_card_type & CT_SDC1) { // SDv1
            *(DWORD *)buff =
              (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) <<
                ((csd[13] >> 6) - 1);
          } else {          // MMCv3
            *(DWORD *)buff =
              ((WORD)((csd[10] & 124) >>
                      2) + 1)
              * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
          }
          res = RES_OK;
        }
      }
      deselect();
      break;

#ifdef FF_USE_TRIM
    // Erase a block of sectors (used when _USE_TRIM in ffconf.h is 1)
    case CTRL_TRIM:
      // Check if the card is SDC
      if (!(sd_card_type & CT_SDC)) {
        break;
      }
      // Get CSD
      if (sd_card_disk_ioctl(MMC_GET_CSD, csd)) {
        break;
      }
      // Check if ERASE_BLK_EN = 1
      if (!(csd[10] & 0x40)) {
        break;
      }
      range = buff;
      st = (DWORD)range[0];
      ed = (DWORD)range[1]; // Load sector block
      if (!(sd_card_type & CT_BLOCK)) {
        st *= 512;
        ed *= 512;
      }
      // Erase sector block
      if ((send_cmd(CMD32, st) == 0)
          && (send_cmd(CMD33, ed) == 0)
          && (send_cmd(CMD38, 0) == 0)
          && wait_ready(60000)) {
        res = RES_OK; // FatFs does not check result of this command
      }
      break;
#endif

    // Following commands are never used by FatFs module
    // Get card type flags (1 byte)
    case MMC_GET_TYPE:
      *ptr = sd_card_type;
      res = RES_OK;
      break;

    // Receive CSD as a data block (16 bytes)
    case MMC_GET_CSD:
      // READ_CSD
      if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(ptr, 16)) {
        res = RES_OK;
      }
      deselect();
      break;

    // Receive CID as a data block (16 bytes)
    case MMC_GET_CID:
      // READ_CID
      if ((send_cmd(CMD10, 0) == 0) && rcvr_datablock(ptr, 16)) {
        res = RES_OK;
      }
      deselect();
      break;

    // Receive OCR as an R3 resp (4 bytes)
    case MMC_GET_OCR:
      // READ_OCR
      if (send_cmd(CMD58, 0) == 0) {
        for (n = 4; n; n--) {
          *ptr++ = sdc_xchg_spi(sdc_spi_handle, 0xff, &data);
        }
        res = RES_OK;
      }
      deselect();
      break;

    // Receive SD status as a data block (64 bytes)
    case MMC_GET_SDSTAT:
      // SD_STATUS
      if (send_cmd(ACMD13, 0) == 0) {
        sdc_xchg_spi(sdc_spi_handle, 0xff, &data);
        if (rcvr_datablock(ptr, 64)) {
          res = RES_OK;
        }
      }
      deselect();
      break;

    default:
      res = RES_PARERR;
      break;
  }

  return res;
}

/***************************************************************************//**
 * @brief Initialize SPI interface for SD Card.
 ******************************************************************************/
sl_status_t sd_card_spi_init(SPIDRV_Handle_t spi_handle)
{
  bool timer_is_running = false;
  sdc_spi_handle = spi_handle;

  SPI_Pins_t pins;
  Ecode_t ret;

#if defined(MIKROE_MICROSD_MMC_CD_PORT) && defined(MIKROE_MICROSD_MMC_CD_PORT)
  GPIO_PinModeSet(MIKROE_MICROSD_MMC_CD_PORT,
                  MIKROE_MICROSD_MMC_CD_PIN,
                  gpioModeInputPull,
                  1);
#endif

  // MISO pin is not pulled-up by SDcard click board side with a resistor.
  // This pin should be reconfigured in gpioModeInputPull mode.
  ret = GetSpiPins(spi_handle, &pins);
  if (ret != ECODE_EMDRV_SPIDRV_OK) {
    return ret;
  }
  GPIO_PinModeSet(pins.misoPort,
                  pins.misoPin,
                  gpioModeInputPull,
                  1);

  // Make sure the disk_timerproc_timer_handle timer is initialized only once
  sl_sleeptimer_is_timer_running(&disk_timerproc_timer_handle,
                                 &timer_is_running);
  if (timer_is_running == false) {
    // Start a periodic timer 1 ms to generate card control timing
    sl_sleeptimer_start_periodic_timer_ms(&disk_timerproc_timer_handle,
                                          1,
                                          disk_timerproc_timer_callback,
                                          (void *)NULL,
                                          0,
                                          0);
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Sleeptimer callback function to generate card control timing.
 ******************************************************************************/
static void disk_timerproc_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                                          void *data)
{
  (void)handle;
  (void)data;

  disk_timerproc();
}

/***************************************************************************//**
 * Device timer function.
 * This function must be called from timer interrupt routine in period
 * of 1 ms to generate card control timing.
 ******************************************************************************/
void disk_timerproc(void)
{
  WORD n;
  BYTE s;

  n = sd_card_timer_1; // 1kHz decrement timer stopped at 0
  if (n) {
    sd_card_timer_1 = --n;
  }

  n = sd_card_timer_2;
  if (n) {
    sd_card_timer_2 = --n;
  }

  s = sd_card_status;

#ifdef MMC_CD
  if (MMC_CD) {      // Card is in socket
    s &= ~STA_NODISK;
  } else {           // Socket empty
    s |= (STA_NODISK | STA_NOINIT);
  }
#else
  s &= ~STA_NODISK;
#endif
  sd_card_status = s;
}

#if defined(_SILICON_LABS_32B_SERIES_0)

/***************************************************************************//**
 * @brief Get SPI pins for Series 0 devices.
 ******************************************************************************/
static Ecode_t GetSpiPins(SPIDRV_Handle_t handle, SPI_Pins_t *pins)
{
  uint32_t location;

  location = handle->initData.portLocation;

  if (0) {
#if defined(USART0)
  } else if (handle->peripheral.usartPort == USART0) {
    pins->mosiPort = AF_USART0_TX_PORT(location);
    pins->misoPort = AF_USART0_RX_PORT(location);
    pins->clkPort = AF_USART0_CLK_PORT(location);
    pins->csPort = AF_USART0_CS_PORT(location);
    pins->mosiPin = AF_USART0_TX_PIN(location);
    pins->misoPin = AF_USART0_RX_PIN(location);
    pins->clkPin = AF_USART0_CLK_PIN(location);
    pins->csPin = AF_USART0_CS_PIN(location);
#endif
#if defined(USART1)
  } else if (handle->peripheral.usartPort == USART1) {
    pins->mosiPort = AF_USART1_TX_PORT(location);
    pins->misoPort = AF_USART1_RX_PORT(location);
    pins->clkPort = AF_USART1_CLK_PORT(location);
    pins->csPort = AF_USART1_CS_PORT(location);
    pins->mosiPin = AF_USART1_TX_PIN(location);
    pins->misoPin = AF_USART1_RX_PIN(location);
    pins->clkPin = AF_USART1_CLK_PIN(location);
    pins->csPin = AF_USART1_CS_PIN(location);
#endif
#if defined(USART2)
  } else if (handle->peripheral.usartPort == USART2) {
    pins->mosiPort = AF_USART2_TX_PORT(location);
    pins->misoPort = AF_USART2_RX_PORT(location);
    pins->clkPort = AF_USART2_CLK_PORT(location);
    pins->csPort = AF_USART2_CS_PORT(location);
    pins->mosiPin = AF_USART2_TX_PIN(location);
    pins->misoPin = AF_USART2_RX_PIN(location);
    pins->clkPin = AF_USART2_CLK_PIN(location);
    pins->csPin = AF_USART2_CS_PIN(location);
#endif
#if defined(USARTRF0)
  } else if (handle->peripheral.usartPort == USARTRF0) {
    pins->mosiPort = AF_USARTRF0_TX_PORT(location);
    pins->misoPort = AF_USARTRF0_RX_PORT(location);
    pins->clkPort = AF_USARTRF0_CLK_PORT(location);
    pins->csPort = AF_USARTRF0_CS_PORT(location);
    pins->mosiPin = AF_USARTRF0_TX_PIN(location);
    pins->misoPin = AF_USARTRF0_RX_PIN(location);
    pins->clkPin = AF_USARTRF0_CLK_PIN(location);
    pins->csPin = AF_USARTRF0_CS_PIN(location);
#endif
#if defined(USARTRF1)
  } else if (handle->peripheral.usartPort == USARTRF1) {
    pins->mosiPort = AF_USARTRF1_TX_PORT(location);
    pins->misoPort = AF_USARTRF1_RX_PORT(location);
    pins->clkPort = AF_USARTRF1_CLK_PORT(location);
    pins->csPort = AF_USARTRF1_CS_PORT(location);
    pins->mosiPin = AF_USARTRF1_TX_PIN(location);
    pins->misoPin = AF_USARTRF1_RX_PIN(location);
    pins->clkPin = AF_USARTRF1_CLK_PIN(location);
    pins->csPin = AF_USARTRF1_CS_PIN(location);
#endif
  } else {
    return ECODE_EMDRV_SPIDRV_PARAM_ERROR;
  }
  return ECODE_EMDRV_SPIDRV_OK;
}

#endif

#if defined(_SILICON_LABS_32B_SERIES_1)

/***************************************************************************//**
 * @brief Get SPI pins for Series 1 devices.
 ******************************************************************************/
static Ecode_t GetSpiPins(SPIDRV_Handle_t handle, SPI_Pins_t *pins)
{
  if (0) {
#if defined(USART0)
  } else if (handle->peripheral.usartPort == USART0) {
    pins->mosiPort = AF_USART0_TX_PORT(handle->initData.portLocationTx);
    pins->misoPort = AF_USART0_RX_PORT(handle->initData.portLocationRx);
    pins->clkPort = AF_USART0_CLK_PORT(handle->initData.portLocationClk);
    pins->csPort = AF_USART0_CS_PORT(handle->initData.portLocationCs);
    pins->mosiPin = AF_USART0_TX_PIN(handle->initData.portLocationTx);
    pins->misoPin = AF_USART0_RX_PIN(handle->initData.portLocationRx);
    pins->clkPin = AF_USART0_CLK_PIN(handle->initData.portLocationClk);
    pins->csPin = AF_USART0_CS_PIN(handle->initData.portLocationCs);
#endif
#if defined(USART1)
  } else if (handle->peripheral.usartPort == USART1) {
    pins->mosiPort = AF_USART1_TX_PORT(handle->initData.portLocationTx);
    pins->misoPort = AF_USART1_RX_PORT(handle->initData.portLocationRx);
    pins->clkPort = AF_USART1_CLK_PORT(handle->initData.portLocationClk);
    pins->csPort = AF_USART1_CS_PORT(handle->initData.portLocationCs);
    pins->mosiPin = AF_USART1_TX_PIN(handle->initData.portLocationTx);
    pins->misoPin = AF_USART1_RX_PIN(handle->initData.portLocationRx);
    pins->clkPin = AF_USART1_CLK_PIN(handle->initData.portLocationClk);
    pins->csPin = AF_USART1_CS_PIN(handle->initData.portLocationCs);
#endif
#if defined(USART2)
  } else if (handle->peripheral.usartPort == USART2) {
    pins->mosiPort = AF_USART2_TX_PORT(handle->initData.portLocationTx);
    pins->misoPort = AF_USART2_RX_PORT(handle->initData.portLocationRx);
    pins->clkPort = AF_USART2_CLK_PORT(handle->initData.portLocationClk);
    pins->csPort = AF_USART2_CS_PORT(handle->initData.portLocationCs);
    pins->mosiPin = AF_USART2_TX_PIN(handle->initData.portLocationTx);
    pins->misoPin = AF_USART2_RX_PIN(handle->initData.portLocationRx);
    pins->clkPin = AF_USART2_CLK_PIN(handle->initData.portLocationClk);
    pins->csPin = AF_USART2_CS_PIN(handle->initData.portLocationCs);
#endif
#if defined(USART3)
  } else if (handle->peripheral.usartPort == USART3) {
    pins->mosiPort = AF_USART3_TX_PORT(handle->initData.portLocationTx);
    pins->misoPort = AF_USART3_RX_PORT(handle->initData.portLocationRx);
    pins->clkPort = AF_USART3_CLK_PORT(handle->initData.portLocationClk);
    pins->csPort = AF_USART3_CS_PORT(handle->initData.portLocationCs);
    pins->mosiPin = AF_USART3_TX_PIN(handle->initData.portLocationTx);
    pins->misoPin = AF_USART3_RX_PIN(handle->initData.portLocationRx);
    pins->clkPin = AF_USART3_CLK_PIN(handle->initData.portLocationClk);
    pins->csPin = AF_USART3_CS_PIN(handle->initData.portLocationCs);
#endif
#if defined(USART4)
  } else if (handle->peripheral.usartPort == USART4) {
    pins->mosiPort = AF_USART4_TX_PORT(handle->initData.portLocationTx);
    pins->misoPort = AF_USART4_RX_PORT(handle->initData.portLocationRx);
    pins->clkPort = AF_USART4_CLK_PORT(handle->initData.portLocationClk);
    pins->csPort = AF_USART4_CS_PORT(handle->initData.portLocationCs);
    pins->mosiPin = AF_USART4_TX_PIN(handle->initData.portLocationTx);
    pins->misoPin = AF_USART4_RX_PIN(handle->initData.portLocationRx);
    pins->clkPin = AF_USART4_CLK_PIN(handle->initData.portLocationClk);
    pins->csPin = AF_USART4_CS_PIN(handle->initData.portLocationCs);
#endif
#if defined(USART5)
  } else if (handle->peripheral.usartPort == USART5) {
    pins->mosiPort = AF_USART5_TX_PORT(handle->initData.portLocationTx);
    pins->misoPort = AF_USART5_RX_PORT(handle->initData.portLocationRx);
    pins->clkPort = AF_USART5_CLK_PORT(handle->initData.portLocationClk);
    pins->csPort = AF_USART5_CS_PORT(handle->initData.portLocationCs);
    pins->mosiPin = AF_USART5_TX_PIN(handle->initData.portLocationTx);
    pins->misoPin = AF_USART5_RX_PIN(handle->initData.portLocationRx);
    pins->clkPin = AF_USART5_CLK_PIN(handle->initData.portLocationClk);
    pins->csPin = AF_USART5_CS_PIN(handle->initData.portLocationCs);
#endif
#if defined(USARTRF0)
  } else if (handle->peripheral.usartPort == USARTRF0) {
    pins->mosiPort = AF_USARTRF0_TX_PORT(handle->initData.portLocationTx);
    pins->misoPort = AF_USARTRF0_RX_PORT(handle->initData.portLocationRx);
    pins->clkPort = AF_USARTRF0_CLK_PORT(handle->initData.portLocationClk);
    pins->csPort = AF_USARTRF0_CS_PORT(handle->initData.portLocationCs);
    pins->mosiPin = AF_USARTRF0_TX_PIN(handle->initData.portLocationTx);
    pins->misoPin = AF_USARTRF0_RX_PIN(handle->initData.portLocationRx);
    pins->clkPin = AF_USARTRF0_CLK_PIN(handle->initData.portLocationClk);
    pins->csPin = AF_USARTRF0_CS_PIN(handle->initData.portLocationCs);
#endif
#if defined(USARTRF1)
  } else if (handle->peripheral.usartPort == USARTRF1) {
    pins->mosiPort = AF_USARTRF1_TX_PORT(handle->initData.portLocationTx);
    pins->misoPort = AF_USARTRF1_RX_PORT(handle->initData.portLocationRx);
    pins->clkPort = AF_USARTRF1_CLK_PORT(handle->initData.portLocationClk);
    pins->csPort = AF_USARTRF1_CS_PORT(handle->initData.portLocationCs);
    pins->mosiPin = AF_USARTRF1_TX_PIN(handle->initData.portLocationTx);
    pins->misoPin = AF_USARTRF1_RX_PIN(handle->initData.portLocationRx);
    pins->clkPin = AF_USARTRF1_CLK_PIN(handle->initData.portLocationClk);
    pins->csPin = AF_USARTRF1_CS_PIN(handle->initData.portLocationCs);
#endif
  } else {
    return ECODE_EMDRV_SPIDRV_PARAM_ERROR;
  }
  return ECODE_EMDRV_SPIDRV_OK;
}

#endif

#if defined(_SILICON_LABS_32B_SERIES_2)

/***************************************************************************//**
 * @brief Get SPI pins for Series 2 devices.
 ******************************************************************************/
static Ecode_t GetSpiPins(SPIDRV_Handle_t handle, SPI_Pins_t *pins)
{
  pins->mosiPort = handle->initData.portTx;
  pins->misoPort = handle->initData.portRx;
  pins->clkPort = handle->initData.portClk;
  pins->csPort = handle->initData.portCs;
  pins->mosiPin = handle->initData.pinTx;
  pins->misoPin = handle->initData.pinRx;
  pins->clkPin = handle->initData.pinClk;
  pins->csPin = handle->initData.pinCs;

  return ECODE_EMDRV_SPIDRV_OK;
}

#endif
