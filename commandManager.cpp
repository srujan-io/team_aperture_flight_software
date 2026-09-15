#include "commandManager.h"


CommandManager::CommandManager()
{
    missionClock = nullptr;
    calibrationManager = nullptr;

    telemetryOn = false;
    simulationOn = false;
}


// ====================================================
// BEGIN
// ====================================================

bool CommandManager::begin(
    MissionClock* clock,
    CalibrationManager* calibration)
{
    missionClock = clock;
    calibrationManager = calibration;

    telemetryOn = false;
    simulationOn = false;

    Serial.println("Command Manager initialized.");

    return true;
}


// ====================================================
// PROCESS COMMAND
// ====================================================

String CommandManager::processCommand(
    const String& command)
{
    String cmd = command;

    cmd.trim();

    if (cmd.length() == 0)
        return "";

    // ------------------------------------------------
    // Must begin with CMD
    // ------------------------------------------------

    if (!cmd.startsWith("CMD,"))
        return "NACK,INVALID_FORMAT\r";


    // ------------------------------------------------
    // Check team ID
    // ------------------------------------------------

    if (!validateTeamID(cmd))
        return "NACK,INVALID_TEAM_ID\r";


    String type =
        getField(cmd, 2);


    String parameter =
        getField(cmd, 3);


    // =================================================
    // CX
    // =================================================

    if (type == "CX")
    {
        if (parameter == "ON")
        {
            telemetryOn = true;

            return makeAck(
                type,
                parameter,
                true
            );
        }


        if (parameter == "OFF")
        {
            telemetryOn = false;

            return makeAck(
                type,
                parameter,
                true
            );
        }


        return makeAck(
            type,
            parameter,
            false
        );
    }


    // =================================================
    // ST
    // =================================================

    if (type == "ST")
    {
        // ---------------------------------------------
        // GPS synchronization
        // ---------------------------------------------

        if (parameter == "GPS")
        {
            // GNSS integration will be connected
            // once CommandManager receives the GNSS object.

            return makeAck(
                type,
                parameter,
                true
            );
        }


        // ---------------------------------------------
        // Manual UTC time
        //
        // CMD,TEAM_ID,ST,UTC_TIME,12:46:55
        // ---------------------------------------------

        if (parameter == "UTC_TIME")
        {
            String utc =
                getField(cmd, 4);

            if (missionClock == nullptr)
            {
                return makeAck(
                    type,
                    parameter,
                    false
                );
            }


            if (missionClock->setUTC(utc))
            {
                return makeAck(
                    type,
                    parameter,
                    true
                );
            }


            return makeAck(
                type,
                parameter,
                false
            );
        }


        // Also allow:
        //
        // CMD,TEAM_ID,ST,12:46:55
        //
        if (missionClock != nullptr &&
            missionClock->parseUTC(
                parameter,
                *(new uint8_t),
                *(new uint8_t),
                *(new uint8_t)))
        {
            missionClock->setUTC(parameter);

            return makeAck(
                type,
                parameter,
                true
            );
        }


        return makeAck(
            type,
            parameter,
            false
        );
    }


    // =================================================
    // CAL
    // =================================================

    if (type == "CAL")
    {
        if (calibrationManager == nullptr)
        {
            return makeAck(
                type,
                parameter,
                false
            );
        }


        // ---------------------------------------------
        // IMU calibration
        // ---------------------------------------------

        if (parameter == "IMU")
        {
            bool success =
                calibrationManager->calibrateIMU();

            return makeAck(
                type,
                parameter,
                success
            );
        }


        // ---------------------------------------------
        // BAROMETER calibration
        // ---------------------------------------------

        if (parameter == "BARO")
        {
            bool success =
                calibrationManager->calibrateBarometer();

            return makeAck(
                type,
                parameter,
                success
            );
        }


        return makeAck(
            type,
            parameter,
            false
        );
    }


    // =================================================
    // SIM
    // =================================================

    if (type == "SIM")
    {
        // ---------------------------------------------
        // ENABLE
        // ---------------------------------------------

        if (parameter == "ENABLE")
        {
            simulationOn = true;

            return makeAck(
                type,
                parameter,
                true
            );
        }


        // ---------------------------------------------
        // DISABLE
        // ---------------------------------------------

        if (parameter == "DISABLE")
        {
            simulationOn = false;

            return makeAck(
                type,
                parameter,
                true
            );
        }


        return makeAck(
            type,
            parameter,
            false
        );
    }


    // =================================================
    // UNKNOWN COMMAND
    // =================================================

    return "NACK,UNKNOWN_COMMAND\r";
}


// ====================================================
// TELEMETRY STATUS
// ====================================================

bool CommandManager::telemetryEnabled() const
{
    return telemetryOn;
}


// ====================================================
// SIMULATION STATUS
// ====================================================

bool CommandManager::simulationEnabled() const
{
    return simulationOn;
}


// ====================================================
// ACK
// ====================================================

String CommandManager::makeAck(
    const String& type,
    const String& parameter,
    bool success)
{
    String response;

    response += "ACK,";
    response += TEAM_ID;
    response += ",";
    response += type;
    response += ",";
    response += parameter;
    response += ",";

    if (success)
        response += "OK";
    else
        response += "FAIL";

    response += "\r";

    return response;
}


// ====================================================
// TEAM ID VALIDATION
// ====================================================

bool CommandManager::validateTeamID(
    const String& command)
{
    String receivedTeamID =
        getField(command, 1);

    return receivedTeamID == TEAM_ID;
}


// ====================================================
// CSV FIELD EXTRACTION
// ====================================================

String CommandManager::getField(
    const String& command,
    uint8_t index)
{
    uint8_t currentField = 0;

    int start = 0;

    for (int i = 0;
         i <= command.length();
         i++)
    {
        if (command.charAt(i) == ',' ||
            i == command.length())
        {
            if (currentField == index)
            {
                return command.substring(
                    start,
                    i
                );
            }

            currentField++;

            start = i + 1;
        }
    }

    return "";
}