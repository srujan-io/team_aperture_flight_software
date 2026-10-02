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


// ============================================================
// BEGIN
// ============================================================

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


// ============================================================
// UPDATE
// ============================================================

void FlightStateManager::update(
    const TelemetryData& data,
    DeploymentController& deployment)
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

            if (apogeeDetected(data))
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

        deployment.startDeployment();
    }

    break;


        // ====================================================
        // PHASE B
        // ====================================================

        case PARAGLIDER_DEPLOY:

    if (deployment.isDeployed())
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
// UPDATE TELEMETRY STATE
// ============================================================

void FlightStateManager::updateTelemetryState(
    TelemetryData& data)
{
    strncpy(
        data.flightSoftwareState,
        getStateName(),
        sizeof(data.flightSoftwareState) - 1
    );

    data.flightSoftwareState[
        sizeof(data.flightSoftwareState) - 1
    ] = '\0';
}


// ============================================================
// STATE TRANSITION
// ============================================================

void FlightStateManager::transitionTo(
    FlightState newState)
{
    if (currentState == newState)
    {
        return;
    }

    currentState = newState;

    Serial.print("FLIGHT STATE -> ");
    Serial.println(getStateName());


    // --------------------------------------------------------
    // Entering ASCENT
    // --------------------------------------------------------

    if (newState == ASCENT)
    {
        separationConfirmationCount = 0;
        descentConfirmationCount = 0;

        previousAltitudeValid = false;
    }


    // --------------------------------------------------------
    // Entering ROCKET_DEPLOY
    // --------------------------------------------------------

    if (newState == ROCKET_DEPLOY)
    {
        descentConfirmationCount = 0;
    }


    // --------------------------------------------------------
    // Entering DESCENT
    // --------------------------------------------------------

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
     * Later this will be connected to the actual
     * system initialization status.
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

bool FlightStateManager::launchDetected(
    const TelemetryData& data)
{
    /*
     * Phase A launch criterion:
     *
     * AGL altitude reaches the launch threshold.
     *
     * Current threshold = 10 m.
     */

    if (data.altitude >= LAUNCH_DETECTION_ALTITUDE_M)
    {
        Serial.println("Launch detected.");

        return true;
    }

    return false;
}


// ============================================================
// APOGEE DETECTION
// ============================================================

bool FlightStateManager::apogeeDetected(
    const TelemetryData& data)
{
    /*
     * Phase A:
     *
     * We do not directly detect the physical rocket
     * separation mechanism here.
     *
     * Instead, we detect apogee and the beginning
     * of the descent phase using:
     *
     * 1. Altitude begins decreasing.
     * 2. Multiple consecutive samples confirm it.
     * 3. Vertical velocity is downward.
     */


    // --------------------------------------------------------
    // First altitude sample
    // --------------------------------------------------------

    if (!previousAltitudeValid)
    {
        previousAltitude =
            data.altitude;

        previousAltitudeValid =
            true;

        return false;
    }


    // --------------------------------------------------------
    // Calculate altitude change
    // --------------------------------------------------------

    float altitudeChange =
        data.altitude - previousAltitude;

    previousAltitude =
        data.altitude;


    // --------------------------------------------------------
    // Check for downward movement
    // --------------------------------------------------------

    if (altitudeChange < 0.0f &&
        data.velocity < 0.0f)
    {
        separationConfirmationCount++;

        if (separationConfirmationCount >=
            SEPARATION_CONFIRMATION_SAMPLES)
        {
            separationConfirmationCount = 0;

            Serial.println(
                "Apogee detected. Descent beginning."
            );

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
// STABLE PRIMARY PARACHUTE DESCENT
// ============================================================

bool FlightStateManager::descentConfirmed(
    const TelemetryData& data)
{
    /*
     * Phase A:
     *
     * Confirm sustained downward velocity after
     * the apogee / descent transition.
     *
     * Velocity convention:
     *
     * Positive = upward
     * Negative = downward
     *
     * Current threshold:
     * velocity <= -2 m/s
     */

    if (data.velocity <=
        DESCENT_VELOCITY_THRESHOLD_MS)
    {
        descentConfirmationCount++;

        if (descentConfirmationCount >=
            DESCENT_CONFIRMATION_SAMPLES)
        {
            descentConfirmationCount = 0;

            Serial.println(
                "Stable primary parachute descent confirmed."
            );

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
// 600 m AGL PARAGLIDER TRIGGER
// ============================================================

bool FlightStateManager::paragliderAltitudeReached(
    const TelemetryData& data)
{
    if (data.altitude < -50.0f ||
        data.altitude > MAX_VALID_ALTITUDE_M)
    {
        return false;
    }

    // Only trigger while descending.
    if (data.velocity >= 0.0f)
    {
        return false;
    }

    return data.altitude <= PARAGLIDER_DEPLOYMENT_TRIGGER_M;

}


// ============================================================
// PHASE B PLACEHOLDER
// ============================================================




// ============================================================
// PHASE B PLACEHOLDER
// ============================================================

bool FlightStateManager::impactDetected(
    const TelemetryData& data)
{
    /*
     * Phase B / end-of-mission.
     *
     * Not implemented yet.
     */

    return false;
}