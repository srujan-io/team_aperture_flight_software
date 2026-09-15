#include "gnss.h"

GNSS gnss;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println();
    Serial.println("================================");
    Serial.println("          GNSS TEST");
    Serial.println("================================");

    gnss.begin();

    Serial.println("GNSS initialized.");
}

void loop()
{
    // Read GNSS data
    gnss.update();

    static unsigned long lastPrint = 0;

    if (millis() - lastPrint >= 1000)
    {
        lastPrint = millis();

        Serial.println();
        Serial.println("---------- GNSS ----------");

        Serial.print("Fix: ");
        Serial.println(gnss.hasFix() ? "YES" : "NO");

        Serial.print("Satellites: ");
        Serial.println(gnss.getSatellites());

        Serial.print("Latitude: ");
        Serial.println(gnss.getLatitude(), 6);

        Serial.print("Longitude: ");
        Serial.println(gnss.getLongitude(), 6);

        Serial.print("Altitude: ");
        Serial.print(gnss.getAltitude(), 2);
        Serial.println(" m");

        char timeBuffer[16];
        gnss.getTime(timeBuffer, sizeof(timeBuffer));

        Serial.print("UTC Time: ");
        Serial.println(timeBuffer);

        Serial.println("--------------------------");
    }
}