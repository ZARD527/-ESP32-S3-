#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include "app_state.h"
#include "ui.h"

#include "lvgl/lvgl.h"

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <unistd.h>
#endif

static lv_display_t * hal_init(int32_t w, int32_t h)
{
    lv_group_set_default(lv_group_create());

    lv_display_t * disp = lv_sdl_window_create(w, h);
    lv_display_set_default(disp);
    lv_sdl_window_set_title(disp, "ESP32 LVGL Dashboard Simulator - 400 x 300 reflective");
    lv_sdl_window_set_resizeable(disp, false);

    lv_indev_t * mouse = lv_sdl_mouse_create();
    lv_indev_set_display(mouse, disp);
    lv_indev_set_group(mouse, lv_group_get_default());

    lv_indev_t * wheel = lv_sdl_mousewheel_create();
    lv_indev_set_display(wheel, disp);
    lv_indev_set_group(wheel, lv_group_get_default());

    lv_indev_t * keyboard = lv_sdl_keyboard_create();
    lv_indev_set_display(keyboard, disp);
    lv_indev_set_group(keyboard, lv_group_get_default());

    return disp;
}

int SDL_main(int argc, char ** argv)
{
    (void)argc;
    (void)argv;

    lv_init();
    hal_init(APP_SCREEN_W, APP_SCREEN_H);
    app_state_init();
    ui_init();

    while(1) {
        app_state_tick(lv_tick_get());
        ui_refresh();
        lv_timer_handler();
#ifdef _MSC_VER
        Sleep(5);
#else
        usleep(5 * 1000);
#endif
    }

    return 0;
}
