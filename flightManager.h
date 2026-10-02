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

    void update(
        const TelemetryData& data,
        DeploymentController& deployment
    );

    void updateTelemetryState(
        TelemetryData& data
    );

    FlightState getState() const;

    const char* getStateName() const;

    void transitionTo(
        FlightState newState
    );

private:

    FlightState currentState;

    uint8_t apogeeConfirmationCount;
    uint8_t descentConfirmationCount;

    float previousAltitude;
    bool previousAltitudeValid;

    bool initializationComplete();
    bool testsPassed();

    bool launchDetected(
        const TelemetryData& data
    );

    bool apogeeDetected(
        const TelemetryData& data
    );

    bool descentConfirmed(
        const TelemetryData& data
    );

    bool paragliderAltitudeReached(
        const TelemetryData& data
    );

    bool impactDetected(
        const TelemetryData& data
    );
};


private:

    FlightState currentState;


    // ============================================================
    // FLIGHT-STATE CONFIRMATION COUNTERS
    // ============================================================
    uint8_t apogeeConfirmationCount;
    uint8_t descentConfirmationCount;


    // ============================================================
    // PREVIOUS ALTITUDE
    // ============================================================

    float previousAltitude;
    bool previousAltitudeValid;


    // ============================================================
    // STATE TRANSITION HELPERS
    // ============================================================

    bool initializationComplete();
    bool testsPassed();

    bool launchDetected(
        const TelemetryData& data
    );

    bool apogeeDetected(
        const TelemetryData& data
    );

    bool descentConfirmed(
        const TelemetryData& data
    );

    bool paragliderAltitudeReached(
        const TelemetryData& data
    );

    

    bool impactDetected(
        const TelemetryData& data
    );
};

#endif