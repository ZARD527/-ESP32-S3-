#pragma once

#include "app_state.h"

void imu_service_init(void);
app_orientation_t imu_service_read_orientation(void);
