#include "missionClock.h"


MissionClock::MissionClock()
{
    missionTimestamp = 0;
    lastUpdate = 0;
}


// ====================================================
// BEGIN
// ====================================================

void MissionClock::begin(uint32_t recoveredTimestamp)
{
    missionTimestamp = recoveredTimestamp;

    lastUpdate = millis();
}


// ====================================================
// UPDATE
// ====================================================

void MissionClock::update()
{
    unsigned long now = millis();

    if (now - lastUpdate >= 1000)
    {
        uint32_t elapsed =
            (now - lastUpdate) / 1000;

        missionTimestamp += elapsed;

        lastUpdate += elapsed * 1000;
    }
}


// ====================================================
// GET TIMESTAMP
// ====================================================

uint32_t MissionClock::getTimestamp() const
{
    return missionTimestamp;
}


// ====================================================
// SET TIMESTAMP
// ====================================================

void MissionClock::setTimestamp(uint32_t timestamp)
{
    missionTimestamp = timestamp;

    lastUpdate = millis();
}
// ====================================================
// SET UTC TIME
// ====================================================

bool MissionClock::setUTC(const String& utc)
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    if (!parseUTC(
            utc,
            hour,
            minute,
            second))
    {
        Serial.println("Invalid UTC time.");
        return false;
    }

    missionTimestamp =
        ((uint32_t)hour * 3600UL) +
        ((uint32_t)minute * 60UL) +
        second;

    lastUpdate = millis();

    Serial.print("Mission clock set to UTC: ");
    Serial.println(utc);

    return true;
}


// ====================================================
// PARSE UTC
// ====================================================

bool MissionClock::parseUTC(
    const String& utc,
    uint8_t& hour,
    uint8_t& minute,
    uint8_t& second)
{
    if (utc.length() != 8)
        return false;

    if (utc.charAt(2) != ':' ||
        utc.charAt(5) != ':')
    {
        return false;
    }

    hour =
        utc.substring(0, 2).toInt();

    minute =
        utc.substring(3, 5).toInt();

    second =
        utc.substring(6, 8).toInt();

    if (hour > 23 ||
        minute > 59 ||
        second > 59)
    {
        return false;
    }

    return true;
}