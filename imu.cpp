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

    data.ax = 0.0f;
    data.ay = 0.0f;
    data.az = 0.0f;

    data.gx = 0.0f;
    data.gy = 0.0f;
    data.gz = 0.0f;

    data.roll = 0.0f;
    data.pitch = 0.0f;

    data.accelMagnitude = 0.0f;
    data.timestamp = millis();

    data.healthy = true;
    data.calibrated = false;
    data.stationary = false;

    firstUpdate = true;

    return true;
}

void IMU::calibrate()
{
    const int samples = 500;

    gyroBiasX = 0.0f;
    gyroBiasY = 0.0f;
    gyroBiasZ = 0.0f;

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

    Serial.print("Gyro Bias X: ");
    Serial.println(gyroBiasX);

    Serial.print("Gyro Bias Y: ");
    Serial.println(gyroBiasY);

    Serial.print("Gyro Bias Z: ");
    Serial.println(gyroBiasZ);
}

void IMU::update()
{
    if (!data.healthy)
        return;

    mpu.accelUpdate();
    mpu.gyroUpdate();

    // -------------------------
    // Raw accelerometer
    // -------------------------

    data.ax = mpu.accelX();
    data.ay = mpu.accelY();
    data.az = mpu.accelZ();

    // -------------------------
    // Raw gyroscope
    // -------------------------

    data.gx = mpu.gyroX() - gyroBiasX;
    data.gy = mpu.gyroY() - gyroBiasY;
    data.gz = mpu.gyroZ() - gyroBiasZ;

    // -------------------------
    // Acceleration magnitude
    // -------------------------

    data.accelMagnitude =
        sqrt(
            data.ax * data.ax +
            data.ay * data.ay +
            data.az * data.az
        );

    // -------------------------
    // Accelerometer angles
    // -------------------------

    float accelRoll =
        atan2(
            data.ay,
            data.az
        ) * 180.0f / PI;

    float accelPitch =
        atan2(
            -data.ax,
            sqrt(
                data.ay * data.ay +
                data.az * data.az
            )
        ) * 180.0f / PI;

    // -------------------------
    // Time difference
    // -------------------------

    unsigned long currentTime = millis();

    float dt =
        (currentTime - previousTime) / 1000.0f;

    previousTime = currentTime;

    if (dt <= 0.0f)
        dt = 0.001f;

    // -------------------------
    // Initialize Kalman filter
    // -------------------------

    if (firstUpdate)
    {
        kalmanRoll.setAngle(accelRoll);
        kalmanPitch.setAngle(accelPitch);

        firstUpdate = false;
    }

    // -------------------------
    // Kalman filtering
    // -------------------------

    data.roll =
        kalmanRoll.update(
            accelRoll,
            data.gx,
            dt
        );

    data.pitch =
        kalmanPitch.update(
            accelPitch,
            data.gy,
            dt
        );

    // -------------------------
    // Stationary detection
    // -------------------------

    const float gyroThreshold = 1.0f;

    data.stationary =
        (fabs(data.gx) < gyroThreshold) &&
        (fabs(data.gy) < gyroThreshold) &&
        (fabs(data.gz) < gyroThreshold);

    // -------------------------
    // Timestamp
    // -------------------------

    data.timestamp = millis();
}

IMUData IMU::getData() const
{
    return data;
}