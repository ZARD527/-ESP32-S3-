#include "app_state.h"
#include "holiday_service.h"
#include "sensor_service.h"
#include "sync_service.h"
#include "weather_service.h"

#include "esp_random.h"

#include <string.h>
#include <time.h>

static app_state_t state;
static uint32_t last_second_ms;
static uint32_t last_service_ms;

static void init_clock(void)
{
    time_t now = time(NULL);
    struct tm tm_now;
#ifdef _WIN32
    localtime_s(&tm_now, &now);
#else
    localtime_r(&now, &tm_now);
#endif

    if(tm_now.tm_year < 120) {
        state.clock.year = 2026;
        state.clock.month = 9;
        state.clock.day = 11;
        state.clock.hour = 12;
        state.clock.minute = 0;
        state.clock.second = 0;
        state.clock.weekday = 5;
        return;
    }

    state.clock.year = (uint16_t)(tm_now.tm_year + 1900);
    state.clock.month = (uint8_t)(tm_now.tm_mon + 1);
    state.clock.day = (uint8_t)tm_now.tm_mday;
    state.clock.hour = (uint8_t)tm_now.tm_hour;
    state.clock.minute = (uint8_t)tm_now.tm_min;
    state.clock.second = (uint8_t)tm_now.tm_sec;
    state.clock.weekday = (uint8_t)tm_now.tm_wday;
}

void app_state_init(void)
{
    memset(&state, 0, sizeof(state));

    state.status.level = 86;
    state.status.charging = false;
    state.status.wifi_rssi = -48;
    state.status.cloud_online = true;
    state.settings.default_countdown_sec = APP_DEFAULT_COUNTDOWN_SEC;
    state.countdown_remaining_sec = state.settings.default_countdown_sec;
    state.dice_value = 1;
    state.orientation = APP_ORIENTATION_NORMAL;

    init_clock();
    weather_service_fetch(&state.weather);
    sensor_service_read(&state.environment);
    sync_service_pull_notes(state.notes, &state.note_count, APP_MAX_NOTES);
    sync_service_pull_todos(state.todos, &state.todo_count, APP_MAX_TODOS);
    holiday_service_pull(state.holidays, &state.holiday_count, APP_MAX_HOLIDAYS);
}

void app_state_tick(uint32_t now_ms)
{
    if(now_ms - last_second_ms >= 1000U) {
        last_second_ms = now_ms;
        init_clock();

        if(state.stopwatch_running) {
            state.stopwatch_ms += 1000U;
        }

        if(state.countdown_running && state.countdown_remaining_sec > 0) {
            state.countdown_remaining_sec--;
            if(state.countdown_remaining_sec == 0) {
                state.countdown_running = false;
            }
        }
    }

    if(now_ms - last_service_ms >= 5000U) {
        last_service_ms = now_ms;
        sensor_service_read(&state.environment);
        weather_service_fetch(&state.weather);
    }
}

const app_state_t * app_state_get(void)
{
    return &state;
}

app_imu_action_t app_state_apply_orientation(app_orientation_t orientation)
{
    if(orientation == state.orientation) {
        return APP_IMU_ACTION_NONE;
    }

    state.orientation = orientation;
    if(orientation == APP_ORIENTATION_LEFT_DOWN) {
        app_state_start_countdown();
        return APP_IMU_ACTION_COUNTDOWN_STARTED;
    }

    if(orientation == APP_ORIENTATION_RIGHT_DOWN) {
        app_state_roll_dice();
        return APP_IMU_ACTION_DICE_ROLLED;
    }

    return APP_IMU_ACTION_NONE;
}

void app_state_start_countdown(void)
{
    if(state.countdown_remaining_sec == 0) {
        state.countdown_remaining_sec = state.settings.default_countdown_sec;
    }
    state.countdown_running = true;
}

void app_state_stop_countdown(void)
{
    state.countdown_running = false;
}

void app_state_set_countdown_preset(uint32_t seconds)
{
    if(seconds < 60U) seconds = 60U;
    if(seconds > 3600U) seconds = 3600U;

    state.settings.default_countdown_sec = seconds;
    if(!state.countdown_running) {
        state.countdown_remaining_sec = seconds;
    }
}

void app_state_toggle_stopwatch(void)
{
    state.stopwatch_running = !state.stopwatch_running;
}

void app_state_reset_stopwatch(void)
{
    state.stopwatch_running = false;
    state.stopwatch_ms = 0;
}

void app_state_roll_dice(void)
{
    state.dice_value = (uint8_t)((esp_random() % 6U) + 1U);
}

uint8_t app_state_count_todos_on(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t count = 0;
    for(uint8_t i = 0; i < state.todo_count; i++) {
        const app_todo_t * todo = &state.todos[i];
        if(todo->year == year && todo->month == month && todo->day == day) {
            count++;
        }
    }
    return count;
}

uint8_t app_state_count_holidays_on(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t count = 0;
    for(uint8_t i = 0; i < state.holiday_count; i++) {
        const app_holiday_t * holiday = &state.holidays[i];
        if(holiday->year == year && holiday->month == month && holiday->day == day) {
            count++;
        }
    }
    return count;
}
