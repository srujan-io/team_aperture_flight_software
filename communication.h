#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>

class Communication
{
public:

    Communication();

    bool begin();

    bool sendTelemetry(const String& packet);

    bool isAvailable();

private:

    bool initialized;
};

#endif