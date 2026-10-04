#ifndef MISSION_CLOCK_H
#define MISSION_CLOCK_H

#include <Arduino.h>

class MissionClock
{
public:

    MissionClock();

    void begin(uint32_t recoveredTimestamp = 0);

    void update();

    uint32_t getTimestamp() const;

    // Reset / recovery support
    void setTimestamp(uint32_t timestamp);

    // Set clock from GNSS UTC time
    bool setUTC(const String& utc);

    // Parse HH:MM:SS
    bool parseUTC(
        const String& utc,
        uint8_t& hour,
        uint8_t& minute,
        uint8_t& second
    );

private:

    uint32_t missionTimestamp;

    unsigned long lastUpdate;
};

#endif