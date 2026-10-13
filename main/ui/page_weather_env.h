#pragma once

#include "app_state.h"
#include "lvgl.h"

lv_obj_t * page_weather_env_create(lv_obj_t * parent);
void page_weather_env_refresh(const app_state_t * state);
