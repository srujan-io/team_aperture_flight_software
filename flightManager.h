#ifndef FLIGHT_MANAGER_H
#define FLIGHT_MANAGER_H

#include <Arduino.h>
#include "telemetryData.h"

enum FlightState
{
    BOOT = 0,
    TEST_MODE,
    LAUNCH_PAD,
    ASCENT,
    ROCKET_DEPLOY,
    DESCENT,
    PARAGLIDER_DEPLOY,
    PARAGLIDE_ACTIVE,
    IMPACT
};

class FlightStateManager
{
public:
    FlightStateManager();

    void begin();
    void update(const TelemetryData& data);

    FlightState getState() const;
    const char* getStateName() const;

    void transitionTo(FlightState newState);

private:
    FlightState currentState;

    // Flight-state confirmation counters
    uint8_t separationConfirmationCount;
    uint8_t descentConfirmationCount;

    // Previous altitude for detecting altitude trend
    float previousAltitude;
    bool previousAltitudeValid;

    // State transition helpers
    bool initializationComplete();
    bool testsPassed();

    bool launchDetected(const TelemetryData& data);
    bool rocketSeparationDetected(const TelemetryData& data);
    bool descentConfirmed(const TelemetryData& data);
    bool paragliderAltitudeReached(const TelemetryData& data);
    bool paragliderStable(const TelemetryData& data);
    bool impactDetected(const TelemetryData& data);
};

#endif