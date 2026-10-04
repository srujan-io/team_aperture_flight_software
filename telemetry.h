#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "telemetryData.h"

class Telemetry
{
public:

    Telemetry();

    String createPacket(
        const TelemetryData& data
    );
};

#endif