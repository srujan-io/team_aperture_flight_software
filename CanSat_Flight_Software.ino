#include "gnss.h"
#include "navigation.h"

GNSS gnss;
NavigationValidator navigation;

void setup()
{
    Serial.begin(115200);

    delay(2000);

    gnss.begin();
    navigation.begin();

    Serial.println();
    Serial.println("==============================");
    Serial.println("GNSS + NAVIGATION VALIDATOR");
    Serial.println("==============================");
}

void loop()
{
    gnss.update();
    navigation.update(gnss);

    static unsigned long lastPrint = 0;

    if (millis() - lastPrint >= 1000)
    {
        lastPrint = millis();

        Serial.println();
        Serial.println("========== NAVIGATION ==========");

        Serial.print("Fix: ");

        if (navigation.hasFix())
            Serial.println("VALID");
        else
            Serial.println("INVALID");

        Serial.print("Satellites: ");
        Serial.println(gnss.getSatellites());

        Serial.print("Satellite check: ");

        if (navigation.hasEnoughSatellites())
            Serial.println("PASS");
        else
            Serial.println("FAIL");

        Serial.print("Course: ");

        if (gnss.hasCourse())
        {
            Serial.print(gnss.getCourse(), 2);
            Serial.println(" deg");
        }
        else
        {
            Serial.println("INVALID");
        }

        Serial.print("Course check: ");

        if (navigation.hasValidCourse())
            Serial.println("PASS");
        else
            Serial.println("FAIL");

        Serial.println();

        Serial.print("NAVIGATION STATUS: ");

        if (navigation.isValid())
            Serial.println("VALID");
        else
            Serial.println("INVALID");

        Serial.println("================================");
    }
}