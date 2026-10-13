#include "weather_service.h"

#include <string.h>

void weather_service_fetch(app_weather_t * out)
{
    if(out == NULL) return;

    strncpy(out->condition, "多云", sizeof(out->condition) - 1U);
    out->condition[sizeof(out->condition) - 1U] = '\0';
    out->temp_c = 24;
    out->humidity = 62;
    out->high_c = 27;
    out->low_c = 19;
}
