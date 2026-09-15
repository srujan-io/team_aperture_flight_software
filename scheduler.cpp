#include "scheduler.h"

Scheduler::Scheduler()
{
    lastSensorUpdate = 0;
    lastTelemetryUpdate = 0;
}

void Scheduler::begin()
{
    unsigned long now = millis();

    lastSensorUpdate = now;
    lastTelemetryUpdate = now;
}

bool Scheduler::sensorTask()
{
    unsigned long now = millis();

    if (now - lastSensorUpdate >= SENSOR_INTERVAL)
    {
        lastSensorUpdate = now;
        return true;
    }

    return false;
}

bool Scheduler::telemetryTask()
{
    unsigned long now = millis();

    if (now - lastTelemetryUpdate >= TELEMETRY_INTERVAL)
    {
        lastTelemetryUpdate = now;
        return true;
    }

    return false;
}