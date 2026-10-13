#pragma once

#include "app_state.h"
#include "lvgl.h"

lv_obj_t * page_home_create(lv_obj_t * parent);
void page_home_refresh(const app_state_t * state);
