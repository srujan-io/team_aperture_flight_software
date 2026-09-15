#include "bme280.h"

BME280Sensor::BME280Sensor()
{
    initialized = false;

    temperature = 0.0f;
    pressure = 0.0f;
    humidity = 0.0f;
    altitude = 0.0f;

    referencePressure = 1013.25f;
}

bool BME280Sensor::begin()
{
    if (bme.begin(0x76, &Wire))
    {
        initialized = true;
    }
    else if (bme.begin(0x77, &Wire))
    {
        initialized = true;
    }
    else
    {
        initialized = false;
        return false;
    }

    altitudeFilter.reset(0.0f);

    return true;
}

void BME280Sensor::update()
{
    if (!initialized)
        return;

    
    temperature = bme.readTemperature();

    pressure = bme.readPressure() / 100.0f;   

    humidity = bme.readHumidity();

    
    float rawAltitude =
        bme.readAltitude(referencePressure);

    
    altitude =
        altitudeFilter.update(rawAltitude);
}

float BME280Sensor::getTemperature() const
{
    return temperature;
}

float BME280Sensor::getPressure() const
{
    return pressure;
}

float BME280Sensor::getHumidity() const
{
    return humidity;
}

float BME280Sensor::getAltitude() const
{
    return altitude;
}

bool BME280Sensor::isReady() const
{
    return initialized;
}

void BME280Sensor::setReferencePressure(float pressure)
{
    referencePressure = pressure;
    altitudeFilter.reset(0.0f);
}

float BME280Sensor::getReferencePressure() const
{
    return referencePressure;
}