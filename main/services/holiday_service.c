#include "holiday_service.h"

#include <string.h>

void holiday_service_pull(app_holiday_t * holidays, uint8_t * count, uint8_t max_count)
{
    if(holidays == NULL || count == NULL || max_count == 0) return;

    static const app_holiday_t demo_holidays[] = {
        { 2026, 9, 10, "教师节" },
        { 2026, 10, 1, "国庆节" },
        { 2026, 10, 6, "中秋节" },
    };

    uint8_t n = (uint8_t)(sizeof(demo_holidays) / sizeof(demo_holidays[0]));
    if(n > max_count) n = max_count;

    memcpy(holidays, demo_holidays, sizeof(demo_holidays[0]) * n);
    *count = n;
}
