#include "mlx90614.h"

bool MLX90614Sensor::begin()
{
    if (!sensor.begin())
    {
        Serial.println("MLX90614 not detected.");
        return false;
    }

    data.objectTemperature = 0.0;
    data.ambientTemperature = 0.0;

    Serial.println("MLX90614 initialized.");

    return true;
}

void MLX90614Sensor::update()
{
    data.objectTemperature = sensor.readObjectTempC();
    data.ambientTemperature = sensor.readAmbientTempC();
}

MLX90614Data MLX90614Sensor::getData()
{
    return data;
}