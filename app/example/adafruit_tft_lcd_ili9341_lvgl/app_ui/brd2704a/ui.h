// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_loader
void ui_loader_screen_init(void);

extern lv_obj_t *ui_loader;
extern lv_obj_t *ui_logo;
extern lv_obj_t *ui_Spinner1;
// SCREEN: ui_main
void ui_main_screen_init(void);

extern lv_obj_t *ui_main;
extern lv_obj_t *ui_Image4;
extern lv_obj_t *ui_Panel1;
extern lv_obj_t *ui_Label1;
void ui_event_BTN0(lv_event_t *e);

extern lv_obj_t *ui_BTN0;
extern lv_obj_t *ui_Led0Label;
extern lv_obj_t *ui_ControlTitle;
extern lv_obj_t *ui____initial_actions0;

LV_IMG_DECLARE(ui_img_2106501360);    // assets\silabs-logo-large.png
LV_IMG_DECLARE(ui_img_1802009198);    // assets\silabs-logo.png

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
