#include "missionManager.h"
#include "config.h"

#include <string.h>

MissionManager::MissionManager()
{
    packetCount = 0;
    flightState = STATE_BOOT;

    recoveryAvailable = false;
    lastRecoverySave = 0;
}



// BEGIN


bool MissionManager::begin()
{
    Serial.println("Starting Mission Manager...");


    // Initialize recovery


    if (!recovery.begin())
    {
        Serial.println("Recovery initialization failed.");
        return false;
    }


    // Attempt recovery


    uint32_t recoveredTimestamp = 0;
    uint32_t recoveredPacketCount = 0;
    uint8_t recoveredFlightState = STATE_BOOT;

    if (recovery.load(
            recoveredTimestamp,
            recoveredPacketCount,
            recoveredFlightState))
    {
        recoveryAvailable = true;

        packetCount = recoveredPacketCount;
        flightState = recoveredFlightState;

        missionClock.begin(recoveredTimestamp);

        Serial.println("Mission data recovered.");

        Serial.print("Recovered timestamp: ");
        Serial.println(recoveredTimestamp);

        Serial.print("Recovered packet count: ");
        Serial.println(recoveredPacketCount);

        Serial.print("Recovered flight state: ");
        Serial.println(recoveredFlightState);
    }
    else
    {
        recoveryAvailable = false;

        packetCount = 0;
        flightState = STATE_BOOT;

        missionClock.begin(0);

        Serial.println("Starting new mission.");
    }

    lastRecoverySave = millis();

    return true;
}



// UPDATE


void MissionManager::update()
{
    missionClock.update();

    unsigned long now = millis();

    if (now - lastRecoverySave >=
        RECOVERY_SAVE_INTERVAL)
    {
        saveRecovery();

        lastRecoverySave = now;
    }
}



// GET MISSION TIME


uint32_t MissionManager::getMissionTime() const
{
    return missionClock.getTimestamp();
}



// GET PACKET COUNT


uint32_t MissionManager::getPacketCount() const
{
    return packetCount;
}



// GET FLIGHT STATE


uint8_t MissionManager::getFlightState() const
{
    return flightState;
}



// SET FLIGHT STATE


void MissionManager::setFlightState(uint8_t state)
{
    flightState = state;
}



// INCREMENT PACKET COUNT


void MissionManager::incrementPacketCount()
{
    packetCount++;
}



// SAVE RECOVERY


bool MissionManager::saveRecovery()
{
    return recovery.save(
        missionClock.getTimestamp(),
        packetCount,
        flightState
    );
}



// BUILD TELEMETRY


TelemetryData MissionManager::buildTelemetry(
    const TelemetryData& sensorData)
{
    TelemetryData telemetry = sensorData;


    // TEAM ID


    strncpy(
        telemetry.teamID,
        TEAM_ID,
        sizeof(telemetry.teamID) - 1
    );

    telemetry.teamID[
        sizeof(telemetry.teamID) - 1
    ] = '\0';


    // MISSION TIME


    telemetry.timestamp =
        getMissionTime();


    // PACKET COUNT


    telemetry.packetCount =
        getPacketCount();


    // FLIGHT SOFTWARE STATE


    switch (flightState)
    {
        case STATE_BOOT:
            strncpy(
                telemetry.flightSoftwareState,
                STATE_NAME_BOOT,
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;

        case STATE_TEST_MODE:
            strncpy(
                telemetry.flightSoftwareState,
                STATE_NAME_TEST_MODE,
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;

        case STATE_LAUNCH_PAD:
            strncpy(
                telemetry.flightSoftwareState,
                STATE_NAME_LAUNCH_PAD,
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;

        case STATE_ASCENT:
            strncpy(
                telemetry.flightSoftwareState,
                STATE_NAME_ASCENT,
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;

        case STATE_ROCKET_DEPLOY:
            strncpy(
                telemetry.flightSoftwareState,
                STATE_NAME_ROCKET_DEPLOY,
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;

        case STATE_DESCENT:
            strncpy(
                telemetry.flightSoftwareState,
                STATE_NAME_DESCENT,
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;

        case STATE_PARAGLIDER_DEPLOY:
            strncpy(
                telemetry.flightSoftwareState,
                STATE_NAME_PARAGLIDER_DEPLOY,
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;

        case STATE_PARAGLIDE_ACTIVE:
            strncpy(
                telemetry.flightSoftwareState,
                STATE_NAME_PARAGLIDE_ACTIVE,
                sizeof(telemetry.flightSoftwareState) - 1
            );
            break;

        case STATE_IMPACT:
            strncpy(
                telemetry.flightSoftwareState,
                STATE_NAME_IMPACT,
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