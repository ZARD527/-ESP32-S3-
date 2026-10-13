#include "imu_service.h"
#include "imu_port.h"

void imu_service_init(void)
{
    imu_port_init();
}

app_orientation_t imu_service_read_orientation(void)
{
    return imu_port_read_orientation();
}
