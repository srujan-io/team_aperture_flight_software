#include "missionClock.h"


MissionClock::MissionClock()
{
    missionTimestamp = 0;
    lastUpdate = 0;
}


// BEGIN

void MissionClock::begin(uint32_t recoveredTimestamp)
{
    missionTimestamp = recoveredTimestamp;

    lastUpdate = millis();
}


// UPDATE

void MissionClock::update()
{
    unsigned long now = millis();

    unsigned long elapsedMs =
        now - lastUpdate;

    if (elapsedMs >= 1000UL)
    {
        uint32_t elapsedSeconds =
            elapsedMs / 1000UL;

        missionTimestamp += elapsedSeconds;

        lastUpdate +=
            elapsedSeconds * 1000UL;
    }
}


// GET TIMESTAMP

uint32_t MissionClock::getTimestamp() const
{
    return missionTimestamp;
}


// SET TIMESTAMP

void MissionClock::setTimestamp(
    uint32_t timestamp)
{
    missionTimestamp = timestamp;

    lastUpdate = millis();
}


// SET UTC TIME

bool MissionClock::setUTC(
    const String& utc)
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
        Serial.println(
            "Invalid UTC time."
        );

        return false;
    }

    missionTimestamp =
        ((uint32_t)hour * 3600UL) +
        ((uint32_t)minute * 60UL) +
        second;

    lastUpdate = millis();

    Serial.print(
        "Mission clock set to UTC: "
    );

    Serial.println(utc);

    return true;
}


// PARSE UTC

bool MissionClock::parseUTC(
    const String& utc,
    uint8_t& hour,
    uint8_t& minute,
    uint8_t& second)
{
    // Expected format:
    // HH:MM:SS

    if (utc.length() != 8)
    {
        return false;
    }

    if (utc.charAt(2) != ':' ||
        utc.charAt(5) != ':')
    {
        return false;
    }

    // Ensure all other characters are digits
    if (!isDigit(utc.charAt(0)) ||
        !isDigit(utc.charAt(1)) ||
        !isDigit(utc.charAt(3)) ||
        !isDigit(utc.charAt(4)) ||
        !isDigit(utc.charAt(6)) ||
        !isDigit(utc.charAt(7)))
    {
        return false;
    }

    hour =
        (uint8_t)utc.substring(0, 2).toInt();

    minute =
        (uint8_t)utc.substring(3, 5).toInt();

    second =
        (uint8_t)utc.substring(6, 8).toInt();

    if (hour > 23 ||
        minute > 59 ||
        second > 59)
    {
        return false;
    }

    return true;
}