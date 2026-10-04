#include "storage.h"

#include <SPI.h>
#include <SD.h>


// ============================================================
// INITIALIZE SD CARD
// ============================================================

bool Storage::begin()
{
    // Raspberry Pi Pico SPI configuration

    SPI.setRX(16);
    SPI.setTX(19);
    SPI.setSCK(18);

    SPI.begin();


    // Initialize SD card

    if (!SD.begin(CS_PIN))
    {
        Serial.println("SD initialization failed!");
        return false;
    }

    Serial.println("SD initialized successfully.");


    // Create telemetry file if it does not exist

    if (!SD.exists(filename))
    {
        File file = SD.open(filename, FILE_WRITE);

        if (!file)
        {
            Serial.println("Failed to create telemetry.csv");
            return false;
        }


        // ----------------------------------------------------
        // CSV HEADER
        // Same order as telemetry packet
        // ----------------------------------------------------

        file.println(
            "TEAM_ID,"
            "TIME_STAMP,"
            "PACKET_COUNT,"
            "ALTITUDE,"
            "PRESSURE,"
            "TEMPERATURE,"
            "VOLTAGE,"
            "GNSS_TIME,"
            "GNSS_LATITUDE,"
            "GNSS_LONGITUDE,"
            "GNSS_ALTITUDE,"
            "GNSS_SATS,"
            "ACCEL_X,"
            "ACCEL_Y,"
            "ACCEL_Z,"
            "GYRO_SPIN_RATE,"
            "FLIGHT_SOFTWARE_STATE,"
            "HUMIDITY,"
            "VELOCITY,"
            "DISTANCE,"
            "ECO2,"
            "TVOC,"
            "AQI,"
            "IR_TEMPERATURE,"
            "SPECTRAL_F1,"
            "SPECTRAL_F2,"
            "SPECTRAL_F3,"
            "SPECTRAL_F4,"
            "SPECTRAL_F5,"
            "SPECTRAL_F6,"
            "SPECTRAL_F7,"
            "SPECTRAL_F8,"
            "SPECTRAL_CLEAR,"
            "SPECTRAL_NIR"
        );

        file.close();

        Serial.println("telemetry.csv created.");
    }
    else
    {
        Serial.println("telemetry.csv already exists.");
    }


    return true;
}


// ============================================================
// WRITE TELEMETRY
// ============================================================

bool Storage::writeTelemetry(
    const TelemetryData& data)
{
    File file = SD.open(filename, FILE_WRITE);

    if (!file)
    {
        Serial.println("Failed to open telemetry.csv");
        return false;
    }



    // REQUIRED COMPETITION TELEMETRY


    // 1. TEAM ID
    file.print(data.teamID);
    file.print(",");


    // 2. TIME STAMP
    file.print(data.timestamp);
    file.print(",");


    // 3. PACKET COUNT
    file.print(data.packetCount);
    file.print(",");


    // 4. ALTITUDE
    file.print(data.altitude, 1);
    file.print(",");


    // 5. PRESSURE
    file.print(data.pressure, 0);
    file.print(",");


    // 6. TEMPERATURE
    file.print(data.temperature, 1);
    file.print(",";


    // 7. VOLTAGE
    file.print(data.voltage, 2);
    file.print(",";


    // 8. GNSS TIME
    file.print(data.gnssTime);
    file.print(",";


    // 9. GNSS LATITUDE
    file.print(data.latitude, 4);
    file.print(",";


    // 10. GNSS LONGITUDE
    file.print(data.longitude, 4);
    file.print(",";


    // 11. GNSS ALTITUDE
    file.print(data.gnssAltitude, 1);
    file.print(",";


    // 12. GNSS SATELLITES
    file.print(data.gnssSats);
    file.print(",";


    // 13. ACCEL X
    file.print(data.accelX, 3);
    file.print(",";


    // 14. ACCEL Y
    file.print(data.accelY, 3);
    file.print(",";


    // 15. ACCEL Z
    file.print(data.accelZ, 3);
    file.print(",";


    // 16. GYRO SPIN RATE
    file.print(data.gyroSpinRate, 2);
    file.print(",";


    // 17. FLIGHT SOFTWARE STATE
    file.print(data.flightSoftwareState);
    file.print(",";



    // OPTIONAL TELEMETRY


    // 18. HUMIDITY
    file.print(data.humidity, 1);
    file.print(",";


    // 19. VERTICAL VELOCITY
    file.print(data.velocity, 2);
    file.print(",";


    // 20. DISTANCE TO TARGET
    file.print(data.distance, 2);
    file.print(",";


    // 21. eCO2
    file.print(data.eco2);
    file.print(",";


    // 22. TVOC
    file.print(data.tvoc);
    file.print(",";


    // 23. AQI
    file.print(data.aqi);
    file.print(",";


    // 24. IR TEMPERATURE
    file.print(data.irTemperature, 1);
    file.print(",";



    // AS7341


    file.print(data.spectralF1);
    file.print(",";

    file.print(data.spectralF2);
    file.print(",";

    file.print(data.spectralF3);
    file.print(",";

    file.print(data.spectralF4);
    file.print(",";

    file.print(data.spectralF5);
    file.print(",";

    file.print(data.spectralF6);
    file.print(",";

    file.print(data.spectralF7);
    file.print(",";

    file.print(data.spectralF8);
    file.print(",";

    file.print(data.spectralClear);
    file.print(",";


    // 34. NIR
    file.print(data.spectralNIR);



    // CSV RECORD TERMINATION


    file.print("\r\n");


    // Make sure data is physically written

    file.flush();
    file.close();

    return true;
}