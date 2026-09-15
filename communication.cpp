#include "communication.h"

Communication::Communication()
{
    initialized = false;
}

bool Communication::begin()
{
    // LoRa initialization will be added
    // once the E32 frequency/channel is finalized.

    initialized = true;

    Serial.println("Communication module initialized.");

    return true;
}

bool Communication::sendTelemetry(const String& packet)
{
    if (!initialized)
        return false;

    // E32 transmission will be connected here.
    // Do NOT transmit yet because the RF configuration
    // has not been finalized.

    return true;
}

bool Communication::telemetryAvailable()
{
    return initialized;
}