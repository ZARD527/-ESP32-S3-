#pragma once

#include "app_state.h"
#include "lvgl.h"

typedef void (*page_note_open_cb_t)(uint8_t index);

lv_obj_t * page_notes_create(lv_obj_t * parent, page_note_open_cb_t open_cb);
void page_notes_refresh(const app_state_t * state);
