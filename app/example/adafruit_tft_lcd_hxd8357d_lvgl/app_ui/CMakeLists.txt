SET(SOURCES screens/ui_main.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_842152610.c
    images/ui_img_buld_off_png.c
    images/ui_img_buld_on_png.c
    images/ui_img_button1_off_png.c
    images/ui_img_button1_on_png.c
    images/ui_img_gear1_png.c)

add_library(ui ${SOURCES})
