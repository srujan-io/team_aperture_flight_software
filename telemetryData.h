#ifndef TELEMETRY_DATA_H
#define TELEMETRY_DATA_H

#include <stdint.h>

struct TelemetryData
{
    // ========================================================
    // SYSTEM
    // ========================================================

    char teamID[32];

    uint32_t timestamp;
    uint32_t packetCount;


    // ========================================================
    // BME280
    // ========================================================

    float altitude;       // m
    float pressure;      // Pa
    float temperature;    // °C
    float humidity;       // %


    // ========================================================
    // BATTERY / POWER
    // ========================================================

    float voltage;        // V


    // ========================================================
    // GNSS
    // ========================================================

    char gnssTime[16];

    double latitude;      // degrees
    double longitude;     // degrees

    float gnssAltitude;   // m

    uint8_t gnssSats;


    // ========================================================
    // IMU
    // ========================================================

    float accelX;         // m/s²
    float accelY;         // m/s²
    float accelZ;         // m/s²

    float gyroSpinRate;   // deg/s


    // ========================================================
    // FLIGHT SOFTWARE
    // ========================================================

    char flightSoftwareState[24];


    // ========================================================
    // FLIGHT PARAMETERS
    // ========================================================

    float velocity;       // m/s
    float distance;       // m


    // ========================================================
    // ENS160
    // ========================================================

    uint16_t eco2;
    uint16_t tvoc;
    uint16_t aqi;


    // ========================================================
    // MLX90614
    // ========================================================

    float irTemperature;  // °C


    // ========================================================
    // AS7341
    // ========================================================

    uint16_t spectralF1;
    uint16_t spectralF2;
    uint16_t spectralF3;
    uint16_t spectralF4;
    uint16_t spectralF5;
    uint16_t spectralF6;
    uint16_t spectralF7;
    uint16_t spectralF8;

    uint16_t spectralClear;
    uint16_t spectralNIR;
};

#endif