#ifndef INA260_H
#define INA260_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA260.h>

class INA260Sensor
{
public:
    INA260Sensor();

    bool begin();
    void update();

    float getVoltage() const;
    float getCurrent() const;
    float getPower() const;

    bool isReady() const;

private:
    Adafruit_INA260 ina260;

    bool initialized;

    float voltage;
    float current;
    float power;
};

#endif