#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "telemetryData.h"

class Telemetry
{

public:

    Telemetry();

    String createLoRaPacket(TelemetryData data);

private:

    unsigned long packetCount;

};


#endif