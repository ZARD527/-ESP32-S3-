#pragma once

#include "app_config.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    APP_ORIENTATION_NORMAL = 0,
    APP_ORIENTATION_LEFT_DOWN,
    APP_ORIENTATION_RIGHT_DOWN,
    APP_ORIENTATION_FACE_DOWN,
    APP_ORIENTATION_FACE_UP,
} app_orientation_t;

typedef enum {
    APP_IMU_ACTION_NONE = 0,
    APP_IMU_ACTION_COUNTDOWN_STARTED,
    APP_IMU_ACTION_DICE_ROLLED,
} app_imu_action_t;

typedef struct {
    uint8_t level;
    bool charging;
    int8_t wifi_rssi;
    bool cloud_online;
} app_status_t;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t weekday;
} app_clock_t;

typedef struct {
    char condition[20];
    int8_t temp_c;
    uint8_t humidity;
    int8_t high_c;
    int8_t low_c;
} app_weather_t;

typedef struct {
    float temp_c;
    uint8_t humidity;
    uint16_t co2_ppm;
    uint16_t light_lux;
} app_environment_t;

typedef struct {
    char title[APP_NOTE_TITLE_LEN];
    char body[APP_NOTE_BODY_LEN];
    bool pinned;
} app_note_t;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    char title[APP_TODO_TITLE_LEN];
    bool done;
} app_todo_t;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    char title[APP_HOLIDAY_TITLE_LEN];
} app_holiday_t;

typedef struct {
    uint32_t default_countdown_sec;
} app_settings_t;

typedef struct {
    app_status_t status;
    app_clock_t clock;
    app_weather_t weather;
    app_environment_t environment;
    app_note_t notes[APP_MAX_NOTES];
    uint8_t note_count;
    app_todo_t todos[APP_MAX_TODOS];
    uint8_t todo_count;
    app_holiday_t holidays[APP_MAX_HOLIDAYS];
    uint8_t holiday_count;
    app_settings_t settings;
    uint32_t stopwatch_ms;
    bool stopwatch_running;
    uint32_t countdown_remaining_sec;
    bool countdown_running;
    uint8_t dice_value;
    app_orientation_t orientation;
} app_state_t;

void app_state_init(void);
void app_state_tick(uint32_t now_ms);
const app_state_t * app_state_get(void);

app_imu_action_t app_state_apply_orientation(app_orientation_t orientation);
void app_state_start_countdown(void);
void app_state_stop_countdown(void);
void app_state_set_countdown_preset(uint32_t seconds);
void app_state_toggle_stopwatch(void);
void app_state_reset_stopwatch(void);
void app_state_roll_dice(void);

uint8_t app_state_count_todos_on(uint16_t year, uint8_t month, uint8_t day);
uint8_t app_state_count_holidays_on(uint16_t year, uint8_t month, uint8_t day);
