#ifndef BME280_H
#define BME280_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

#include "kalmanaltitude.h"

class BME280Sensor
{
public:
    BME280Sensor();

    bool begin();
    void update();

    float getTemperature() const;
    float getPressure() const;
    float getHumidity() const;
    float getAltitude() const;

    bool isReady() const;

    void setReferencePressure(float pressure);
    float getReferencePressure() const;

private:
    Adafruit_BME280 bme;

    bool initialized;

    float temperature;
    float pressure;
    float humidity;
    float altitude;

    float referencePressure;

    KalmanAltitude altitudeFilter;
};

#endif