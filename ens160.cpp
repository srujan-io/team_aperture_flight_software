#include "ens160.h"

ENS160Sensor::ENS160Sensor()
{
    initialized = false;

    eco2 = 0;
    tvoc = 0;
    aqi = 0;
}

bool ENS160Sensor::begin()
{
    if (!ens160.begin())
    {
        initialized = false;
        return false;
    }

    // Standard operating mode
    ens160.setMode(ENS160_OPMODE_STD);

    initialized = true;

    return true;
}

void ENS160Sensor::update()
{
    if (!initialized)
        return;

    // Read latest ENS160 measurements
    eco2 = ens160.eCO2;
    tvoc = ens160.TVOC;
    aqi = ens160.AQI;
}

uint16_t ENS160Sensor::getECO2() const
{
    return eco2;
}

uint16_t ENS160Sensor::getTVOC() const
{
    return tvoc;
}

uint8_t ENS160Sensor::getAQI() const
{
    return aqi;
}

bool ENS160Sensor::isReady() const
{
    return initialized;
}