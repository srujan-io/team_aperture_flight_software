#include "commands.h"

static const char* TEAM_ID =
    "2026-IN-SPACeCAN-7USAT-024";


Commands::Commands()
{
    telemetryEnabled = false;
    simulationEnabled = false;

    gpsSyncRequest = false;
    utcTimeRequest = false;

    baroCalibrationRequest = false;
    imuCalibrationRequest = false;

    requestedUTC = "";
}


void Commands::begin()
{
    telemetryEnabled = false;
    simulationEnabled = false;

    gpsSyncRequest = false;
    utcTimeRequest = false;

    baroCalibrationRequest = false;
    imuCalibrationRequest = false;

    requestedUTC = "";

    Serial.println("Command system initialized.");
    Serial.println("Telemetry = OFF");
    Serial.println("Simulation = OFF");
}


// ====================================================
// COMMAND PARSER
// ====================================================

void Commands::processCommand(const String& command)
{
    String prefix = "CMD,";
    prefix += TEAM_ID;
    prefix += ",";

    // -----------------------------------------------
    // Check command prefix / team ID
    // -----------------------------------------------

    if (!command.startsWith(prefix))
    {
        Serial.println("Invalid command / TEAM ID.");
        return;
    }

    // Remove:
    // CMD,2026-IN-SPACeCAN-7USAT-024,

    String payload = command.substring(prefix.length());


    // =================================================
    // CX
    // =================================================

    if (payload.startsWith("CX,"))
    {
        String argument = payload.substring(3);

        handleTelemetry(argument);
        return;
    }


    // =================================================
    // ST
    // =================================================

    if (payload.startsWith("ST,"))
    {
        String argument = payload.substring(3);

        handleTime(argument);
        return;
    }


    // =================================================
    // CAL
    // =================================================

    if (payload.startsWith("CAL,"))
    {
        String argument = payload.substring(4);

        handleCalibration(argument);
        return;
    }


    // =================================================
    // SIM
    // =================================================

    if (payload.startsWith("SIM,"))
    {
        String argument = payload.substring(4);

        handleSimulation(argument);
        return;
    }


    Serial.println("Unknown command.");
}


// ====================================================
// CX - TELEMETRY CONTROL
// ====================================================

void Commands::handleTelemetry(const String& argument)
{
    if (argument == "ON")
    {
        telemetryEnabled = true;

        Serial.println("Telemetry transmission ENABLED.");

        return;
    }


    if (argument == "OFF")
    {
        telemetryEnabled = false;

        Serial.println("Telemetry transmission DISABLED.");
        Serial.println("Sensor acquisition and SD logging continue.");

        return;
    }


    Serial.println("Invalid CX command.");
}


// ====================================================
// ST - TIME SYNCHRONIZATION
// ====================================================

void Commands::handleTime(const String& argument)
{
    // -----------------------------------------------
    // ST,GPS
    // -----------------------------------------------

    if (argument == "GPS")
    {
        gpsSyncRequest = true;

        Serial.println("GNSS time synchronization requested.");

        return;
    }


    // -----------------------------------------------
    // ST,HH:MM:SS
    // -----------------------------------------------

    if (argument.length() == 8 &&
        argument.charAt(2) == ':' &&
        argument.charAt(5) == ':')
    {
        requestedUTC = argument;

        utcTimeRequest = true;

        Serial.print("UTC time requested: ");
        Serial.println(requestedUTC);

        return;
    }


    Serial.println("Invalid ST command.");
}


// ====================================================
// CAL - SENSOR CALIBRATION
// ====================================================

void Commands::handleCalibration(const String& argument)
{
    // -----------------------------------------------
    // CAL,BARO
    // -----------------------------------------------

    if (argument == "BARO")
    {
        baroCalibrationRequest = true;

        Serial.println("Barometric calibration requested.");

        return;
    }


    // -----------------------------------------------
    // CAL,IMU
    // -----------------------------------------------

    if (argument == "IMU")
    {
        imuCalibrationRequest = true;

        Serial.println("IMU calibration requested.");

        return;
    }


    Serial.println("Invalid CAL command.");
}


// ====================================================
// SIM - SIMULATION MODE
// ====================================================

void Commands::handleSimulation(const String& argument)
{
    // -----------------------------------------------
    // SIM,ENABLE
    // -----------------------------------------------

    if (argument == "ENABLE")
    {
        simulationEnabled = true;

        Serial.println("Simulation mode ENABLED.");

        return;
    }


    // -----------------------------------------------
    // SIM,DISABLE
    // -----------------------------------------------

    if (argument == "DISABLE")
    {
        simulationEnabled = false;

        Serial.println("Simulation mode DISABLED.");

        return;
    }


    Serial.println("Invalid SIM command.");
}


// ====================================================
// STATUS
// ====================================================

bool Commands::isTelemetryEnabled() const
{
    return telemetryEnabled;
}


bool Commands::isSimulationEnabled() const
{
    return simulationEnabled;
}


// ====================================================
// REQUEST STATUS
// ====================================================

bool Commands::gpsTimeSyncRequested()
{
    return gpsSyncRequest;
}


bool Commands::utcTimeSetRequested()
{
    return utcTimeRequest;
}


String Commands::getRequestedUTC() const
{
    return requestedUTC;
}


bool Commands::baroCalibrationRequested()
{
    return baroCalibrationRequest;
}


bool Commands::imuCalibrationRequested()
{
    return imuCalibrationRequest;
}


// ====================================================
// CLEAR REQUEST FLAGS
// ====================================================

void Commands::clearGPSRequest()
{
    gpsSyncRequest = false;
}


void Commands::clearUTCRequest()
{
    utcTimeRequest = false;
}


void Commands::clearBaroRequest()
{
    baroCalibrationRequest = false;
}


void Commands::clearIMURequest()
{
    imuCalibrationRequest = false;
}