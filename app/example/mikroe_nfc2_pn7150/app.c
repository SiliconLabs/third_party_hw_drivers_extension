/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <string.h>
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "mikroe_pn7150.h"

#define NFC2_IRQ_STATE_LOW    0
#define NFC2_IRQ_STATE_HIGH   1

static mikroe_pn7150_control_packet_t ctrl_pck_data;

/**
 * @brief NFC 2 display packet function.
 * @details This function displays data values.
 */
static void display_packet(mikroe_pn7150_control_packet_t *ctrl_pck);

/**
 * @brief NFC 2 display nfc data function.
 * @details This function displays nfc data values.
 */
static void display_nfc_data(mikroe_pn7150_control_packet_t *ctrl_pck);

/**
 * @brief NFC 2 read nfc data function.
 * @details This function reads nfc data and displays data.
 */
static void read_nfc_data(mikroe_pn7150_control_packet_t *ctrl_pck);

/**
 * @brief NFC 2  test antena function.
 * @details This function tests antenna and displays data.
 */
static void test_antenna(mikroe_pn7150_control_packet_t *ctrl_pck);

void app_init(void)
{
  app_log(" Application Init ");

  if (SL_STATUS_OK != mikroe_pn7150_init(sl_i2cspm_mikroe)) {
    app_log("> PN7150 - NFC 2 Click board driver init failed.\n");
  }
  sl_sleeptimer_delay_millisecond(100);

  app_log("        HW Reset       \r\n");
  mikroe_pn7150_hw_reset();
  sl_sleeptimer_delay_millisecond(100);

  app_log("-----------------------\r\n");
  app_log(" Reset and Init. Core  \r\n");
  mikroe_pn7150_cmd_core_reset();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_cmd_core_init();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(&ctrl_pck_data);

  while (mikroe_pn7150_check_irq() == NFC2_IRQ_STATE_HIGH) {}

  app_log("-----------------------\r\n");
  app_log(" Disabling Standby Mode \r\n");
  mikroe_pn7150_cmd_disable_standby_mode();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(&ctrl_pck_data);

  test_antenna(&ctrl_pck_data);

  app_log("-----------------------\r\n");
  app_log("Starting Test Procedure\r\n");
  mikroe_pn7150_cmd_test_procedure();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(&ctrl_pck_data);

  mikroe_pn7150_hw_reset();
  sl_sleeptimer_delay_millisecond(100);

  app_log("-----------------------\r\n");
  app_log("       NFC Config.     \r\n");
  mikroe_pn7150_default_cfg(&ctrl_pck_data);

  app_log("-----------------------\r\n");
  app_log("     Discovery Start   \r\n");
  mikroe_pn7150_cmd_start_discovery();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(&ctrl_pck_data);

  app_log("-----------------------\r\n");
  app_log("-------- START --------\r\n");
  app_log("-----------------------\r\n");
  sl_sleeptimer_delay_millisecond(500);

  app_log(" Application Task ");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  while (mikroe_pn7150_check_irq() == NFC2_IRQ_STATE_HIGH) {
    read_nfc_data(&ctrl_pck_data);
  }

  while (mikroe_pn7150_check_irq() == NFC2_IRQ_STATE_LOW) {}

  app_log("-----------------------\r\n");
  sl_sleeptimer_delay_millisecond(1000);
}

static void display_packet(mikroe_pn7150_control_packet_t *ctrl_pck)
{
  app_log("- - - - - - - - - - - -\r\n");
  app_log(" Message Type   = %d\r\n", ( uint16_t ) ctrl_pck->message_type);
  app_log(" Pck Bound Flag = %d\r\n", ( uint16_t ) ctrl_pck->pck_bound_flag);
  app_log(" Group Ident    = %d\r\n", ( uint16_t ) ctrl_pck->group_ident);
  app_log(" Opcode Ident   = %d\r\n", ( uint16_t ) ctrl_pck->opcode_ident);
  app_log(" Payload Length = %d\r\n", ( uint16_t ) ctrl_pck->payload_length);
  app_log("- - - - - - - - - - - -\r\n");

  for (uint8_t n_cnt = 0; n_cnt < ctrl_pck_data.payload_length; n_cnt++) {
    app_log(" Payload[ %.2d ]  = 0x%.2X\r\n", ( uint16_t ) n_cnt,
            ( uint16_t ) ( uint16_t ) ctrl_pck_data.payload[n_cnt]);
  }

  app_log("- - - - - - - - - - - -\r\n");
  memset(ctrl_pck_data.payload, 0x00, 255);
}

static void display_nfc_data(mikroe_pn7150_control_packet_t *ctrl_pck)
{
  app_log("- - - - - - - - - - - -\r\n");
  app_log(" Read Block:\r\n");

  for (uint8_t n_cnt = 1; n_cnt < ctrl_pck->payload_length - 2; n_cnt++) {
    app_log("\t 0x%.2X \r\n", ( uint16_t ) ctrl_pck->payload[n_cnt]);
  }
  app_log("\t 0x%.2X \r\n",
          ( uint16_t ) ctrl_pck->payload[ctrl_pck->payload_length - 2]);

  app_log("- - - - - - - - - - - -\r\n");
  memset(ctrl_pck->payload, 0x00, 255);
}

static void read_nfc_data(mikroe_pn7150_control_packet_t *ctrl_pck)
{
  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  display_nfc_data(ctrl_pck);
  app_log("    Disconnect Card    \r\n");
  mikroe_pn7150_cmd_card_disconnected();
  sl_sleeptimer_delay_millisecond(10);
  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(10);

  while (mikroe_pn7150_check_irq() == NFC2_IRQ_STATE_LOW) {}

  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(100);
}

static void test_antenna(mikroe_pn7150_control_packet_t *ctrl_pck)
{
  app_log("-----------------------\r\n");
  app_log("    Testing Antenna    ");
  mikroe_pn7150_cmd_antenna_test(0x01);
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_cmd_antenna_test(0x07);
  sl_sleeptimer_delay_millisecond(100);
  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_cmd_antenna_test(0x0F);
  sl_sleeptimer_delay_millisecond(100);
  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(ctrl_pck);
}
