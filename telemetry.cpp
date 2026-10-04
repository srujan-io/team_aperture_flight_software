#include "telemetry.h"


Telemetry::Telemetry()
{
}


// ============================================================
// CREATE TELEMETRY PACKET
// ============================================================

String Telemetry::createPacket(
    const TelemetryData& data)
{
    String packet;

    packet.reserve(512);


    // ========================================================
    // REQUIRED COMPETITION TELEMETRY
    // ========================================================

    // 1. TEAM ID
    packet += data.teamID;
    packet += ",";


    // 2. TIME STAMP
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


    // 13. ACCELEROMETER X
    packet += String(data.accelX, 3);
    packet += ",";


    // 14. ACCELEROMETER Y
    packet += String(data.accelY, 3);
    packet += ",";


    // 15. ACCELEROMETER Z
    packet += String(data.accelZ, 3);
    packet += ",";


    // 16. GYRO SPIN RATE
    packet += String(data.gyroSpinRate, 2);
    packet += ",";


    // 17. FLIGHT SOFTWARE STATE
    packet += data.flightSoftwareState;
    packet += ",";


    // ========================================================
    // OPTIONAL TELEMETRY
    // ========================================================

    // 18. HUMIDITY
    packet += String(data.humidity, 1);
    packet += ",";


    // 19. VERTICAL VELOCITY
    packet += String(data.velocity, 2);
    packet += ",";


    // 20. DISTANCE TO TARGET
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


    // ========================================================
    // AS7341 SPECTRAL DATA
    // ========================================================

    // 25. F1
    packet += String(data.spectralF1);
    packet += ",";

    // 26. F2
    packet += String(data.spectralF2);
    packet += ",";

    // 27. F3
    packet += String(data.spectralF3);
    packet += ",";

    // 28. F4
    packet += String(data.spectralF4);
    packet += ",";

    // 29. F5
    packet += String(data.spectralF5);
    packet += ",";

    // 30. F6
    packet += String(data.spectralF6);
    packet += ",";

    // 31. F7
    packet += String(data.spectralF7);
    packet += ",";

    // 32. F8
    packet += String(data.spectralF8);
    packet += ",";

    // 33. CLEAR
    packet += String(data.spectralClear);
    packet += ",";

    // 34. NIR
    packet += String(data.spectralNIR);


    // ========================================================
    // COMPETITION FORMAT
    // ========================================================

    // ASCII carriage return
    packet += "\r";


    return packet;
}