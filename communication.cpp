#include "communication.h"
#include "config.h"

#include <SPI.h>

#define LORA_CS       17
#define LORA_DIO1     20
#define LORA_RST      21
#define LORA_BUSY     22

#define LORA_MISO     16
#define LORA_SCK      18
#define LORA_MOSI     19

#define LORA_FREQUENCY        866.5
#define LORA_BANDWIDTH        125.0
#define LORA_SPREADING_FACTOR 9
#define LORA_CODING_RATE      5
#define LORA_SYNC_WORD        0x97
#define LORA_TX_POWER         10
#define LORA_PREAMBLE         8

Communication::Communication()
    : radio(new Module(
        LORA_CS,
        LORA_DIO1,
        LORA_RST,
        LORA_BUSY))
{
    initialized = false;

    lastRSSI = 0.0f;
    lastSNR = 0.0f;
}

bool Communication::begin()
{
    if (initialized)
        return true;

    // -------------------------
    // Configure Pico SPI0
    // -------------------------

    SPI.setRX(LORA_MISO);
    SPI.setCS(LORA_CS);
    SPI.setSCK(LORA_SCK);
    SPI.setTX(LORA_MOSI);

    SPI.begin();

    // -------------------------
    // Initialize SX1262
    // -------------------------

    if (!initializeRadio())
    {
        initialized = false;
        return false;
    }

    initialized = true;

    return true;
}

bool Communication::initializeRadio()
{
    int state = radio.begin(
        LORA_FREQUENCY,
        LORA_BANDWIDTH,
        LORA_SPREADING_FACTOR,
        LORA_CODING_RATE,
        LORA_SYNC_WORD,
        LORA_TX_POWER,
        LORA_PREAMBLE
    );

    if (state != RADIOLIB_ERR_NONE)
    {
        Serial.print("SX1262 initialization failed. Error: ");
        Serial.println(state);

        return false;
    }

    Serial.println("SX1262 initialized successfully.");

    return true;
}

bool Communication::sendTelemetry(const TelemetryData& data)
{
    if (!initialized)
        return false;

    String packet =
        buildTelemetryPacket(data);

    int state =
        radio.transmit(packet);

    if (state != RADIOLIB_ERR_NONE)
    {
        Serial.print("LoRa transmission failed. Error: ");
        Serial.println(state);

        return false;
    }

    return true;
}

bool Communication::receiveCommand(String& command)
{
    if (!initialized)
        return false;

    String receivedData;

    int state =
        radio.receive(receivedData);

    if (state != RADIOLIB_ERR_NONE)
    {
        return false;
    }

    lastRSSI = radio.getRSSI();
    lastSNR = radio.getSNR();

    command = receivedData;

    return true;
}

String Communication::buildTelemetryPacket(
    const TelemetryData& data)
{
    String packet;

    packet += data.teamID;
    packet += ",";

    packet += String(data.timestamp);
    packet += ",";

    packet += String(data.packetCount);
    packet += ",";

    packet += String(data.altitude, 2);
    packet += ",";

    packet += String(data.pressure, 2);
    packet += ",";

    packet += String(data.temperature, 2);
    packet += ",";

    packet += String(data.voltage, 2);
    packet += ",";

    packet += data.gnssTime;
    packet += ",";

    packet += String(data.latitude, 6);
    packet += ",";

    packet += String(data.longitude, 6);
    packet += ",";

    packet += String(data.gnssAltitude, 2);
    packet += ",";

    packet += String(data.gnssSats);
    packet += ",";

    packet += String(data.accelX, 2);
    packet += ",";

    packet += String(data.accelY, 2);
    packet += ",";

    packet += String(data.accelZ, 2);
    packet += ",";

    packet += String(data.gyroSpinRate, 2);
    packet += ",";

    packet += data.flightSoftwareState;
    packet += ",";

    packet += String(data.humidity, 2);
    packet += ",";

    packet += String(data.velocity, 2);
    packet += ",";

    packet += String(data.distance, 2);
    packet += ",";

    packet += String(data.eco2);
    packet += ",";

    packet += String(data.tvoc);
    packet += ",";

    packet += String(data.aqi);
    packet += ",";

    packet += String(data.irTemperature, 2);
    packet += ",";

    packet += String(data.spectralF1);
    packet += ",";

    packet += String(data.spectralF2);
    packet += ",";

    packet += String(data.spectralF3);
    packet += ",";

    packet += String(data.spectralF4);
    packet += ",";

    packet += String(data.spectralF5);
    packet += ",";

    packet += String(data.spectralF6);
    packet += ",";

    packet += String(data.spectralF7);
    packet += ",";

    packet += String(data.spectralF8);
    packet += ",";

    packet += String(data.spectralClear);
    packet += ",";

    packet += String(data.spectralNIR);

    return packet;
}

bool Communication::isInitialized() const
{
    return initialized;
}

float Communication::getLastRSSI() const
{
    return lastRSSI;
}

float Communication::getLastSNR() const
{
    return lastSNR;
}