#include "ui_style.h"
#include "app_theme.h"
#include "app_cjk_font.h"

const lv_font_t * ui_font_cjk(void)
{
    return &app_cjk_font_16;
}

const lv_font_t * ui_font_title(void)
{
    return &app_cjk_font_22;
}

const lv_font_t * ui_font_small(void)
{
    return &lv_font_montserrat_12;
}

const lv_font_t * ui_font_time(void)
{
    return &lv_font_montserrat_48;
}

void ui_style_screen(lv_obj_t * obj)
{
    lv_obj_remove_style_all(obj);
    lv_obj_set_style_bg_color(obj, lv_color_hex(APP_COLOR_PAPER), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_text_color(obj, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(obj, ui_font_cjk(), 0);
}

void ui_style_page(lv_obj_t * obj)
{
    lv_obj_remove_style_all(obj);
    lv_obj_set_style_bg_color(obj, lv_color_hex(APP_COLOR_PAPER), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_text_color(obj, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(obj, ui_font_cjk(), 0);
}

void ui_style_card(lv_obj_t * obj)
{
    lv_obj_set_style_bg_color(obj, lv_color_hex(APP_COLOR_PAPER_DARK), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
    lv_obj_set_style_border_color(obj, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_border_width(obj, 1, 0);
    lv_obj_set_style_radius(obj, 2, 0);
    lv_obj_set_style_pad_all(obj, 6, 0);
}

void ui_style_button(lv_obj_t * obj, bool active)
{
    lv_obj_set_style_bg_color(obj, lv_color_hex(active ? APP_COLOR_RED_DARK : APP_COLOR_PAPER_DARK), 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(APP_COLOR_RED_SOFT), LV_STATE_PRESSED);
    lv_obj_set_style_border_color(obj, lv_color_hex(active ? APP_COLOR_RED : APP_COLOR_BLACK), 0);
    lv_obj_set_style_border_width(obj, 1, 0);
    lv_obj_set_style_radius(obj, 2, 0);
    lv_obj_set_style_shadow_width(obj, 0, 0);
}

void ui_style_ink_line(lv_obj_t * parent, int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
    lv_obj_t * line = lv_obj_create(parent);
    lv_obj_remove_style_all(line);
    lv_obj_set_pos(line, x, y);
    lv_obj_set_size(line, w, h);
    lv_obj_set_style_bg_color(line, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
}

void ui_add_reflective_texture(lv_obj_t * parent, int32_t w, int32_t h)
{
    for(int32_t y = 8; y < h; y += 10) {
        lv_obj_t * line = lv_obj_create(parent);
        lv_obj_remove_style_all(line);
        lv_obj_set_pos(line, 0, y);
        lv_obj_set_size(line, w, 1);
        lv_obj_set_style_bg_color(line, lv_color_hex(APP_COLOR_GRAY_65), 0);
        lv_obj_set_style_bg_opa(line, LV_OPA_10, 0);
    }

    for(int32_t i = 0; i < 64; i++) {
        int32_t x = (i * 31 + 13) % w;
        int32_t y = (i * 19 + 5) % h;
        lv_obj_t * dot = lv_obj_create(parent);
        lv_obj_remove_style_all(dot);
        lv_obj_set_pos(dot, x, y);
        lv_obj_set_size(dot, 1, 1);
        lv_obj_set_style_bg_color(dot, lv_color_hex((i & 1) ? APP_COLOR_GRAY_80 : APP_COLOR_GRAY_55), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_30, 0);
    }
}
