#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include "telemetryData.h"

class Storage
{
public:

    bool begin();

    bool writeTelemetry(
        const TelemetryData& data
    );

private:

    static const int CS_PIN = 17;

    const char* filename = "/telemetry.csv";
};

#endif