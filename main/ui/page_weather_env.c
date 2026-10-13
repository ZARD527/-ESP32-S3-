#include "page_weather_env.h"
#include "app_theme.h"
#include "ui_style.h"

static lv_obj_t * weather_big;
static lv_obj_t * env_grid;

lv_obj_t * page_weather_env_create(lv_obj_t * parent)
{
    lv_obj_t * root = lv_obj_create(parent);
    ui_style_page(root);
    lv_obj_set_size(root, APP_SCREEN_W, APP_CONTENT_H);
    ui_add_reflective_texture(root, APP_SCREEN_W, APP_CONTENT_H);

    lv_obj_t * title = lv_label_create(root);
    lv_label_set_text(title, "天气与环境");
    lv_obj_set_style_text_color(title, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(title, ui_font_title(), 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10, 10);
    ui_style_ink_line(root, 10, 36, 380, 2, APP_COLOR_RED);

    lv_obj_t * weather_card = lv_obj_create(root);
    lv_obj_set_pos(weather_card, 10, 50);
    lv_obj_set_size(weather_card, 180, 170);
    ui_style_card(weather_card);
    ui_style_ink_line(weather_card, 0, 0, 4, 170, APP_COLOR_RED);

    lv_obj_t * env_card = lv_obj_create(root);
    lv_obj_set_pos(env_card, 204, 50);
    lv_obj_set_size(env_card, 186, 170);
    ui_style_card(env_card);
    ui_style_ink_line(env_card, 0, 0, 4, 170, APP_COLOR_RED);

    weather_big = lv_label_create(weather_card);
    lv_obj_set_style_text_color(weather_big, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(weather_big, ui_font_cjk(), 0);
    lv_obj_set_style_text_line_space(weather_big, 10, 0);
    lv_obj_set_pos(weather_big, 12, 12);

    env_grid = lv_label_create(env_card);
    lv_obj_set_width(env_grid, 158);
    lv_obj_set_style_text_color(env_grid, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(env_grid, ui_font_cjk(), 0);
    lv_obj_set_style_text_line_space(env_grid, 10, 0);
    lv_obj_set_pos(env_grid, 12, 12);

    return root;
}

void page_weather_env_refresh(const app_state_t * state)
{
    int16_t env_tenths = (int16_t)(state->environment.temp_c * 10.0f);

    lv_label_set_text_fmt(weather_big, "网络天气\n%d C\n%s\n最高 %d  最低 %d", state->weather.temp_c, state->weather.condition,
                          state->weather.high_c, state->weather.low_c);
    lv_label_set_text_fmt(env_grid, "传感器\n室温 %d.%d C\n湿度 %u%%\nCO2 %u ppm\n光照 %u lx", env_tenths / 10,
                          env_tenths % 10, state->environment.humidity, state->environment.co2_ppm,
                          state->environment.light_lux);
}
