#pragma once

#include "app_state.h"

void imu_port_init(void);
app_orientation_t imu_port_read_orientation(void);
