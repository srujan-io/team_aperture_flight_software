#ifndef ENS160_H
#define ENS160_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ENS160.h>

class ENS160Sensor
{
public:
    ENS160Sensor();

    bool begin();
    void update();

    uint16_t getECO2() const;
    uint16_t getTVOC() const;
    uint8_t getAQI() const;

    bool isReady() const;

private:
    Adafruit_ENS160 ens160;

    bool initialized;

    uint16_t eco2;
    uint16_t tvoc;
    uint8_t aqi;
};

#endif