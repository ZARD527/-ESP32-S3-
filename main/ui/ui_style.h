#pragma once

#include "lvgl.h"

#include <stdbool.h>
#include <stdint.h>

const lv_font_t * ui_font_cjk(void);
const lv_font_t * ui_font_title(void);
const lv_font_t * ui_font_small(void);
const lv_font_t * ui_font_time(void);

void ui_style_screen(lv_obj_t * obj);
void ui_style_page(lv_obj_t * obj);
void ui_style_card(lv_obj_t * obj);
void ui_style_button(lv_obj_t * obj, bool active);
void ui_style_ink_line(lv_obj_t * parent, int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
void ui_add_reflective_texture(lv_obj_t * parent, int32_t w, int32_t h);
