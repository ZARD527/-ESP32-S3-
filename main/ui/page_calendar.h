#pragma once

#include "app_state.h"
#include "lvgl.h"

typedef void (*page_day_open_cb_t)(uint8_t day);

lv_obj_t * page_calendar_create(lv_obj_t * parent, page_day_open_cb_t open_cb);
void page_calendar_refresh(const app_state_t * state);
