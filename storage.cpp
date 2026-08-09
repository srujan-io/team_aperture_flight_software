#include "storage.h"

#include <SPI.h>
#include <SD.h>

bool Storage::begin()
{
    SPI.setRX(16);
    SPI.setTX(19);
    SPI.setSCK(18);

    SPI.begin();

    if (!SD.begin(CS_PIN))
    {
        Serial.println("SD initialization failed!");
        return false;
    }

    Serial.println("SD initialized successfully.");

    if (!SD.exists("telemetry.csv"))
    {
        File file = SD.open("telemetry.csv", FILE_WRITE);

        if (!file)
        {
            Serial.println("Failed to create telemetry.csv");
            return false;
        }

        file.println(
            "TEAM_ID,"
            "TIME_STAMPING,"
            "PACKET_COUNT,"
            "ALTITUDE,"
            "PRESSURE,"
            "TEMPERATURE,"
            "VOLTAGE,"
            "GNSS_TIME,"
            "GNSS_LATITUDE,"
            "GNSS_LONGITUDE,"
            "GNSS_ALTITUDE,"
            "GNSS_SATS,"
            "ACCEL_X,"
            "ACCEL_Y,"
            "ACCEL_Z,"
            "GYRO_SPIN_RATE,"
            "FLIGHT_SOFTWARE_STATE,"
            "HUMIDITY,"
            "VELOCITY,"
            "DISTANCE,"
            "ECO2,"
            "TVOC,"
            "AQI,"
            "IR_TEMPERATURE,"
            "SPECTRAL_F1,"
            "SPECTRAL_F2,"
            "SPECTRAL_F3,"
            "SPECTRAL_F4,"
            "SPECTRAL_F5,"
            "SPECTRAL_F6,"
            "SPECTRAL_F7,"
            "SPECTRAL_F8,"
            "SPECTRAL_CLEAR,"
            "SPECTRAL_NIR"
        );

        file.close();

        Serial.println("telemetry.csv created.");
    }

    return true;
}

bool Storage::writeTelemetry(const TelemetryData &data)
{
    File file = SD.open("telemetry.csv", FILE_WRITE);

    if (!file)
    {
        Serial.println("Failed to open telemetry.csv");
        return false;
    }

    file.print(data.teamID);
    file.print(",");

    file.print(data.timestamp);
    file.print(",");

    file.print(data.packetCount);
    file.print(",");

    file.print(data.altitude);
    file.print(",");

    file.print(data.pressure);
    file.print(",");

    file.print(data.temperature);
    file.print(",");

    file.print(data.voltage);
    file.print(",");

    file.print(data.gnssTime);
    file.print(",");

    // Actual struct names are latitude / longitude
    file.print(data.latitude, 6);
    file.print(",");

    file.print(data.longitude, 6);
    file.print(",");

    file.print(data.gnssAltitude);
    file.print(",");

    file.print(data.gnssSats);
    file.print(",");

    file.print(data.accelX);
    file.print(",");

    file.print(data.accelY);
    file.print(",");

    file.print(data.accelZ);
    file.print(",");

    file.print(data.gyroSpinRate);
    file.print(",");

    file.print(data.flightSoftwareState);
    file.print(",");

    file.print(data.humidity);
    file.print(",");

    file.print(data.velocity);
    file.print(",");

    file.print(data.distance);
    file.print(",");

    file.print(data.eco2);
    file.print(",");

    file.print(data.tvoc);
    file.print(",");

    file.print(data.aqi);
    file.print(",");

    file.print(data.irTemperature);
    file.print(",");

    file.print(data.spectralF1);
    file.print(",");

    file.print(data.spectralF2);
    file.print(",");

    file.print(data.spectralF3);
    file.print(",");

    file.print(data.spectralF4);
    file.print(",");

    file.print(data.spectralF5);
    file.print(",");

    file.print(data.spectralF6);
    file.print(",");

    file.print(data.spectralF7);
    file.print(",");

    file.print(data.spectralF8);
    file.print(",");

    file.print(data.spectralClear);
    file.print(",");

    file.println(data.spectralNIR);

    file.close();

    return true;
}