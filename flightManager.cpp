#include "flightManager.h"
#include "config.h"

#include <string.h>
#include <math.h>

FlightStateManager::FlightStateManager()
{
    currentState = BOOT;

    dropConfirmationCount = 0;
    descentConfirmationCount = 0;

    previousAltitude = 0.0f;
    previousAltitudeValid = false;
}

void FlightStateManager::begin()
{
    currentState = BOOT;

    dropConfirmationCount = 0;
    descentConfirmationCount = 0;

    previousAltitude = 0.0f;
    previousAltitudeValid = false;

    Serial.println("Flight State Manager initialized.");
    Serial.println("FLIGHT STATE: BOOT");
}

void FlightStateManager::update(
    const TelemetryData& data,
    DeploymentController& deployment
)
{
    switch (currentState)
    {
        // ----------------------------------------------------
        // BOOT
        // ----------------------------------------------------

        case BOOT:

            if (initializationComplete())
            {
                transitionTo(TEST_MODE);
            }

            break;


        // ----------------------------------------------------
        // TEST MODE
        // ----------------------------------------------------

        case TEST_MODE:

            if (testsPassed())
            {
                transitionTo(LAUNCH_PAD);
            }

            break;


        // ----------------------------------------------------
        // LAUNCH PAD
        // CANSat is still attached to drone
        // ----------------------------------------------------

        case LAUNCH_PAD:

            if (dropDetected(data))
            {
                transitionTo(DROP_DETECTED);
            }

            break;


        // ----------------------------------------------------
        // DROP DETECTED
        // Confirm that this is actually descent
        // ----------------------------------------------------

        case DROP_DETECTED:

            if (descentConfirmed(data))
            {
                transitionTo(DESCENT);
            }

            break;


        // ----------------------------------------------------
        // PRIMARY PARACHUTE DESCENT
        // ----------------------------------------------------

        case DESCENT:

            if (paragliderAltitudeReached(data))
            {
                if (deployment.startDeployment())
                {
                    transitionTo(PARAGLIDER_DEPLOY);
                }
                else
                {
                    Serial.println(
                        "ERROR: Paraglider deployment failed to start."
                    );
                }
            }

            break;


        // ----------------------------------------------------
        // PARAGLIDER DEPLOYMENT / INFLATION
        // ----------------------------------------------------

        case PARAGLIDER_DEPLOY:

        deployment.update();

        if (deployment.isInflationComplete())
        {
            transitionTo(PARAGLIDE_ACTIVE);
        }

        break;


        // ----------------------------------------------------
        // PARAGLIDER ACTIVE
        // Guidance + steering can operate here
        // ----------------------------------------------------

        case PARAGLIDE_ACTIVE:

            if (impactDetected(data))
            {
                transitionTo(IMPACT);
            }

            break;


        // ----------------------------------------------------
        // IMPACT
        // Mission finished
        // ----------------------------------------------------

        case IMPACT:

            break;
    }
}

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

        case DROP_DETECTED:
            return "DROP_DETECTED";

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

void FlightStateManager::updateTelemetryState(
    TelemetryData& data
)
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

void FlightStateManager::transitionTo(
    FlightState newState
)
{
    if (currentState == newState)
        return;

    currentState = newState;

    Serial.print("FLIGHT STATE -> ");
    Serial.println(getStateName());

    // --------------------------------------------------------
    // DROP DETECTED
    // --------------------------------------------------------

    if (newState == DROP_DETECTED)
    {
        dropConfirmationCount = 0;
        descentConfirmationCount = 0;

        previousAltitudeValid = false;

        Serial.println("Drone release detected.");
    }

    // --------------------------------------------------------
    // DESCENT
    // --------------------------------------------------------

    if (newState == DESCENT)
    {
        descentConfirmationCount = 0;

        Serial.println(
            "Primary parachute descent confirmed."
        );
    }

    // --------------------------------------------------------
    // PARAGLIDER DEPLOY
    // --------------------------------------------------------

    if (newState == PARAGLIDER_DEPLOY)
    {
        Serial.println(
            "Paraglider deployment initiated."
        );
    }

    // --------------------------------------------------------
    // PARAGLIDE ACTIVE
    // --------------------------------------------------------

    if (newState == PARAGLIDE_ACTIVE)
    {
        Serial.println(
            "Paraglider active. Guidance enabled."
        );
    }

    // --------------------------------------------------------
    // IMPACT
    // --------------------------------------------------------

    if (newState == IMPACT)
    {
        Serial.println(
            "Impact detected. Mission complete."
        );
    }
}

bool FlightStateManager::initializationComplete()
{
    /*
     * Temporary implementation.
     *
     * Later this should check the actual subsystem
     * initialization status.
     */

    return true;
}

bool FlightStateManager::testsPassed()
{
    /*
     * Temporary implementation.
     *
     * Later this will be connected to the actual
     * pre-flight / launch-pad checks.
     */

    return true;
}

bool FlightStateManager::dropDetected(
    const TelemetryData& data
)
{
    // Reject impossible altitude values
    if (data.altitude < -50.0f ||
        data.altitude > MAX_VALID_ALTITUDE_M)
    {
        return false;
    }

    // --------------------------------------------------------
    // First valid altitude sample
    // --------------------------------------------------------

    if (!previousAltitudeValid)
    {
        previousAltitude = data.altitude;
        previousAltitudeValid = true;

        return false;
    }

    // --------------------------------------------------------
    // Calculate altitude change
    // --------------------------------------------------------

    float altitudeChange =
        data.altitude - previousAltitude;

    previousAltitude = data.altitude;

    // --------------------------------------------------------
    // Drone release should result in:
    //
    // 1. Altitude decreasing
    // 2. Negative vertical velocity
    //
    // We require multiple consecutive samples to avoid
    // triggering because of one noisy BME280 reading.
    // --------------------------------------------------------

    bool altitudeDecreasing =
        altitudeChange < 0.0f;

    bool descending =
        data.velocity < DESCENT_VELOCITY_THRESHOLD_MS;

    if (altitudeDecreasing && descending)
    {
        dropConfirmationCount++;

        if (dropConfirmationCount >=
            DROP_CONFIRMATION_SAMPLES)
        {
            dropConfirmationCount = 0;

            Serial.println(
                "Drone release confirmed."
            );

            return true;
        }
    }
    else
    {
        dropConfirmationCount = 0;
    }

    return false;
}

bool FlightStateManager::descentConfirmed(
    const TelemetryData& data
)
{
    // --------------------------------------------------------
    // Confirm sustained downward velocity
    // --------------------------------------------------------

    if (data.velocity <= DESCENT_VELOCITY_THRESHOLD_MS)
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

bool FlightStateManager::paragliderAltitudeReached(
    const TelemetryData& data
)
{
    // Reject impossible altitude values
    if (data.altitude < -50.0f ||
        data.altitude > MAX_VALID_ALTITUDE_M)
    {
        return false;
    }

    // We should only deploy while descending
    if (data.velocity >= 0.0f)
    {
        return false;
    }

    // --------------------------------------------------------
    // Secondary mechanism activation
    //
    // Target altitude:
    // 600 m ± 10 m
    //
    // Current configuration uses:
    // 600 + 10 = 610 m
    // --------------------------------------------------------

    return data.altitude <=
           PARAGLIDER_DEPLOYMENT_TRIGGER_M;
}

bool FlightStateManager::impactDetected(
    const TelemetryData& data
)
{
    /*
     * TODO:
     *
     * We will implement proper impact detection using
     * altitude + acceleration + velocity + persistence.
     *
     * Do NOT use a random acceleration threshold here yet.
     */

    return false;
}