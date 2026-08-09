#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>

#include "telemetryData.h"
#include "imu.h"
#include "as7341.h"
#include "mlx90614.h"

class SensorManager
{
public:

    bool begin();

    void update();

    TelemetryData getData();

private:

    TelemetryData data;

    // IMU
    IMU imu;
    bool imuAvailable;

    // AS7341
    AS7341Sensor as7341;
    bool as7341Available;

    // MLX90614
    MLX90614Sensor mlx90614;
    bool mlx90614Available;
};

#endif