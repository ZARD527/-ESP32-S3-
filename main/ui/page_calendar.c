#include "page_calendar.h"
#include "app_theme.h"
#include "ui_style.h"

#include <stdbool.h>

static page_day_open_cb_t day_open_cb;
static lv_obj_t * month_label;
static lv_obj_t * legend_label;
static lv_obj_t * day_buttons[42];
static lv_obj_t * day_labels[42];
static uint8_t day_numbers[42];

static bool leap_year(uint16_t year)
{
    return ((year % 4U == 0U) && (year % 100U != 0U)) || (year % 400U == 0U);
}

static uint8_t days_in_month(uint16_t year, uint8_t month)
{
    static const uint8_t days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if(month == 2 && leap_year(year)) return 29;
    return days[month - 1U];
}

static uint8_t first_weekday(uint16_t year, uint8_t month)
{
    if(month < 3) {
        month += 12;
        year--;
    }

    uint16_t k = year % 100U;
    uint16_t j = year / 100U;
    uint16_t h = (1U + (13U * (month + 1U)) / 5U + k + k / 4U + j / 4U + 5U * j) % 7U;
    return (uint8_t)((h + 6U) % 7U);
}

static void day_event_cb(lv_event_t * e)
{
    lv_obj_t * target = lv_event_get_target_obj(e);
    for(uint8_t i = 0; i < 42; i++) {
        if(target == day_buttons[i] && day_numbers[i] > 0 && day_open_cb != NULL) {
            day_open_cb(day_numbers[i]);
            return;
        }
    }
}

lv_obj_t * page_calendar_create(lv_obj_t * parent, page_day_open_cb_t open_cb)
{
    day_open_cb = open_cb;

    lv_obj_t * root = lv_obj_create(parent);
    ui_style_page(root);
    lv_obj_set_size(root, APP_SCREEN_W, APP_CONTENT_H);
    ui_add_reflective_texture(root, APP_SCREEN_W, APP_CONTENT_H);

    month_label = lv_label_create(root);
    lv_obj_set_style_text_color(month_label, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(month_label, ui_font_title(), 0);
    lv_obj_align(month_label, LV_ALIGN_TOP_LEFT, 10, 8);

    legend_label = lv_label_create(root);
    lv_label_set_text(legend_label, "* 待办   # 节日");
    lv_obj_set_style_text_color(legend_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(legend_label, ui_font_cjk(), 0);
    lv_obj_align(legend_label, LV_ALIGN_TOP_RIGHT, -12, 10);
    ui_style_ink_line(root, 10, 31, 380, 2, APP_COLOR_RED);

    static const char * week[] = { "日", "一", "二", "三", "四", "五", "六" };
    for(uint8_t i = 0; i < 7; i++) {
        lv_obj_t * label = lv_label_create(root);
        lv_label_set_text(label, week[i]);
        lv_obj_set_style_text_color(label, lv_color_hex(APP_COLOR_BLACK), 0);
        lv_obj_set_style_text_font(label, ui_font_cjk(), 0);
        lv_obj_set_pos(label, 25 + i * 56, 34);
    }

    for(uint8_t i = 0; i < 42; i++) {
        uint8_t row = i / 7U;
        uint8_t col = i % 7U;

        day_buttons[i] = lv_button_create(root);
        lv_obj_set_pos(day_buttons[i], 8 + col * 56, 54 + row * 31);
        lv_obj_set_size(day_buttons[i], 48, 26);
        lv_obj_set_style_bg_color(day_buttons[i], lv_color_hex(APP_COLOR_PAPER), 0);
        lv_obj_set_style_bg_color(day_buttons[i], lv_color_hex(APP_COLOR_RED_SOFT), LV_STATE_PRESSED);
        lv_obj_set_style_border_color(day_buttons[i], lv_color_hex(APP_COLOR_BLACK), 0);
        lv_obj_set_style_border_width(day_buttons[i], 1, 0);
        lv_obj_set_style_radius(day_buttons[i], 1, 0);
        lv_obj_add_event_cb(day_buttons[i], day_event_cb, LV_EVENT_CLICKED, NULL);

        day_labels[i] = lv_label_create(day_buttons[i]);
        lv_obj_set_style_text_color(day_labels[i], lv_color_hex(APP_COLOR_BLACK), 0);
        lv_obj_set_style_text_font(day_labels[i], ui_font_small(), 0);
        lv_obj_center(day_labels[i]);
    }

    return root;
}

void page_calendar_refresh(const app_state_t * state)
{
    lv_label_set_text_fmt(month_label, "%04u 年 %02u 月", state->clock.year, state->clock.month);

    uint8_t first = first_weekday(state->clock.year, state->clock.month);
    uint8_t days = days_in_month(state->clock.year, state->clock.month);

    for(uint8_t i = 0; i < 42; i++) {
        int16_t day = (int16_t)i - (int16_t)first + 1;
        if(day < 1 || day > days) {
            day_numbers[i] = 0;
            lv_label_set_text(day_labels[i], "");
            lv_obj_set_style_bg_color(day_buttons[i], lv_color_hex(APP_COLOR_PAPER), 0);
            lv_obj_set_style_border_color(day_buttons[i], lv_color_hex(APP_COLOR_GRAY_65), 0);
            continue;
        }

        day_numbers[i] = (uint8_t)day;
        uint8_t todo_count = app_state_count_todos_on(state->clock.year, state->clock.month, (uint8_t)day);
        uint8_t holiday_count = app_state_count_holidays_on(state->clock.year, state->clock.month, (uint8_t)day);
        if(todo_count > 0 || holiday_count > 0) {
            lv_label_set_text_fmt(day_labels[i], "%d%s%s", day, todo_count > 0 ? "*" : "",
                                  holiday_count > 0 ? "#" : "");
            lv_obj_set_style_border_color(day_buttons[i], lv_color_hex(APP_COLOR_RED), 0);
        }
        else {
            lv_label_set_text_fmt(day_labels[i], "%d", day);
            lv_obj_set_style_border_color(day_buttons[i], lv_color_hex(APP_COLOR_GRAY_25), 0);
        }

        lv_obj_set_style_bg_color(day_buttons[i],
                                  lv_color_hex((uint8_t)day == state->clock.day ? APP_COLOR_RED_DARK
                                                                                : APP_COLOR_PAPER),
                                  0);
        lv_obj_set_style_text_color(day_labels[i],
                                    lv_color_hex((uint8_t)day == state->clock.day ? APP_COLOR_GRAY_95
                                                                                  : APP_COLOR_BLACK),
                                    0);
    }
}
