#include "scheduler.h"


Scheduler::Scheduler()
{
    lastSensorUpdate = 0;
    lastTelemetryUpdate = 0;
}


// INITIALIZE SCHEDULER

void Scheduler::begin()
{
    unsigned long now = millis();

    lastSensorUpdate = now;
    lastTelemetryUpdate = now;
}


// SENSOR TASK
// 10 Hz

bool Scheduler::sensorTask()
{
    unsigned long now = millis();

    if (now - lastSensorUpdate >= SENSOR_INTERVAL)
    {
        // Advance by the fixed interval rather than jumping
        // directly to 'now'. This prevents long-term drift.
        lastSensorUpdate += SENSOR_INTERVAL;

        return true;
    }

    return false;
}


// TELEMETRY TASK
// 1 Hz

bool Scheduler::telemetryTask()
{
    unsigned long now = millis();

    if (now - lastTelemetryUpdate >= TELEMETRY_INTERVAL)
    {
        // Advance by the fixed interval to maintain timing.
        lastTelemetryUpdate += TELEMETRY_INTERVAL;

        return true;
    }

    return false;
}