#include "ui.h"
#include "app_config.h"
#include "app_theme.h"
#include "page_calendar.h"
#include "page_home.h"
#include "page_notes.h"
#include "page_weather_env.h"
#include "page_widgets.h"
#include "ui_style.h"

#include "lvgl.h"

static lv_obj_t * status_label;
static lv_obj_t * nav_buttons[UI_PAGE_COUNT];
static lv_obj_t * nav_labels[UI_PAGE_COUNT];
static lv_obj_t * pages[UI_PAGE_COUNT];
static ui_page_t current_page = UI_PAGE_HOME;
static uint32_t last_refresh_ms;

static void style_root(lv_obj_t * obj)
{
    ui_style_screen(obj);
}

static void close_modal_cb(lv_event_t * e)
{
    lv_obj_t * modal = lv_event_get_user_data(e);
    if(modal != NULL) {
        lv_obj_delete(modal);
    }
}

static void nav_event_cb(lv_event_t * e)
{
    uintptr_t page = (uintptr_t)lv_event_get_user_data(e);
    ui_show_page((ui_page_t)page);
}

static void make_nav(lv_obj_t * parent)
{
    static const char * labels[] = { "首页", "便签", "日历", "环境", "工具" };
    const int32_t nav_w = APP_SCREEN_W / UI_PAGE_COUNT;

    for(uint8_t i = 0; i < UI_PAGE_COUNT; i++) {
        nav_buttons[i] = lv_button_create(parent);
        lv_obj_set_pos(nav_buttons[i], i * nav_w, APP_SCREEN_H - APP_NAV_H);
        lv_obj_set_size(nav_buttons[i], nav_w, APP_NAV_H);
        ui_style_button(nav_buttons[i], false);
        lv_obj_add_flag(nav_buttons[i], LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(nav_buttons[i], nav_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)i);

        nav_labels[i] = lv_label_create(nav_buttons[i]);
        lv_label_set_text(nav_labels[i], labels[i]);
        lv_obj_set_style_text_color(nav_labels[i], lv_color_hex(APP_COLOR_BLACK), 0);
        lv_obj_set_style_text_font(nav_labels[i], ui_font_cjk(), 0);
        lv_obj_center(nav_labels[i]);
    }
}

static void refresh_status(const app_state_t * state)
{
    lv_label_set_text_fmt(status_label, "%02u:%02u ｜ 电量 %u%%%s ｜ WiFi %ddBm ｜ %s", state->clock.hour,
                          state->clock.minute, state->status.level, state->status.charging ? "+" : "",
                          state->status.wifi_rssi, state->status.cloud_online ? "已同步" : "本地");
}

static void refresh_nav(void)
{
    for(uint8_t i = 0; i < UI_PAGE_COUNT; i++) {
        if(i == current_page) {
            lv_obj_add_state(nav_buttons[i], LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(nav_buttons[i], lv_color_hex(APP_COLOR_RED_DARK), 0);
            lv_obj_set_style_border_color(nav_buttons[i], lv_color_hex(APP_COLOR_RED), 0);
            lv_obj_set_style_text_color(nav_labels[i], lv_color_hex(APP_COLOR_GRAY_95), 0);
        }
        else {
            lv_obj_remove_state(nav_buttons[i], LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(nav_buttons[i], lv_color_hex(APP_COLOR_PAPER_DARK), 0);
            lv_obj_set_style_border_color(nav_buttons[i], lv_color_hex(APP_COLOR_BLACK), 0);
            lv_obj_set_style_text_color(nav_labels[i], lv_color_hex(APP_COLOR_BLACK), 0);
        }
    }
}

void ui_init(void)
{
    lv_obj_t * screen = lv_obj_create(NULL);
    style_root(screen);
    lv_obj_set_size(screen, APP_SCREEN_W, APP_SCREEN_H);
    lv_screen_load(screen);
    ui_add_reflective_texture(screen, APP_SCREEN_W, APP_SCREEN_H);

    lv_obj_t * status_bar = lv_obj_create(screen);
    lv_obj_remove_style_all(status_bar);
    lv_obj_set_pos(status_bar, 0, 0);
    lv_obj_set_size(status_bar, APP_SCREEN_W, APP_STATUS_H);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(APP_COLOR_PAPER_DARK), 0);
    lv_obj_set_style_bg_opa(status_bar, LV_OPA_70, 0);
    lv_obj_set_style_border_color(status_bar, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_border_width(status_bar, 1, 0);

    status_label = lv_label_create(status_bar);
    lv_obj_set_style_text_color(status_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(status_label, ui_font_cjk(), 0);
    lv_obj_align(status_label, LV_ALIGN_LEFT_MID, 6, 0);

    ui_style_ink_line(screen, 0, APP_STATUS_H, APP_SCREEN_W, 2, APP_COLOR_RED);

    lv_obj_t * content = lv_obj_create(screen);
    ui_style_page(content);
    lv_obj_set_pos(content, 0, APP_CONTENT_Y);
    lv_obj_set_size(content, APP_SCREEN_W, APP_CONTENT_H);

    pages[UI_PAGE_HOME] = page_home_create(content);
    pages[UI_PAGE_NOTES] = page_notes_create(content, ui_open_note_detail);
    pages[UI_PAGE_CALENDAR] = page_calendar_create(content, ui_open_day_detail);
    pages[UI_PAGE_WEATHER_ENV] = page_weather_env_create(content);
    pages[UI_PAGE_WIDGETS] = page_widgets_create(content);

    make_nav(screen);
    ui_show_page(UI_PAGE_HOME);
    ui_refresh();
}

void ui_show_page(ui_page_t page)
{
    if(page >= UI_PAGE_COUNT) return;

    current_page = page;
    for(uint8_t i = 0; i < UI_PAGE_COUNT; i++) {
        if(i == page) {
            lv_obj_remove_flag(pages[i], LV_OBJ_FLAG_HIDDEN);
        }
        else {
            lv_obj_add_flag(pages[i], LV_OBJ_FLAG_HIDDEN);
        }
    }

    refresh_nav();
}

void ui_refresh(void)
{
    uint32_t now = lv_tick_get();
    if(last_refresh_ms != 0 && now - last_refresh_ms < 250U) return;
    last_refresh_ms = now;

    const app_state_t * state = app_state_get();
    refresh_status(state);
    page_home_refresh(state);
    page_notes_refresh(state);
    page_calendar_refresh(state);
    page_weather_env_refresh(state);
    page_widgets_refresh(state);
}

void ui_handle_imu_action(app_imu_action_t action)
{
    if(action == APP_IMU_ACTION_COUNTDOWN_STARTED || action == APP_IMU_ACTION_DICE_ROLLED) {
        ui_show_page(UI_PAGE_WIDGETS);
    }
}

void ui_open_note_detail(uint8_t index)
{
    const app_state_t * state = app_state_get();
    if(index >= state->note_count) return;

    lv_obj_t * modal = lv_obj_create(lv_layer_top());
    lv_obj_set_size(modal, APP_SCREEN_W, APP_SCREEN_H);
    lv_obj_set_style_bg_color(modal, lv_color_hex(APP_COLOR_PAPER), 0);
    lv_obj_set_style_bg_opa(modal, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(modal, 0, 0);
    lv_obj_set_style_radius(modal, 0, 0);
    ui_add_reflective_texture(modal, APP_SCREEN_W, APP_SCREEN_H);

    lv_obj_t * title = lv_label_create(modal);
    lv_label_set_text(title, state->notes[index].title);
    lv_obj_set_width(title, 260);
    lv_label_set_long_mode(title, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_color(title, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(title, ui_font_title(), 0);
    lv_obj_set_pos(title, 14, 18);
    ui_style_ink_line(modal, 14, 56, APP_SCREEN_W - 28, 2, APP_COLOR_RED);

    lv_obj_t * body = lv_label_create(modal);
    lv_label_set_text(body, state->notes[index].body);
    lv_obj_set_width(body, APP_SCREEN_W - 28);
    lv_label_set_long_mode(body, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_color(body, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(body, ui_font_cjk(), 0);
    lv_obj_set_style_text_line_space(body, 8, 0);
    lv_obj_set_pos(body, 14, 72);

    lv_obj_t * close = lv_button_create(modal);
    lv_obj_set_size(close, 58, 34);
    lv_obj_set_pos(close, APP_SCREEN_W - 72, APP_SCREEN_H - 48);
    ui_style_button(close, true);
    lv_obj_add_event_cb(close, close_modal_cb, LV_EVENT_CLICKED, modal);

    lv_obj_t * close_label = lv_label_create(close);
    lv_label_set_text(close_label, "关闭");
    lv_obj_set_style_text_color(close_label, lv_color_hex(APP_COLOR_GRAY_95), 0);
    lv_obj_set_style_text_font(close_label, ui_font_cjk(), 0);
    lv_obj_center(close_label);
}

void ui_open_day_detail(uint8_t day)
{
    const app_state_t * state = app_state_get();

    lv_obj_t * modal = lv_obj_create(lv_layer_top());
    lv_obj_set_size(modal, APP_SCREEN_W, APP_SCREEN_H);
    lv_obj_set_style_bg_color(modal, lv_color_hex(APP_COLOR_PAPER), 0);
    lv_obj_set_style_bg_opa(modal, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(modal, 0, 0);
    lv_obj_set_style_radius(modal, 0, 0);
    ui_add_reflective_texture(modal, APP_SCREEN_W, APP_SCREEN_H);

    lv_obj_t * title = lv_label_create(modal);
    lv_label_set_text_fmt(title, "%04u/%02u/%02u", state->clock.year, state->clock.month, day);
    lv_obj_set_style_text_color(title, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(title, ui_font_title(), 0);
    lv_obj_set_pos(title, 14, 18);
    ui_style_ink_line(modal, 14, 56, APP_SCREEN_W - 28, 2, APP_COLOR_RED);

    lv_obj_t * list = lv_label_create(modal);
    lv_obj_set_width(list, APP_SCREEN_W - 28);
    lv_label_set_long_mode(list, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_color(list, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(list, ui_font_cjk(), 0);
    lv_obj_set_style_text_line_space(list, 8, 0);
    lv_obj_set_pos(list, 14, 70);

    char text[320] = "";
    uint16_t used = 0;
    for(uint8_t i = 0; i < state->todo_count && used < sizeof(text); i++) {
        const app_todo_t * todo = &state->todos[i];
        if(todo->year == state->clock.year && todo->month == state->clock.month && todo->day == day) {
            int written = lv_snprintf(text + used, sizeof(text) - used, "%s %s\n", todo->done ? "[x]" : "[ ]",
                                      todo->title);
            if(written <= 0) break;
            used += (uint16_t)written;
        }
    }
    for(uint8_t i = 0; i < state->holiday_count && used < sizeof(text); i++) {
        const app_holiday_t * holiday = &state->holidays[i];
        if(holiday->year == state->clock.year && holiday->month == state->clock.month && holiday->day == day) {
            int written = lv_snprintf(text + used, sizeof(text) - used, "# %s\n", holiday->title);
            if(written <= 0) break;
            used += (uint16_t)written;
        }
    }
    if(used == 0) {
        lv_snprintf(text, sizeof(text), "暂无待办");
    }
    lv_label_set_text(list, text);

    lv_obj_t * close = lv_button_create(modal);
    lv_obj_set_size(close, 58, 34);
    lv_obj_set_pos(close, APP_SCREEN_W - 72, APP_SCREEN_H - 48);
    ui_style_button(close, true);
    lv_obj_add_event_cb(close, close_modal_cb, LV_EVENT_CLICKED, modal);

    lv_obj_t * close_label = lv_label_create(close);
    lv_label_set_text(close_label, "关闭");
    lv_obj_set_style_text_color(close_label, lv_color_hex(APP_COLOR_GRAY_95), 0);
    lv_obj_set_style_text_font(close_label, ui_font_cjk(), 0);
    lv_obj_center(close_label);
}
