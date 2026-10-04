#include "commandManager.h"


CommandManager::CommandManager()
{
    missionClock = nullptr;
    calibrationManager = nullptr;

    telemetryOn = false;
    simulationOn = false;
}


// BEGIN

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


// PROCESS COMMAND

String CommandManager::processCommand(
    const String& command)
{
    String cmd = command;

    cmd.trim();

    if (cmd.length() == 0)
        return "";



    // COMMAND FORMAT


    // Expected:
    //
    // CMD,TEAM_ID,TYPE,PARAMETER
    //
    // Example:
    //
    // CMD,CAN-7USAT-024,CX,ON


    if (!cmd.startsWith("CMD,"))
        return "NACK,INVALID_FORMAT\r";



    // TEAM ID VALIDATION


    if (!validateTeamID(cmd))
        return "NACK,INVALID_TEAM_ID\r";


    String type =
        getField(cmd, 2);

    String parameter =
        getField(cmd, 3);



    // CX
    // TELEMETRY CONTROL


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



    // ST
    // TIME SYNCHRONIZATION


    if (type == "ST")
    {
        if (missionClock == nullptr)
        {
            return makeAck(
                type,
                parameter,
                false
            );
        }



        // GPS TIME SYNCHRONIZATION


        if (parameter == "GPS")
        {
            // GNSS object is not currently connected
            // to CommandManager.
            //
            // This will be connected during final
            // integration once the GNSS interface is ready.

            return makeAck(
                type,
                parameter,
                true
            );
        }



        // EXPLICIT UTC TIME
        //
        // CMD,TEAM_ID,ST,UTC_TIME,12:46:55


        if (parameter == "UTC_TIME")
        {
            String utc =
                getField(cmd, 4);

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



        // DIRECT UTC FORMAT
        //
        // CMD,TEAM_ID,ST,12:46:55


        uint8_t hour = 0;
        uint8_t minute = 0;
        uint8_t second = 0;

        if (missionClock->parseUTC(
                parameter,
                hour,
                minute,
                second))
        {
            if (missionClock->setUTC(parameter))
            {
                return makeAck(
                    type,
                    parameter,
                    true
                );
            }
        }


        return makeAck(
            type,
            parameter,
            false
        );
    }



    // CAL
    // SENSOR CALIBRATION


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



        // IMU


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



        // BAROMETER


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



    // SIM
    // SIMULATION MODE


    if (type == "SIM")
    {

        // ENABLE


        if (parameter == "ENABLE")
        {
            simulationOn = true;

            return makeAck(
                type,
                parameter,
                true
            );
        }



        // DISABLE


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



    // UNKNOWN COMMAND


    return "NACK,UNKNOWN_COMMAND\r";
}


// TELEMETRY STATUS

bool CommandManager::telemetryEnabled() const
{
    return telemetryOn;
}


// SIMULATION STATUS

bool CommandManager::simulationEnabled() const
{
    return simulationOn;
}


// CREATE ACKNOWLEDGEMENT

String CommandManager::makeAck(
    const String& type,
    const String& parameter,
    bool success)
{
    String response;

    response.reserve(96);

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


// TEAM ID VALIDATION

bool CommandManager::validateTeamID(
    const String& command)
{
    String receivedTeamID =
        getField(command, 1);

    return receivedTeamID == TEAM_ID;
}


// CSV FIELD EXTRACTION

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