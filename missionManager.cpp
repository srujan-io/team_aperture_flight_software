#include "missionManager.h"


MissionManager::MissionManager()
{
    packetCount = 0;

    flightState = 0;

    recoveryAvailable = false;

    lastRecoverySave = 0;
}


// ====================================================
// BEGIN
// ====================================================

bool MissionManager::begin()
{
    Serial.println("Starting Mission Manager...");


    // ------------------------------------------------
    // Initialize EEPROM recovery
    // ------------------------------------------------

    if (!recovery.begin())
    {
        Serial.println(
            "Recovery initialization failed."
        );

        return false;
    }


    // ------------------------------------------------
    // Attempt recovery
    // ------------------------------------------------

    uint32_t recoveredTimestamp = 0;
    uint32_t recoveredPacketCount = 0;
    uint8_t recoveredFlightState = 0;


    if (recovery.load(
            recoveredTimestamp,
            recoveredPacketCount,
            recoveredFlightState))
    {
        // --------------------------------------------
        // Valid recovery data
        // --------------------------------------------

        recoveryAvailable = true;

        packetCount =
            recoveredPacketCount;

        flightState =
            recoveredFlightState;

        missionClock.begin(
            recoveredTimestamp
        );

        Serial.println(
            "Mission data recovered."
        );

        Serial.print(
            "Recovered timestamp: "
        );

        Serial.println(
            recoveredTimestamp
        );

        Serial.print(
            "Recovered packet count: "
        );

        Serial.println(
            recoveredPacketCount
        );

        Serial.print(
            "Recovered flight state: "
        );

        Serial.println(
            recoveredFlightState
        );
    }

    else
    {
        // --------------------------------------------
        // No valid recovery
        // --------------------------------------------

        recoveryAvailable = false;

        packetCount = 0;

        flightState = 0;

        missionClock.begin(0);

        Serial.println(
            "Starting new mission."
        );
    }


    lastRecoverySave = millis();

    return true;
}


// ====================================================
// UPDATE
// ====================================================

void MissionManager::update()
{
    // Update mission clock
    missionClock.update();


    unsigned long now = millis();


    // ------------------------------------------------
    // Periodic recovery save
    // ------------------------------------------------

    if (now - lastRecoverySave >=
        RECOVERY_SAVE_INTERVAL)
    {
        saveRecovery();

        lastRecoverySave = now;
    }
}


// ====================================================
// GET MISSION TIME
// ====================================================

uint32_t MissionManager::getMissionTime() const
{
    return missionClock.getTimestamp();
}


// ====================================================
// GET PACKET COUNT
// ====================================================

uint32_t MissionManager::getPacketCount() const
{
    return packetCount;
}


// ====================================================
// GET FLIGHT STATE
// ====================================================

uint8_t MissionManager::getFlightState() const
{
    return flightState;
}


// ====================================================
// INCREMENT PACKET COUNT
// ====================================================

void MissionManager::incrementPacketCount()
{
    packetCount++;
}


// ====================================================
// SAVE RECOVERY
// ====================================================

bool MissionManager::saveRecovery()
{
    return recovery.save(
        missionClock.getTimestamp(),
        packetCount,
        flightState
    );
}

TelemetryData MissionManager::buildTelemetry(
    const TelemetryData& sensorData)
{
    TelemetryData telemetry = sensorData;

    // -----------------------------------------------
    // TEAM ID
    // -----------------------------------------------

    strncpy(
        telemetry.teamID,
        "2026-IN-SPACeCAN-7USAT-024",
        sizeof(telemetry.teamID) - 1
    );

    telemetry.teamID[
        sizeof(telemetry.teamID) - 1
    ] = '\0';


    // -----------------------------------------------
    // MISSION TIME
    // -----------------------------------------------

    telemetry.timestamp =
        getMissionTime();


    // -----------------------------------------------
    // PACKET COUNT
    // -----------------------------------------------

    telemetry.packetCount =
        getPacketCount();


    // -----------------------------------------------
    // FLIGHT SOFTWARE STATE
    //
    // State transitions are intentionally NOT
    // implemented yet.
    // -----------------------------------------------

    switch (flightState)
    {
        case 0:
            strncpy(
                telemetry.flightSoftwareState,
                "BOOT",
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;

        default:
            strncpy(
                telemetry.flightSoftwareState,
                "UNKNOWN",
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;
    }

    telemetry.flightSoftwareState[
        sizeof(telemetry.flightSoftwareState) - 1
    ] = '\0';


    return telemetry;
}