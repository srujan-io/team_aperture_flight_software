#include "flightManager.h"
#include "config.h"

FlightStateManager::FlightStateManager()
{
    currentState = BOOT;

    separationConfirmationCount = 0;
    descentConfirmationCount = 0;

    previousAltitude = 0.0f;
    previousAltitudeValid = false;
}

void FlightStateManager::begin()
{
    currentState = BOOT;

    separationConfirmationCount = 0;
    descentConfirmationCount = 0;

    previousAltitude = 0.0f;
    previousAltitudeValid = false;

    Serial.println("Flight State Manager initialized.");
    Serial.println("FLIGHT STATE: BOOT");
}

void FlightStateManager::update(const TelemetryData& data)
{
    switch (currentState)
    {
        case BOOT:

            if (initializationComplete())
            {
                transitionTo(TEST_MODE);
            }

            break;


        case TEST_MODE:

            if (testsPassed())
            {
                transitionTo(LAUNCH_PAD);
            }

            break;


        case LAUNCH_PAD:

            if (launchDetected(data))
            {
                transitionTo(ASCENT);
            }

            break;


        case ASCENT:

            if (rocketSeparationDetected(data))
            {
                transitionTo(ROCKET_DEPLOY);
            }

            break;


        case ROCKET_DEPLOY:

            if (descentConfirmed(data))
            {
                transitionTo(DESCENT);
            }

            break;


        case DESCENT:

            if (paragliderAltitudeReached(data))
            {
                transitionTo(PARAGLIDER_DEPLOY);
            }

            break;


        case PARAGLIDER_DEPLOY:

            if (paragliderStable(data))
            {
                transitionTo(PARAGLIDE_ACTIVE);
            }

            break;


        case PARAGLIDE_ACTIVE:

            if (impactDetected(data))
            {
                transitionTo(IMPACT);
            }

            break;


        case IMPACT:

            // Final state.
            // No further automatic transitions.

            break;
    }
}


// ============================================================
// STATE INFORMATION
// ============================================================

FlightState FlightStateManager::getState() const
{
    return currentState;
}


const char* FlightStateManager::getStateName() const
{
    switch (currentState)
    {
        case BOOT:
            return "BOOT";

        case TEST_MODE:
            return "TEST_MODE";

        case LAUNCH_PAD:
            return "LAUNCH_PAD";

        case ASCENT:
            return "ASCENT";

        case ROCKET_DEPLOY:
            return "ROCKET_DEPLOY";

        case DESCENT:
            return "DESCENT";

        case PARAGLIDER_DEPLOY:
            return "PARAGLIDER_DEPLOY";

        case PARAGLIDE_ACTIVE:
            return "PARAGLIDE_ACTIVE";

        case IMPACT:
            return "IMPACT";

        default:
            return "UNKNOWN";
    }
}


// ============================================================
// STATE TRANSITION
// ============================================================

void FlightStateManager::transitionTo(FlightState newState)
{
    if (currentState == newState)
    {
        return;
    }

    currentState = newState;

    Serial.print("FLIGHT STATE -> ");
    Serial.println(getStateName());

    // Reset state-specific variables when entering a new state

    if (newState == ASCENT)
    {
        separationConfirmationCount = 0;
        descentConfirmationCount = 0;

        previousAltitudeValid = false;
    }

    if (newState == ROCKET_DEPLOY)
    {
        descentConfirmationCount = 0;
    }

    if (newState == DESCENT)
    {
        descentConfirmationCount = 0;
    }
}


// ============================================================
// BOOT
// ============================================================

bool FlightStateManager::initializationComplete()
{
    /*
     * Temporary implementation.
     *
     * Later this should be connected to actual
     * initialization/health status from the system.
     */

    return true;
}


// ============================================================
// TEST MODE
// ============================================================

bool FlightStateManager::testsPassed()
{
    /*
     * Temporary implementation.
     *
     * Later this should verify:
     * - BME280
     * - IMU
     * - GNSS
     * - SD
     * - LoRa
     * - power monitoring
     * - servos
     */

    return true;
}


// ============================================================
// LAUNCH DETECTION
// ============================================================

bool FlightStateManager::launchDetected(const TelemetryData& data)
{
    /*
     * Initial launch criterion:
     *
     * AGL altitude exceeds the launch threshold.
     *
     * This threshold is currently 10 m.
     */

    if (data.altitude >= LAUNCH_DETECTION_ALTITUDE_M)
    {
        Serial.println("Launch detected.");
        return true;
    }

    return false;
}


// ============================================================
// ROCKET SEPARATION / START OF DESCENT DETECTION
// ============================================================

bool FlightStateManager::rocketSeparationDetected(
    const TelemetryData& data)
{
    /*
     * Detect a transition from increasing altitude
     * to decreasing altitude.
     *
     * We require multiple consecutive decreasing
     * altitude samples to avoid reacting to noise.
     */

    if (!previousAltitudeValid)
    {
        previousAltitude = data.altitude;
        previousAltitudeValid = true;

        return false;
    }

    float altitudeChange =
        data.altitude - previousAltitude;

    previousAltitude = data.altitude;


    if (altitudeChange < 0.0f)
    {
        separationConfirmationCount++;

        if (separationConfirmationCount >=
            SEPARATION_CONFIRMATION_SAMPLES)
        {
            separationConfirmationCount = 0;

            Serial.println(
                "Rocket separation / descent detected.");

            return true;
        }
    }
    else
    {
        separationConfirmationCount = 0;
    }

    return false;
}


// ============================================================
// DESCENT CONFIRMATION
// ============================================================

bool FlightStateManager::descentConfirmed(
    const TelemetryData& data)
{
    /*
     * Velocity convention:
     *
     * Positive  = upward
     * Negative  = downward
     *
     * Example:
     * -15 m/s = descending at 15 m/s
     */

    if (data.velocity <= DESCENT_VELOCITY_THRESHOLD_MS)
    {
        descentConfirmationCount++;

        if (descentConfirmationCount >=
            DESCENT_CONFIRMATION_SAMPLES)
        {
            descentConfirmationCount = 0;

            Serial.println(
                "Stable parachute descent confirmed.");

            return true;
        }
    }
    else
    {
        descentConfirmationCount = 0;
    }

    return false;
}


// ============================================================
// PARAGLIDER DEPLOYMENT ALTITUDE
// ============================================================

bool FlightStateManager::paragliderAltitudeReached(
    const TelemetryData& data)
{
    /*
     * Altitude is assumed to be AGL.
     *
     * Trigger:
     * altitude <= 600 m
     */

    if (data.altitude <= PARAGLIDER_DEPLOY_ALTITUDE_M)
    {
        Serial.println(
            "Paraglider deployment altitude reached.");

        return true;
    }

    return false;
}


// ============================================================
// PARAGLIDER STABILITY
// ============================================================

bool FlightStateManager::paragliderStable(
    const TelemetryData& data)
{
    /*
     * Placeholder for Phase A.
     *
     * Later this will verify that the paraglider
     * has inflated and the vehicle has entered
     * a stable glide before activating guidance.
     */

    return false;
}


// ============================================================
// IMPACT DETECTION
// ============================================================

bool FlightStateManager::impactDetected(
    const TelemetryData& data)
{
    /*
     * Placeholder for later.
     *
     * Impact detection will eventually combine
     * altitude, velocity and IMU information.
     */

    return false;
}