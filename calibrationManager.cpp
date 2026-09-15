#include "calibrationManager.h"

CalibrationManager::CalibrationManager()
{
    bme280 = nullptr;
    imu = nullptr;

    groundAltitude = 0.0f;

    accelOffsetX = 0.0f;
    accelOffsetY = 0.0f;
    accelOffsetZ = 0.0f;

    barometerCalibrated = false;
    imuCalibrated = false;
}


void CalibrationManager::begin(
    BME280Sensor* bme,
    IMU* imuSensor)
{
    bme280 = bme;
    imu = imuSensor;

    groundAltitude = 0.0f;

    accelOffsetX = 0.0f;
    accelOffsetY = 0.0f;
    accelOffsetZ = 0.0f;

    barometerCalibrated = false;
    imuCalibrated = false;
}


// ====================================================
// BAROMETER CALIBRATION
// ====================================================

bool CalibrationManager::calibrateBarometer()
{
    if (bme280 == nullptr)
    {
        Serial.println(
            "BARO calibration failed: BME280 not connected."
        );

        return false;
    }

    Serial.println("Starting BARO calibration...");
    Serial.println("Keep the CanSat stationary.");

    // Get fresh pressure reading
    bme280->update();

    float groundPressure =
        bme280->getPressure();

    if (groundPressure <= 0.0f)
    {
        Serial.println(
            "BARO calibration failed: invalid pressure."
        );

        return false;
    }

    // Set launch-pad pressure as reference
    bme280->setReferencePressure(
        groundPressure
    );

    // Update once more so altitude starts from reference
    bme280->update();

    groundAltitude =
        bme280->getAltitude();

    barometerCalibrated = true;

    Serial.print("BARO reference pressure: ");
    Serial.print(groundPressure);
    Serial.println(" hPa");

    Serial.print("Ground altitude: ");
    Serial.print(groundAltitude);
    Serial.println(" m");

    Serial.println(
        "BARO calibration successful."
    );

    return true;
}


// ====================================================
// IMU CALIBRATION
// ====================================================

bool CalibrationManager::calibrateIMU()
{
    if (imu == nullptr)
    {
        Serial.println(
            "IMU calibration failed: IMU not connected."
        );

        return false;
    }

    Serial.println("Starting IMU calibration...");
    Serial.println("Keep the CanSat completely still.");

    // Use the actual calibration function
    // implemented inside IMU class.
    imu->calibrate();

    imuCalibrated = true;

    Serial.println(
        "IMU calibration successful."
    );

    return true;
}


// ====================================================
// GETTERS
// ====================================================

float CalibrationManager::getGroundAltitude() const
{
    return groundAltitude;
}


float CalibrationManager::getAccelOffsetX() const
{
    return accelOffsetX;
}


float CalibrationManager::getAccelOffsetY() const
{
    return accelOffsetY;
}


float CalibrationManager::getAccelOffsetZ() const
{
    return accelOffsetZ;
}