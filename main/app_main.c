#include "app_state.h"
#include "display_port.h"
#include "imu_service.h"
#include "input_port.h"
#include "ui.h"

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"

#include <stdbool.h>

static const char * TAG = "lvgl_dashboard";

void app_main(void)
{
    ESP_LOGI(TAG, "starting 300x400 dashboard");

    lv_init();
    display_port_init();
    input_port_init();

    app_state_init();
    imu_service_init();
    ui_init();

    while(true) {
        uint32_t now_ms = (uint32_t)(esp_timer_get_time() / 1000ULL);
        app_state_tick(now_ms);

        app_orientation_t orientation = imu_service_read_orientation();
        app_imu_action_t action = app_state_apply_orientation(orientation);
        ui_handle_imu_action(action);

        ui_refresh();
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
