#include "sensor_service.h"

#include "esp_random.h"

void sensor_service_read(app_environment_t * out)
{
    if(out == NULL) return;

    out->temp_c = 23.5f + (float)(esp_random() % 20U) / 10.0f;
    out->humidity = (uint8_t)(48U + (esp_random() % 18U));
    out->co2_ppm = (uint16_t)(520U + (esp_random() % 180U));
    out->light_lux = (uint16_t)(180U + (esp_random() % 420U));
}
