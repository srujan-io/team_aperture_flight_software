#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <RadioLib.h>

#include "telemetryData.h"

class Communication
{
public:
    Communication();

    bool begin();

    bool sendTelemetry(const TelemetryData& data);

    bool receiveCommand(String& command);

    bool isInitialized() const;

    float getLastRSSI() const;
    float getLastSNR() const;

private:
    SX1262 radio;

    bool initialized;

    float lastRSSI;
    float lastSNR;

    bool initializeRadio();
    String buildTelemetryPacket(const TelemetryData& data);
};

#endif