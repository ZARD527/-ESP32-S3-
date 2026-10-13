#pragma once

#include "app_state.h"

#include <stdint.h>

typedef enum {
    UI_PAGE_HOME = 0,
    UI_PAGE_NOTES,
    UI_PAGE_CALENDAR,
    UI_PAGE_WEATHER_ENV,
    UI_PAGE_WIDGETS,
    UI_PAGE_COUNT,
} ui_page_t;

void ui_init(void);
void ui_refresh(void);
void ui_show_page(ui_page_t page);
void ui_handle_imu_action(app_imu_action_t action);
void ui_open_note_detail(uint8_t index);
void ui_open_day_detail(uint8_t day);
