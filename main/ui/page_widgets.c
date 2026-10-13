#include "page_widgets.h"
#include "app_theme.h"
#include "ui_style.h"

static lv_obj_t * countdown_label;
static lv_obj_t * stopwatch_label;
static lv_obj_t * dice_label;
static lv_obj_t * preset_label;
static lv_obj_t * device_label;
static lv_obj_t * alarm_label;

static void fmt_mmss(char * buf, uint32_t len, uint32_t seconds)
{
    lv_snprintf(buf, len, "%02u:%02u", (unsigned)(seconds / 60U), (unsigned)(seconds % 60U));
}

static void countdown_start_cb(lv_event_t * e)
{
    (void)e;
    app_state_start_countdown();
}

static void countdown_stop_cb(lv_event_t * e)
{
    (void)e;
    app_state_stop_countdown();
}

static void preset_dec_cb(lv_event_t * e)
{
    (void)e;
    const app_state_t * state = app_state_get();
    app_state_set_countdown_preset(state->settings.default_countdown_sec - 60U);
}

static void preset_inc_cb(lv_event_t * e)
{
    (void)e;
    const app_state_t * state = app_state_get();
    app_state_set_countdown_preset(state->settings.default_countdown_sec + 60U);
}

static void stopwatch_toggle_cb(lv_event_t * e)
{
    (void)e;
    app_state_toggle_stopwatch();
}

static void stopwatch_reset_cb(lv_event_t * e)
{
    (void)e;
    app_state_reset_stopwatch();
}

static void dice_cb(lv_event_t * e)
{
    (void)e;
    app_state_roll_dice();
}

static lv_obj_t * make_button(lv_obj_t * parent, const char * text, int32_t x, int32_t y, int32_t w,
                              lv_event_cb_t cb)
{
    lv_obj_t * btn = lv_button_create(parent);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, w, 28);
    lv_obj_set_style_bg_color(btn, lv_color_hex(APP_COLOR_PAPER_DARK), 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(APP_COLOR_RED_SOFT), LV_STATE_PRESSED);
    lv_obj_set_style_border_color(btn, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_radius(btn, 2, 0);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(label, ui_font_cjk(), 0);
    lv_obj_center(label);
    return btn;
}

lv_obj_t * page_widgets_create(lv_obj_t * parent)
{
    lv_obj_t * root = lv_obj_create(parent);
    ui_style_page(root);
    lv_obj_set_size(root, APP_SCREEN_W, APP_CONTENT_H);
    ui_add_reflective_texture(root, APP_SCREEN_W, APP_CONTENT_H);

    lv_obj_t * title = lv_label_create(root);
    lv_label_set_text(title, "小组件");
    lv_obj_set_style_text_color(title, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(title, ui_font_title(), 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10, 8);
    ui_style_ink_line(root, 10, 34, 380, 2, APP_COLOR_RED);

    lv_obj_t * timer_card = lv_obj_create(root);
    lv_obj_set_pos(timer_card, 10, 48);
    lv_obj_set_size(timer_card, 270, 78);
    ui_style_card(timer_card);
    ui_style_ink_line(timer_card, 0, 0, 4, 78, APP_COLOR_RED);

    lv_obj_t * dice_card = lv_obj_create(root);
    lv_obj_set_pos(dice_card, 292, 48);
    lv_obj_set_size(dice_card, 98, 78);
    ui_style_card(dice_card);
    ui_style_ink_line(dice_card, 0, 0, 4, 78, APP_COLOR_RED);

    lv_obj_t * stopwatch_card = lv_obj_create(root);
    lv_obj_set_pos(stopwatch_card, 10, 138);
    lv_obj_set_size(stopwatch_card, 180, 84);
    ui_style_card(stopwatch_card);
    ui_style_ink_line(stopwatch_card, 0, 0, 4, 84, APP_COLOR_RED);

    lv_obj_t * info_card = lv_obj_create(root);
    lv_obj_set_pos(info_card, 204, 138);
    lv_obj_set_size(info_card, 186, 84);
    ui_style_card(info_card);
    ui_style_ink_line(info_card, 0, 0, 4, 84, APP_COLOR_RED);

    countdown_label = lv_label_create(timer_card);
    lv_obj_set_style_text_color(countdown_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(countdown_label, ui_font_cjk(), 0);
    lv_obj_set_pos(countdown_label, 12, 6);

    make_button(timer_card, "开始", 12, 38, 52, countdown_start_cb);
    make_button(timer_card, "停止", 70, 38, 52, countdown_stop_cb);
    make_button(timer_card, "-1分", 128, 38, 52, preset_dec_cb);
    make_button(timer_card, "+1分", 186, 38, 52, preset_inc_cb);

    preset_label = lv_label_create(timer_card);
    lv_obj_set_style_text_color(preset_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(preset_label, ui_font_cjk(), 0);
    lv_obj_set_pos(preset_label, 128, 7);

    stopwatch_label = lv_label_create(stopwatch_card);
    lv_obj_set_style_text_color(stopwatch_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(stopwatch_label, ui_font_cjk(), 0);
    lv_obj_set_pos(stopwatch_label, 12, 8);

    make_button(stopwatch_card, "计时", 12, 44, 58, stopwatch_toggle_cb);
    make_button(stopwatch_card, "复位", 78, 44, 58, stopwatch_reset_cb);

    dice_label = lv_label_create(dice_card);
    lv_obj_set_style_text_color(dice_label, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(dice_label, ui_font_cjk(), 0);
    lv_obj_set_pos(dice_label, 12, 6);
    make_button(dice_card, "投掷", 12, 38, 64, dice_cb);

    device_label = lv_label_create(info_card);
    lv_obj_set_style_text_color(device_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(device_label, ui_font_cjk(), 0);
    lv_obj_set_pos(device_label, 12, 10);

    alarm_label = lv_label_create(info_card);
    lv_obj_set_style_text_color(alarm_label, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(alarm_label, ui_font_cjk(), 0);
    lv_obj_set_pos(alarm_label, 12, 42);

    return root;
}

void page_widgets_refresh(const app_state_t * state)
{
    char buf[16];
    fmt_mmss(buf, sizeof(buf), state->countdown_remaining_sec);
    lv_label_set_text_fmt(countdown_label, "倒计时 %s", buf);

    fmt_mmss(buf, sizeof(buf), state->settings.default_countdown_sec);
    lv_label_set_text_fmt(preset_label, "默认 %s  左侧朝下立即启动", buf);

    uint32_t sw_sec = state->stopwatch_ms / 1000U;
    lv_label_set_text_fmt(stopwatch_label, "秒表 %02u:%02u", (unsigned)(sw_sec / 60U), (unsigned)(sw_sec % 60U));
    lv_label_set_text_fmt(dice_label, "骰子 %u", state->dice_value);
    lv_label_set_text(device_label, "设备 ESP32  横屏 400x300");
    lv_label_set_text(alarm_label, "闹钟 07:30 关闭");
}
