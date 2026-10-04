#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

class Scheduler
{
public:

    Scheduler();

    void begin();

    bool sensorTask();
    bool telemetryTask();

private:

    unsigned long lastSensorUpdate;
    unsigned long lastTelemetryUpdate;

    static const unsigned long SENSOR_INTERVAL = 100UL;       // 10 Hz
    static const unsigned long TELEMETRY_INTERVAL = 1000UL;  // 1 Hz
};

#endif