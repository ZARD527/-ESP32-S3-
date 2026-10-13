#include "page_home.h"
#include "app_theme.h"
#include "ui_style.h"

static lv_obj_t * time_label;
static lv_obj_t * weather_label;
static lv_obj_t * env_label;
static lv_obj_t * date_label;
static lv_obj_t * overview_label;

static lv_obj_t * make_tile(lv_obj_t * parent, int32_t x, const char * title)
{
    lv_obj_t * tile = lv_obj_create(parent);
    lv_obj_set_pos(tile, x, 166);
    lv_obj_set_size(tile, 124, 72);
    ui_style_card(tile);
    ui_style_ink_line(tile, 0, 0, 4, 72, APP_COLOR_RED);

    lv_obj_t * label = lv_label_create(tile);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(label, ui_font_cjk(), 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 8, -2);

    return tile;
}

lv_obj_t * page_home_create(lv_obj_t * parent)
{
    lv_obj_t * root = lv_obj_create(parent);
    ui_style_page(root);
    lv_obj_set_size(root, APP_SCREEN_W, APP_CONTENT_H);
    ui_add_reflective_texture(root, APP_SCREEN_W, APP_CONTENT_H);

    time_label = lv_label_create(root);
    lv_obj_set_style_text_color(time_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(time_label, ui_font_time(), 0);
    lv_obj_align(time_label, LV_ALIGN_TOP_MID, -30, 38);

    lv_obj_t * sec_mark = lv_label_create(root);
    lv_label_set_text(sec_mark, "待机");
    lv_obj_set_style_text_color(sec_mark, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(sec_mark, ui_font_title(), 0);
    lv_obj_align(sec_mark, LV_ALIGN_TOP_MID, 88, 54);

    lv_obj_t * date_badge = lv_label_create(root);
    lv_label_set_text(date_badge, "全反射信息屏");
    lv_obj_set_style_text_color(date_badge, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(date_badge, ui_font_title(), 0);
    lv_obj_align(date_badge, LV_ALIGN_TOP_LEFT, 14, 12);

    overview_label = lv_label_create(root);
    lv_obj_set_style_text_color(overview_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(overview_label, ui_font_cjk(), 0);
    lv_obj_align(overview_label, LV_ALIGN_TOP_MID, 0, 120);

    ui_style_ink_line(root, 58, 146, 284, 1, APP_COLOR_BLACK);

    lv_obj_t * weather_tile = make_tile(root, 6, "天气");
    weather_label = lv_label_create(weather_tile);
    lv_obj_set_style_text_color(weather_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(weather_label, ui_font_cjk(), 0);
    lv_obj_align(weather_label, LV_ALIGN_BOTTOM_LEFT, 8, 0);

    lv_obj_t * env_tile = make_tile(root, 138, "环境");
    env_label = lv_label_create(env_tile);
    lv_obj_set_style_text_color(env_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(env_label, ui_font_cjk(), 0);
    lv_obj_align(env_label, LV_ALIGN_BOTTOM_LEFT, 8, 0);

    lv_obj_t * date_tile = make_tile(root, 270, "日期");
    date_label = lv_label_create(date_tile);
    lv_obj_set_style_text_color(date_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(date_label, ui_font_cjk(), 0);
    lv_obj_align(date_label, LV_ALIGN_BOTTOM_LEFT, 8, 0);

    return root;
}

void page_home_refresh(const app_state_t * state)
{
    int16_t env_tenths = (int16_t)(state->environment.temp_c * 10.0f);

    lv_label_set_text_fmt(time_label, "%02u:%02u", state->clock.hour, state->clock.minute);
    lv_label_set_text_fmt(overview_label, "今日事项 %u ｜ 空气 %u ppm ｜ 光照 %u lx",
                          app_state_count_todos_on(state->clock.year, state->clock.month, state->clock.day),
                          state->environment.co2_ppm, state->environment.light_lux);
    lv_label_set_text_fmt(weather_label, "%s\n%d C  %d%%", state->weather.condition, state->weather.temp_c,
                          state->weather.humidity);
    lv_label_set_text_fmt(env_label, "%d.%d C\n湿度 %d%%", env_tenths / 10, env_tenths % 10,
                          state->environment.humidity);
    lv_label_set_text_fmt(date_label, "%04u\n%02u/%02u  周%u", state->clock.year, state->clock.month,
                          state->clock.day, state->clock.weekday);
}
