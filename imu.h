#ifndef IMU_H
#define IMU_H

#include "kalman.h"
#include <Arduino.h>
#include <Wire.h>
#include <MPU9250_asukiaaa.h>

struct IMUData
{
    // Acceleration
    float ax;
    float ay;
    float az;

    // Gyroscope
    float gx;
    float gy;
    float gz;

    // Orientation
    float roll;
    float pitch;

    // Total acceleration magnitude
    float accelMagnitude;

    // Time of latest reading
    uint32_t timestamp;

    // Status
    bool healthy;
    bool calibrated;
    bool stationary;
};

class IMU
{
public:
    bool begin();
    void calibrate();
    void update();

    IMUData getData() const;

private:
    MPU9250_asukiaaa mpu;
    IMUData data;

    float gyroBiasX = 0.0f;
    float gyroBiasY = 0.0f;
    float gyroBiasZ = 0.0f;

    unsigned long previousTime = 0;

    Kalman kalmanRoll;
    Kalman kalmanPitch;

    bool firstUpdate = true;
};

#endif