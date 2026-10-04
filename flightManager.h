#ifndef FLIGHT_MANAGER_H
#define FLIGHT_MANAGER_H

#include <Arduino.h>
#include "telemetryData.h"
#include "deployment.h"

enum FlightState
{
    BOOT = 0,
    TEST_MODE,
    LAUNCH_PAD,
    DROP_DETECTED,
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

    void update(
        const TelemetryData& data,
        DeploymentController& deployment
    );

    void updateTelemetryState(TelemetryData& data);

    FlightState getState() const;
    const char* getStateName() const;

    void transitionTo(FlightState newState);

private:
    FlightState currentState;

    uint8_t dropConfirmationCount;
    uint8_t descentConfirmationCount;

    float previousAltitude;
    bool previousAltitudeValid;

    bool initializationComplete();
    bool testsPassed();

    bool dropDetected(const TelemetryData& data);
    bool descentConfirmed(const TelemetryData& data);
    bool paragliderAltitudeReached(const TelemetryData& data);
    bool impactDetected(const TelemetryData& data);
};

#endif