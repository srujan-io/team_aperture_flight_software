#ifndef COMMANDS_H
#define COMMANDS_H

#include <Arduino.h>

class Commands
{
public:

    Commands();

    void begin();

    // Feed received command into the parser
    void processCommand(const String& command);

    // -------------------------
    // CX - TELEMETRY
    // -------------------------

    bool isTelemetryEnabled() const;

    // -------------------------
    // ST - TIME
    // -------------------------

    bool gpsTimeSyncRequested();
    bool utcTimeSetRequested();

    String getRequestedUTC() const;

    // -------------------------
    // CALIBRATION
    // -------------------------

    bool baroCalibrationRequested();
    bool imuCalibrationRequested();

    // -------------------------
    // SIMULATION
    // -------------------------

    bool isSimulationEnabled() const;

    // -------------------------
    // CLEAR REQUEST FLAGS
    // -------------------------

    void clearGPSRequest();
    void clearUTCRequest();
    void clearBaroRequest();
    void clearIMURequest();

private:

    bool telemetryEnabled;
    bool simulationEnabled;

    bool gpsSyncRequest;
    bool utcTimeRequest;

    bool baroCalibrationRequest;
    bool imuCalibrationRequest;

    String requestedUTC;

    void handleTelemetry(const String& argument);
    void handleTime(const String& argument);
    void handleCalibration(const String& argument);
    void handleSimulation(const String& argument);
};

#endif