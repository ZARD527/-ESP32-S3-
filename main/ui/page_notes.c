#include "page_notes.h"
#include "app_theme.h"
#include "ui_style.h"

static page_note_open_cb_t note_open_cb;
static lv_obj_t * note_buttons[APP_MAX_NOTES];
static lv_obj_t * note_labels[APP_MAX_NOTES];
static lv_obj_t * note_hint;

static void note_event_cb(lv_event_t * e)
{
    lv_obj_t * target = lv_event_get_target_obj(e);
    for(uint8_t i = 0; i < APP_MAX_NOTES; i++) {
        if(target == note_buttons[i] && note_open_cb != NULL) {
            note_open_cb(i);
            return;
        }
    }
}

lv_obj_t * page_notes_create(lv_obj_t * parent, page_note_open_cb_t open_cb)
{
    note_open_cb = open_cb;

    lv_obj_t * root = lv_obj_create(parent);
    ui_style_page(root);
    lv_obj_set_size(root, APP_SCREEN_W, APP_CONTENT_H);
    ui_add_reflective_texture(root, APP_SCREEN_W, APP_CONTENT_H);

    lv_obj_t * title = lv_label_create(root);
    lv_label_set_text(title, "同步便签");
    lv_obj_set_style_text_color(title, lv_color_hex(APP_COLOR_RED), 0);
    lv_obj_set_style_text_font(title, ui_font_title(), 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10, 10);

    note_hint = lv_label_create(root);
    lv_label_set_text(note_hint, "轻触查看详情");
    lv_obj_set_style_text_color(note_hint, lv_color_hex(APP_COLOR_BLACK), 0);
    lv_obj_set_style_text_font(note_hint, ui_font_cjk(), 0);
    lv_obj_align(note_hint, LV_ALIGN_TOP_RIGHT, -12, 12);
    ui_style_ink_line(root, 10, 36, 380, 2, APP_COLOR_RED);

    for(uint8_t i = 0; i < APP_MAX_NOTES; i++) {
        note_buttons[i] = lv_button_create(root);
        lv_obj_set_pos(note_buttons[i], 10, 42 + i * 38);
        lv_obj_set_size(note_buttons[i], 380, 32);
        lv_obj_set_style_bg_color(note_buttons[i], lv_color_hex(APP_COLOR_PAPER_DARK), 0);
        lv_obj_set_style_bg_color(note_buttons[i], lv_color_hex(APP_COLOR_RED_SOFT), LV_STATE_PRESSED);
        lv_obj_set_style_border_color(note_buttons[i], lv_color_hex(APP_COLOR_BLACK), 0);
        lv_obj_set_style_border_width(note_buttons[i], 1, 0);
        lv_obj_set_style_radius(note_buttons[i], 2, 0);
        lv_obj_add_event_cb(note_buttons[i], note_event_cb, LV_EVENT_CLICKED, NULL);
        ui_style_ink_line(note_buttons[i], 0, 0, 3, 32, APP_COLOR_RED);

        note_labels[i] = lv_label_create(note_buttons[i]);
        lv_obj_set_width(note_labels[i], 354);
        lv_label_set_long_mode(note_labels[i], LV_LABEL_LONG_DOT);
        lv_obj_set_style_text_color(note_labels[i], lv_color_hex(APP_COLOR_BLACK), 0);
        lv_obj_set_style_text_font(note_labels[i], ui_font_cjk(), 0);
        lv_obj_center(note_labels[i]);
    }

    return root;
}

void page_notes_refresh(const app_state_t * state)
{
    for(uint8_t i = 0; i < APP_MAX_NOTES; i++) {
        if(i < state->note_count) {
            lv_obj_remove_flag(note_buttons[i], LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text_fmt(note_labels[i], "%s%s", state->notes[i].pinned ? "置顶 " : "", state->notes[i].title);
        }
        else {
            lv_obj_add_flag(note_buttons[i], LV_OBJ_FLAG_HIDDEN);
        }
    }
}
