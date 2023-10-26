SET(SOURCES screens/ui_screenmain.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_1802009198.c
    images/ui_img_1258194689.c
    images/ui_img_1258193662.c)

add_library(ui ${SOURCES})
