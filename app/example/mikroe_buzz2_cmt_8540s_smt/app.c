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

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

#include "sl_sleeptimer.h"
#include "sl_pwm_instances.h"
#include "app_log.h"
#include "mikroe_cmt_8540s_smt.h"

#define W      4 * Q // Whole 4/4 - 4 Beats
#define H      2 * Q // Half 2/4 - 2 Beats
#define Q      250 // Quarter 1/4 - 1 Beat
#define E      Q / 2 // Eighth 1/8 - 1/2 Beat
#define S      Q / 4 // Sixteenth 1/16 - 1/4 Beat

#define VOLUME 100 // goes up to 1000

static void imperial_march( )
{
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, H);
  sl_sleeptimer_delay_millisecond(1 + H);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, H);
  sl_sleeptimer_delay_millisecond(1 + H);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_G7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Gb7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F7, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Bb6, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Eb7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_D7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Db7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_B6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, H);
  sl_sleeptimer_delay_millisecond(1 + H);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_G7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Gb7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F7, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Bb6, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Eb7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_D7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Db7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_B6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab6, VOLUME, H);
  sl_sleeptimer_delay_millisecond(1 + H);
}

void app_init(void)
{
  app_log(
    "- Buzzer 2 Click board driver, example application. -\n");

  if (SL_STATUS_OK != mikroe_cmt_8540s_smt_init(&sl_pwm_mikroe)) {
    app_log(
      "> Buzzer 2 Click board driver init failed, please run program again.\n");
    while (1) {}
  } else {
    app_log("> Buzzer 2 Click board driver init successfully.\n");
    mikroe_cmt_8540s_smt_set_duty_cycle(0.0);
    mikroe_cmt_8540s_smt_pwm_start();
    sl_sleeptimer_delay_millisecond(100);
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  app_log("Playing the Imperial March melody ...\r\n");
  imperial_march( );
  sl_sleeptimer_delay_millisecond(10000);
}
