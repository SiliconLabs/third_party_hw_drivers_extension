// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "../ui.h"

void ui_loader_screen_init(void)
{
  ui_loader = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_loader, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
  lv_obj_set_style_bg_color(ui_loader,
                            lv_color_hex(0x000000),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_loader, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_logo = lv_img_create(ui_loader);
  lv_img_set_src(ui_logo, &ui_img_2106501360);
  lv_obj_set_width(ui_logo, LV_SIZE_CONTENT); /// 134
  lv_obj_set_height(ui_logo, LV_SIZE_CONTENT);  /// 76
  lv_obj_set_align(ui_logo, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_logo, LV_OBJ_FLAG_ADV_HITTEST);   /// Flags
  lv_obj_clear_flag(ui_logo, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

  ui_Spinner1 = lv_spinner_create(ui_loader, 1000, 90);
  lv_obj_set_width(ui_Spinner1, 28);
  lv_obj_set_height(ui_Spinner1, 29);
  lv_obj_set_x(ui_Spinner1, 3);
  lv_obj_set_y(ui_Spinner1, 81);
  lv_obj_set_align(ui_Spinner1, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_Spinner1, LV_OBJ_FLAG_CLICKABLE);    /// Flags
  lv_obj_set_style_arc_color(ui_Spinner1,
                             lv_color_hex(0xFFFFFF),
                             LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_Spinner1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(ui_Spinner1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_arc_color(ui_Spinner1,
                             lv_color_hex(0xD50008),
                             LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_Spinner1,
                           255,
                           LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(ui_Spinner1,
                             5,
                             LV_PART_INDICATOR | LV_STATE_DEFAULT);
}
