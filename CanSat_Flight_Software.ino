#include "sensorManager.h"
#include "storage.h"

SensorManager sensors;
Storage storage;

SensorManager sensors;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println();
    Serial.println("==============================");
    Serial.println("   CanSat Sensor Test");
    Serial.println("==============================");

    if (!sensors.begin())
    {
        Serial.println("Sensor Manager initialization failed!");
        while (1);
    }

    Serial.println("Sensor Manager ready!");

    if (!storage.begin())
{
    Serial.println("Storage initialization failed!");
}
else
{
    Serial.println("Storage ready!");
}

}

void loop()
{
    sensors.update();

    TelemetryData data = sensors.getData();

    Serial.println();
    Serial.println("========== MLX90614 ==========");

    Serial.print("IR/Object Temperature: ");
    Serial.print(data.irTemperature);
    Serial.println(" C");

    Serial.println("==============================");

    delay(1000);
}