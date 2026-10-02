#include "gnss.h"

GNSS::GNSS()
{
}

void GNSS::begin()
{
    // L89HA GNSS UART
    // Pico GP1 = RX
    // Pico GP0 = TX

    Serial1.setRX(1);
    Serial1.setTX(0);

    Serial1.begin(9600);
}

void GNSS::update()
{
    while (Serial1.available())
    {
        gps.encode(Serial1.read());
    }
}

bool GNSS::hasFix()
{
    return gps.location.isValid() &&
           gps.location.age() < 2000;
}

double GNSS::getLatitude()
{
    if (gps.location.isValid())
        return gps.location.lat();

    return 0.0;
}

double GNSS::getLongitude()
{
    if (gps.location.isValid())
        return gps.location.lng();

    return 0.0;
}

double GNSS::getAltitude()
{
    if (gps.altitude.isValid())
        return gps.altitude.meters();

    return 0.0;
}

uint8_t GNSS::getSatellites()
{
    if (gps.satellites.isValid())
        return gps.satellites.value();

    return 0;
}

float GNSS::getSpeed()
{
    if (gps.speed.isValid())
        return gps.speed.kmph();

    return 0.0f;
}

float GNSS::getCourse()
{
    if (gps.course.isValid())
        return gps.course.deg();

    return 0.0f;
}

bool GNSS::hasCourse()
{
    return gps.course.isValid() &&
           gps.course.age() < 2000;
}

void GNSS::getTime(char *buffer, size_t bufferSize)
{
    if (buffer == nullptr || bufferSize == 0)
        return;

    if (!gps.time.isValid())
    {
        strncpy(buffer, "NO_DATA", bufferSize);
        buffer[bufferSize - 1] = '\0';
        return;
    }

    snprintf(
        buffer,
        bufferSize,
        "%02d:%02d:%02d",
        gps.time.hour(),
        gps.time.minute(),
        gps.time.second()
    );
}