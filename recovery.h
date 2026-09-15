#ifndef RECOVERY_H
#define RECOVERY_H

#include <Arduino.h>
#include <EEPROM.h>

class Recovery
{
public:

    Recovery();

    bool begin();

    bool save(
        uint32_t timestamp,
        uint32_t packetCount,
        uint8_t flightState
    );

    bool load(
        uint32_t &timestamp,
        uint32_t &packetCount,
        uint8_t &flightState
    );

private:

    struct RecoveryData
    {
        uint32_t magic;

        uint32_t timestamp;
        uint32_t packetCount;

        uint8_t flightState;

        uint32_t checksum;
    };

    static const uint32_t MAGIC = 0x43414E37; // "CAN7"

    uint32_t calculateChecksum(const RecoveryData &data);
};

#endif