#include "sensorManager.h"

bool SensorManager::begin()
{
    data = TelemetryData();

    // -------------------------
    // IMU
    // -------------------------

    imuAvailable = imu.begin();

    Serial.print("IMU available = ");
    Serial.println(imuAvailable);


    // -------------------------
    // AS7341
    // -------------------------

    as7341Available = as7341.begin();

    Serial.print("AS7341 available = ");
    Serial.println(as7341Available);


    // -------------------------
    // MLX90614
    // -------------------------

    mlx90614Available = mlx90614.begin();

    Serial.print("MLX90614 available = ");
    Serial.println(mlx90614Available);


    return true;
}


void SensorManager::update()
{
    // -------------------------
    // IMU
    // -------------------------

    if (imuAvailable)
    {
        imu.update();

        IMUData imuData = imu.getData();

        data.accelX = imuData.ax;
        data.accelY = imuData.ay;
        data.accelZ = imuData.az;
    }


    // -------------------------
    // AS7341
    // -------------------------

    if (as7341Available)
    {
        as7341.update();

        AS7341Data spectral = as7341.getData();

        data.spectralF1 = spectral.f1;
        data.spectralF2 = spectral.f2;
        data.spectralF3 = spectral.f3;
        data.spectralF4 = spectral.f4;
        data.spectralF5 = spectral.f5;
        data.spectralF6 = spectral.f6;
        data.spectralF7 = spectral.f7;
        data.spectralF8 = spectral.f8;
        data.spectralClear = spectral.clear;
        data.spectralNIR = spectral.nir;
    }


    // -------------------------
    // MLX90614
    // -------------------------

    if (mlx90614Available)
    {
        mlx90614.update();

        MLX90614Data mlxData = mlx90614.getData();

        Serial.print("MLX Object: ");
        Serial.println(mlxData.objectTemperature);

        Serial.print("MLX Ambient: ");
        Serial.println(mlxData.ambientTemperature);

        data.irTemperature = mlxData.objectTemperature;
    }
}


TelemetryData SensorManager::getData()
{
    return data;
}