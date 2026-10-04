#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <Arduino.h>

#include "config.h"
#include "missionClock.h"
#include "calibrationManager.h"

class CommandManager
{
public:

    CommandManager();

    bool begin(
        MissionClock* clock,
        CalibrationManager* calibration
    );

    String processCommand(
        const String& command
    );

    bool telemetryEnabled() const;
    bool simulationEnabled() const;

private:

    MissionClock* missionClock;
    CalibrationManager* calibrationManager;

    bool telemetryOn;
    bool simulationOn;


    String makeAck(
        const String& type,
        const String& parameter,
        bool success
    );

    String getField(
        const String& command,
        uint8_t index
    );

    bool validateTeamID(
        const String& command
    );
};

#endif