#ifndef AS7341_WRAPPER_H
#define AS7341_WRAPPER_H

#include <Arduino.h>
#include <Adafruit_AS7341.h>

struct AS7341Data
{
    uint16_t f1;
    uint16_t f2;
    uint16_t f3;
    uint16_t f4;
    uint16_t f5;
    uint16_t f6;
    uint16_t f7;
    uint16_t f8;
    uint16_t clear;
    uint16_t nir;
};

class AS7341Sensor
{
public:
    bool begin();
    void update();

    AS7341Data getData();

private:
    Adafruit_AS7341 sensor;
    AS7341Data data;
};

#endif