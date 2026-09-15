#include "sensorManager.h"


// ====================================================
// BEGIN
// ====================================================

bool SensorManager::begin()
{
    data = TelemetryData();

    previousAltitude = 0.0f;
    previousAltitudeTime = millis();
    previousAltitudeValid = false;


    // =================================================
    // IMU
    // =================================================

    imuAvailable = imu.begin();

    Serial.print("IMU available = ");
    Serial.println(imuAvailable);


    // =================================================
    // BME280
    // =================================================

    bme280Available = bme280.begin();

    Serial.print("BME280 available = ");
    Serial.println(bme280Available);


    // =================================================
    // AS7341
    // =================================================

    as7341Available = as7341.begin();

    Serial.print("AS7341 available = ");
    Serial.println(as7341Available);


    // =================================================
    // MLX90614
    // =================================================

    mlx90614Available = mlx90614.begin();

    Serial.print("MLX90614 available = ");
    Serial.println(mlx90614Available);


    // =================================================
    // INA260
    // =================================================

    ina260Available = ina260.begin();

    Serial.print("INA260 available = ");
    Serial.println(ina260Available);


    // =================================================
    // GNSS
    // =================================================

    gnssAvailable = gnss.begin();

    Serial.print("GNSS available = ");
    Serial.println(gnssAvailable);


    return true;
}


// ====================================================
// UPDATE
// ====================================================

void SensorManager::update()
{

    // =================================================
    // IMU
    // =================================================

    if (imuAvailable)
    {
        imu.update();

        IMUData imuData =
            imu.getData();


        data.accelX =
            imuData.ax;

        data.accelY =
            imuData.ay;

        data.accelZ =
            imuData.az;


        // ------------------------------------------------
        // Gyro spin rate
        //
        // Magnitude of angular velocity vector
        // ------------------------------------------------

        data.gyroSpinRate =
            sqrt(
                imuData.gx * imuData.gx +
                imuData.gy * imuData.gy +
                imuData.gz * imuData.gz
            );
    }


    // =================================================
    // BME280
    // =================================================

    if (bme280Available)
{
    bme280.update();

    data.altitude =
        bme280.getAltitude();

    data.pressure =
        bme280.getPressure();

    data.temperature =
        bme280.getTemperature();

    data.humidity =
        bme280.getHumidity();


    // ------------------------------------------------
    // Vertical velocity
    // Positive = upward
    // Negative = downward
    // ------------------------------------------------

    unsigned long currentTime = millis();

    if (previousAltitudeValid)
    {
        float dt =
            (currentTime - previousAltitudeTime) / 1000.0f;

        if (dt > 0.0f)
        {
            data.velocity =
                (data.altitude - previousAltitude) / dt;
        }
    }
    else
    {
        data.velocity = 0.0f;
        previousAltitudeValid = true;
    }

    previousAltitude =
        data.altitude;

    previousAltitudeTime =
        currentTime;
}


    // =================================================
    // AS7341
    // =================================================

    if (as7341Available)
    {
        as7341.update();

        AS7341Data spectral =
            as7341.getData();


        data.spectralF1 =
            spectral.f1;

        data.spectralF2 =
            spectral.f2;

        data.spectralF3 =
            spectral.f3;

        data.spectralF4 =
            spectral.f4;

        data.spectralF5 =
            spectral.f5;

        data.spectralF6 =
            spectral.f6;

        data.spectralF7 =
            spectral.f7;

        data.spectralF8 =
            spectral.f8;

        data.spectralClear =
            spectral.clear;

        data.spectralNIR =
            spectral.nir;
    }


    // =================================================
    // MLX90614
    // =================================================

    if (mlx90614Available)
    {
        mlx90614.update();

        MLX90614Data mlxData =
            mlx90614.getData();


        data.irTemperature =
            mlxData.objectTemperature;
    }


    // =================================================
    // INA260
    // =================================================

    if (ina260Available)
    {
        ina260.update();

        data.voltage =
            ina260.getVoltage();
    }


    // =================================================
    // GNSS
    // =================================================

    if (gnssAvailable)
    {
        gnss.update();


        if (gnss.hasFix())
        {
            data.latitude =
                gnss.getLatitude();

            data.longitude =
                gnss.getLongitude();

            data.gnssAltitude =
                gnss.getAltitude();

            data.gnssSats =
                gnss.getSatellites();


            char timeBuffer[16];

            if (gnss.getTime(
                    timeBuffer,
                    sizeof(timeBuffer)))
            {
                strncpy(
                    data.gnssTime,
                    timeBuffer,
                    sizeof(data.gnssTime) - 1
                );

                data.gnssTime[
                    sizeof(data.gnssTime) - 1
                ] = '\0';
            }
        }
    }
}


// ====================================================
// CALIBRATE IMU
// ====================================================

bool SensorManager::calibrateIMU()
{
    if (!imuAvailable)
    {
        Serial.println(
            "Cannot calibrate IMU: unavailable."
        );

        return false;
    }


    imu.calibrate();

    return true;
}


// ====================================================
// CALIBRATE BAROMETER
// ====================================================

bool SensorManager::calibrateBarometer()
{
    if (!bme280Available)
    {
        Serial.println(
            "Cannot calibrate BARO: unavailable."
        );

        return false;
    }


    bme280.update();


    float currentPressure =
        bme280.getPressure();


    if (currentPressure <= 0)
    {
        Serial.println(
            "Invalid barometric pressure."
        );

        return false;
    }


    bme280.setReferencePressure(
        currentPressure
    );


    data.altitude = 0.0f;


    Serial.print(
        "BARO reference pressure set to: "
    );

    Serial.println(
        currentPressure
    );


    return true;
}


// ====================================================
// GET DATA
// ====================================================

TelemetryData SensorManager::getData()
{
    return data;
}