#include "sync_service.h"

#include <string.h>

void sync_service_pull_notes(app_note_t * notes, uint8_t * count, uint8_t max_count)
{
    if(notes == NULL || count == NULL || max_count == 0) return;

    static const app_note_t demo_notes[] = {
        { "采购清单", "确认屏幕排线、IMU 模块、电池座和外壳螺丝。", true },
        { "固件设计", "页面只负责显示，同步、天气、传感器都从服务层进入。", false },
        { "桌面提醒", "后续可以长按便签，把它固定到首页的小栏。", false },
    };

    uint8_t n = (uint8_t)(sizeof(demo_notes) / sizeof(demo_notes[0]));
    if(n > max_count) n = max_count;

    memcpy(notes, demo_notes, sizeof(demo_notes[0]) * n);
    *count = n;
}

void sync_service_pull_todos(app_todo_t * todos, uint8_t * count, uint8_t max_count)
{
    if(todos == NULL || count == NULL || max_count == 0) return;

    static const app_todo_t demo_todos[] = {
        { 2026, 9, 11, "点亮 LVGL 横屏界面", false },
        { 2026, 9, 11, "确定屏幕驱动芯片", false },
        { 2026, 9, 12, "测试 Wi-Fi 同步", false },
        { 2026, 9, 18, "校准环境传感器", false },
    };

    uint8_t n = (uint8_t)(sizeof(demo_todos) / sizeof(demo_todos[0]));
    if(n > max_count) n = max_count;

    memcpy(todos, demo_todos, sizeof(demo_todos[0]) * n);
    *count = n;
}
