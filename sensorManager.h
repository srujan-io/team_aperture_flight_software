#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>

#include "telemetryData.h"

#include "imu.h"
#include "as7341.h"
#include "mlx90614.h"
#include "bme280.h"
#include "ina260.h"
#include "gnss.h"


class SensorManager
{
public:

    bool begin();

    void update();

    TelemetryData getData();


    // =================================================
    // CALIBRATION
    // =================================================

    bool calibrateIMU();
    bool calibrateBarometer();


private:

    TelemetryData data;


    // =================================================
    // VELOCITY ESTIMATION
    // =================================================

    float previousAltitude;
    unsigned long previousAltitudeTime;
    bool previousAltitudeValid;


    // =================================================
    // IMU
    // =================================================

    IMU imu;
    bool imuAvailable;


    // =================================================
    // BME280
    // =================================================

    BME280Sensor bme280;
    bool bme280Available;


    // =================================================
    // AS7341
    // =================================================

    AS7341Sensor as7341;
    bool as7341Available;


    // =================================================
    // MLX90614
    // =================================================

    MLX90614Sensor mlx90614;
    bool mlx90614Available;


    // =================================================
    // INA260
    // =================================================

    INA260Sensor ina260;
    bool ina260Available;


    // =================================================
    // GNSS
    // =================================================

    GNSS gnss;
    bool gnssAvailable;
};

#endif