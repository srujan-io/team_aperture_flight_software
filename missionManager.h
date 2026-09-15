#ifndef MISSION_MANAGER_H
#define MISSION_MANAGER_H

#include <Arduino.h>

#include "missionClock.h"
#include "recovery.h"
#include "telemetryData.h"

class MissionManager
{
public:

    MissionManager();

    bool begin();

    void update();

    uint32_t getMissionTime() const;
    uint32_t getPacketCount() const;
    uint8_t getFlightState() const;

    void incrementPacketCount();

    bool saveRecovery();

    TelemetryData buildTelemetry(
        const TelemetryData& sensorData
);

private:

    MissionClock missionClock;
    Recovery recovery;

    uint32_t packetCount;
    uint8_t flightState;

    bool recoveryAvailable;

    // Recovery save interval
    unsigned long lastRecoverySave;

    static const unsigned long
        RECOVERY_SAVE_INTERVAL = 1000;
};

#endif