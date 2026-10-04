#include "recovery.h"


Recovery::Recovery()
{
}


// BEGIN

bool Recovery::begin()
{
    if (!EEPROM.begin(sizeof(RecoveryData)))
    {
        Serial.println(
            "EEPROM initialization failed."
        );

        return false;
    }

    Serial.println(
        "EEPROM initialized."
    );

    return true;
}


// CHECKSUM

uint32_t Recovery::calculateChecksum(
    const RecoveryData& data)
{
    const uint8_t* bytes =
        reinterpret_cast<const uint8_t*>(&data);

    uint32_t checksum = 0;


    for (size_t i = 0;
         i < sizeof(RecoveryData) - sizeof(uint32_t);
         i++)
    {
        checksum =
            (checksum * 31UL) + bytes[i];
    }

    return checksum;
}


// SAVE

bool Recovery::save(
    uint32_t timestamp,
    uint32_t packetCount,
    uint8_t flightState)
{
    RecoveryData data = {};

    data.magic = MAGIC;

    data.timestamp = timestamp;
    data.packetCount = packetCount;
    data.flightState = flightState;

    /*
     * IMPORTANT:
     * checksum must be zero before calculating it.
     */
    data.checksum = 0;

    data.checksum =
        calculateChecksum(data);

    EEPROM.put(0, data);

    if (!EEPROM.commit())
    {
        Serial.println(
            "EEPROM commit failed."
        );

        return false;
    }

    return true;
}


// LOAD

bool Recovery::load(
    uint32_t& timestamp,
    uint32_t& packetCount,
    uint8_t& flightState)
{
    RecoveryData data = {};

    EEPROM.get(0, data);


    // --------------------------------------------------------
    // Check magic number
    // --------------------------------------------------------

    if (data.magic != MAGIC)
    {
        Serial.println(
            "No valid recovery data."
        );

        return false;
    }


    // --------------------------------------------------------
    // Check checksum
    // --------------------------------------------------------

    uint32_t storedChecksum =
        data.checksum;

    data.checksum = 0;

    uint32_t calculatedChecksum =
        calculateChecksum(data);

    if (storedChecksum != calculatedChecksum)
    {
        Serial.println(
            "Recovery data checksum invalid."
        );

        return false;
    }


    // --------------------------------------------------------
    // Restore values
    // --------------------------------------------------------

    timestamp = data.timestamp;
    packetCount = data.packetCount;
    flightState = data.flightState;

    Serial.println(
        "Recovery data restored."
    );

    Serial.print("Timestamp: ");
    Serial.println(timestamp);

    Serial.print("Packet count: ");
    Serial.println(packetCount);

    Serial.print("Flight state: ");
    Serial.println(flightState);

    return true;
}