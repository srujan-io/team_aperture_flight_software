#ifndef CALIBRATION_MANAGER_H
#define CALIBRATION_MANAGER_H

#include <Arduino.h>

#include "bme280.h"
#include "imu.h"

class CalibrationManager
{
public:

    CalibrationManager();

    void begin(
        BME280Sensor* bme,
        IMU* imuSensor
    );

    bool calibrateBarometer();

    bool calibrateIMU();

    float getGroundAltitude() const;

    float getAccelOffsetX() const;
    float getAccelOffsetY() const;
    float getAccelOffsetZ() const;

private:

    BME280Sensor* bme280;
    IMU* imu;

    float groundAltitude;

    float accelOffsetX;
    float accelOffsetY;
    float accelOffsetZ;

    bool barometerCalibrated;
    bool imuCalibrated;
};

#endif