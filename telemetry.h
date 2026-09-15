#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "telemetryData.h"

class Telemetry
{
public:

    Telemetry();

    String createLoRaPacket(
        const TelemetryData& data
    );
};

#endif