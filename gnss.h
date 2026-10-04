#ifndef GNSS_H
#define GNSS_H

#include <Arduino.h>
#include <TinyGPS++.h>

class GNSS
{
public:

    GNSS();

    void begin();
    void update();

    // ============================================================
    // GNSS FIX
    // ============================================================

    bool hasFix();

    // ============================================================
    // GNSS ACQUISITION / READINESS
    // ============================================================

    bool isReady();
    unsigned long getAcquisitionTime();

    // ============================================================
    // POSITION
    // ============================================================

    double getLatitude();
    double getLongitude();
    double getAltitude();

    // ============================================================
    // SATELLITES
    // ============================================================

    uint8_t getSatellites();

    // ============================================================
    // MOTION
    // ============================================================

    float getSpeed();
    float getCourse();
    bool hasCourse();

    // ============================================================
    // TIME
    // ============================================================

    void getTime(
        char *buffer,
        size_t bufferSize
    );

private:

    TinyGPSPlus gps;

    bool ready;
    unsigned long startTime;
    unsigned long acquisitionTime;
};

#endif