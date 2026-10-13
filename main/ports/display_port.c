#include "display_port.h"
#include "app_config.h"

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lvgl.h"

#include <string.h>

static const char * TAG = "display_port";
static esp_timer_handle_t tick_timer;

static void tick_cb(void * arg)
{
    (void)arg;
    lv_tick_inc(1);
}

static void null_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    (void)area;
    (void)px_map;
    lv_display_flush_ready(disp);
}

void display_port_init(void)
{
    const size_t line_count = 40;
    const size_t buf_size = APP_SCREEN_W * line_count * sizeof(lv_color_t);
    void * buf1 = heap_caps_malloc(buf_size, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    void * buf2 = heap_caps_malloc(buf_size, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);

    if(buf1 == NULL || buf2 == NULL) {
        ESP_LOGE(TAG, "failed to allocate LVGL draw buffers");
        return;
    }

    memset(buf1, 0, buf_size);
    memset(buf2, 0, buf_size);

    lv_display_t * display = lv_display_create(APP_SCREEN_W, APP_SCREEN_H);
    lv_display_set_flush_cb(display, null_flush_cb);
    lv_display_set_buffers(display, buf1, buf2, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_default(display);

    const esp_timer_create_args_t timer_args = {
        .callback = tick_cb,
        .name = "lv_tick",
    };
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(tick_timer, 1000));

    ESP_LOGI(TAG, "null display ready; replace flush callback with panel driver");
}
