#ifndef MLX90614_SENSOR_H
#define MLX90614_SENSOR_H

#include <Arduino.h>
#include <Adafruit_MLX90614.h>

struct MLX90614Data
{
    float objectTemperature;
    float ambientTemperature;
};

class MLX90614Sensor
{
public:
    bool begin();
    void update();

    MLX90614Data getData();

private:
    Adafruit_MLX90614 sensor;
    MLX90614Data data;
};

#endif