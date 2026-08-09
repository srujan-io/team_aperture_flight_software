#include "as7341.h"

bool AS7341Sensor::begin()
{
    if (!sensor.begin())
    {
        Serial.println("AS7341 not detected.");
        return false;
    }

    sensor.setATIME(100);
    sensor.setASTEP(999);
    sensor.setGain(AS7341_GAIN_256X);

    Serial.println("AS7341 initialized.");

    return true;
}

void AS7341Sensor::update()
{
    Serial.println("AS7341 wrapper update");

    if (!sensor.readAllChannels())
    {
        Serial.println("WRAPPER READ FAILED");
        return;
    }

    Serial.println("WRAPPER READ SUCCESS");

    uint16_t testF1 =
        sensor.getChannel(AS7341_CHANNEL_415nm_F1);

    uint16_t testF2 =
        sensor.getChannel(AS7341_CHANNEL_445nm_F2);

    Serial.print("Wrapper raw F1: ");
    Serial.println(testF1);

    Serial.print("Wrapper raw F2: ");
    Serial.println(testF2);

    data.f1 = testF1;
    data.f2 = testF2;

    data.f3 = sensor.getChannel(AS7341_CHANNEL_480nm_F3);
    data.f4 = sensor.getChannel(AS7341_CHANNEL_515nm_F4);
    data.f5 = sensor.getChannel(AS7341_CHANNEL_555nm_F5);
    data.f6 = sensor.getChannel(AS7341_CHANNEL_590nm_F6);
    data.f7 = sensor.getChannel(AS7341_CHANNEL_630nm_F7);
    data.f8 = sensor.getChannel(AS7341_CHANNEL_680nm_F8);

    data.clear = sensor.getChannel(AS7341_CHANNEL_CLEAR);
    data.nir = sensor.getChannel(AS7341_CHANNEL_NIR);

    Serial.print("Stored F1: ");
    Serial.println(data.f1);

    Serial.print("Stored F2: ");
    Serial.println(data.f2);
}

AS7341Data AS7341Sensor::getData()
{
    return data;
}