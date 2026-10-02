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

    bool hasFix();

    double getLatitude();
    double getLongitude();
    double getAltitude();

    uint8_t getSatellites();

    float getSpeed();
    float getCourse();
    bool hasCourse();

    void getTime(char *buffer, size_t bufferSize);

private:
    TinyGPSPlus gps;
};

#endif