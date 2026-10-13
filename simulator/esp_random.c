#include "esp_random.h"

#include <stdint.h>

uint32_t esp_random(void)
{
    static uint32_t x = 0x1234ABCDu;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}
