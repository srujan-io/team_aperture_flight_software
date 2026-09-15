#include "ina260.h"

INA260Sensor::INA260Sensor()
{
    initialized = false;

    voltage = 0.0f;
    current = 0.0f;
    power = 0.0f;
}

bool INA260Sensor::begin()
{
    if (!ina260.begin())
    {
        initialized = false;
        return false;
    }

    initialized = true;

    return true;
}

void INA260Sensor::update()
{
    if (!initialized)
        return;

    // Bus voltage: mV → V
    voltage = ina260.readBusVoltage() / 1000.0f;

    // Current: mA
    current = ina260.readCurrent();

    // Power: mW
    power = ina260.readPower();
}

float INA260Sensor::getVoltage() const
{
    return voltage;
}

float INA260Sensor::getCurrent() const
{
    return current;
}

float INA260Sensor::getPower() const
{
    return power;
}

bool INA260Sensor::isReady() const
{
    return initialized;
}