#include "imu.h"
#include <math.h>

bool IMU::begin()
{
    Wire.begin();

    Wire.beginTransmission(0x68);

    if (Wire.endTransmission() != 0)
    {
        Serial.println("IMU not found!");
        data.healthy = false;
        return false;
    }

    mpu.setWire(&Wire);

    mpu.beginAccel();
    mpu.beginGyro();

    previousTime = millis();

    data.healthy = true;
    data.calibrated = false;
    data.stationary = false;

return true;
}

void IMU::calibrate()
{
    const int samples = 500;

    gyroBiasX = 0;
    gyroBiasY = 0;
    gyroBiasZ = 0;

    Serial.println("Calibrating IMU...");
    Serial.println("Keep the CanSat still.");

    for (int i = 0; i < samples; i++)
    {
        mpu.gyroUpdate();

        gyroBiasX += mpu.gyroX();
        gyroBiasY += mpu.gyroY();
        gyroBiasZ += mpu.gyroZ();

        delay(5);
    }

    gyroBiasX /= samples;
    gyroBiasY /= samples;
    gyroBiasZ /= samples;

    data.calibrated = true;

    Serial.println("Calibration Complete.");
}

void IMU::update()
{
    mpu.accelUpdate();
    mpu.gyroUpdate();

    // Raw accelerometer
    data.ax = mpu.accelX();
    data.ay = mpu.accelY();
    data.az = mpu.accelZ();

    // Raw gyroscope
    data.gx = mpu.gyroX() - gyroBiasX;
    data.gy = mpu.gyroY() - gyroBiasY;
    data.gz = mpu.gyroZ() - gyroBiasZ;

    // Magnitude
    data.accelMagnitude =sqrt(data.ax * data.ax +data.ay * data.ay +data.az * data.az);

    // Accelerometer angles
    float accelRoll =
        atan2(data.ay, data.az) * 180.0 / PI;

    float accelPitch =
        atan2(-data.ax,
              sqrt(data.ay * data.ay +data.az * data.az))* 180.0 / PI;

    // Time difference
    unsigned long currentTime = millis();

    float dt =
        (currentTime - previousTime) / 1000.0f;

    previousTime = currentTime;

    // Initialize Kalman filter once
    if (firstUpdate)
    {
        kalmanRoll.setAngle(accelRoll);
        kalmanPitch.setAngle(accelPitch);

        firstUpdate = false;
    }

    // Kalman filtering
    data.roll =
        kalmanRoll.update(accelRoll,data.gx,dt);

    data.pitch =
        kalmanPitch.update(accelPitch,data.gy,dt);
    const float gyroThreshold = 1.0f;

    data.stationary =
        (fabs(data.gx) < gyroThreshold) &&
    (fabs(data.gy) < gyroThreshold) &&
    (fabs(data.gz) < gyroThreshold);
}

IMUData IMU::getData()
{
    return data;
}