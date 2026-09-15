#include "telemetry.h"


Telemetry::Telemetry()
{
}


// ====================================================
// CREATE TELEMETRY PACKET
// ====================================================

String Telemetry::createLoRaPacket(
    const TelemetryData& data)
{
    String packet;

    packet.reserve(512);


    // ------------------------------------------------
    // REQUIRED COMPETITION TELEMETRY
    // ------------------------------------------------

    // 1. TEAM ID
    packet += data.teamID;
    packet += ",";


    // 2. TIME STAMPING
    packet += String(data.timestamp);
    packet += ",";


    // 3. PACKET COUNT
    packet += String(data.packetCount);
    packet += ",";


    // 4. ALTITUDE
    packet += String(data.altitude, 1);
    packet += ",";


    // 5. PRESSURE
    packet += String(data.pressure, 0);
    packet += ",";


    // 6. TEMPERATURE
    packet += String(data.temperature, 1);
    packet += ",";


    // 7. VOLTAGE
    packet += String(data.voltage, 2);
    packet += ",";


    // 8. GNSS TIME
    packet += data.gnssTime;
    packet += ",";


    // 9. GNSS LATITUDE
    packet += String(data.latitude, 4);
    packet += ",";


    // 10. GNSS LONGITUDE
    packet += String(data.longitude, 4);
    packet += ",";


    // 11. GNSS ALTITUDE
    packet += String(data.gnssAltitude, 1);
    packet += ",";


    // 12. GNSS SATELLITES
    packet += String(data.gnssSats);
    packet += ",";


    // 13. ACCEL X
    packet += String(data.accelX, 3);
    packet += ",";


    // 14. ACCEL Y
    packet += String(data.accelY, 3);
    packet += ",";


    // 15. ACCEL Z
    packet += String(data.accelZ, 3);
    packet += ",";


    // 16. GYRO SPIN RATE
    packet += String(data.gyroSpinRate, 2);
    packet += ",";


    // 17. FLIGHT SOFTWARE STATE
    packet += data.flightSoftwareState;
    packet += ",";


    // ------------------------------------------------
    // OPTIONAL DATA
    // ------------------------------------------------

    // 18. HUMIDITY
    packet += String(data.humidity, 1);
    packet += ",";


    // 19. VELOCITY
    packet += String(data.velocity, 2);
    packet += ",";


    // 20. DISTANCE
    packet += String(data.distance, 2);
    packet += ",";


    // 21. eCO2
    packet += String(data.eco2);
    packet += ",";


    // 22. TVOC
    packet += String(data.tvoc);
    packet += ",";


    // 23. AQI
    packet += String(data.aqi);
    packet += ",";


    // 24. IR TEMPERATURE
    packet += String(data.irTemperature, 1);
    packet += ",";


    // 25-34. AS7341
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


    // ------------------------------------------------
    // COMPETITION REQUIRES CARRIAGE RETURN
    // ------------------------------------------------

    packet += "\r";


    return packet;
}