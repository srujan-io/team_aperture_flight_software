#include "gnss.h"
#include "config.h"


GNSS::GNSS()
{
    ready = false;

    startTime = 0;

    acquisitionTime = 0;
}


// BEGIN


void GNSS::begin()
{
    // L89HA GNSS UART
    //
    // Pico GP1 = RX
    // Pico GP0 = TX

    Serial1.setRX(1);
    Serial1.setTX(0);

    Serial1.begin(9600);

    // Start GNSS acquisition timer.
    startTime = millis();

    ready = false;
    acquisitionTime = 0;
}



// UPDATE

void GNSS::update()
{
    while (Serial1.available())
    {
        gps.encode(Serial1.read());
    }



    // GNSS READINESS


    if (!ready)
    {
        bool fixValid =
            gps.location.isValid() &&
            gps.location.age() < 2000;

        bool satellitesValid =
            gps.satellites.isValid() &&
            gps.satellites.value() >=
                GNSS_MIN_SATELLITES;


        if (fixValid && satellitesValid)
        {
            ready = true;

            acquisitionTime =
                millis() - startTime;

            Serial.println();
            Serial.println("==============================");
            Serial.println("GNSS ACQUISITION COMPLETE");
            Serial.print("Satellites: ");
            Serial.println(
                gps.satellites.value()
            );

            Serial.print("Acquisition time: ");
            Serial.print(acquisitionTime);
            Serial.println(" ms");

            Serial.println("==============================");
            Serial.println();
        }
    }
}



// GNSS FIX


bool GNSS::hasFix()
{
    return gps.location.isValid() &&
           gps.location.age() < 2000;
}



// GNSS READY


bool GNSS::isReady()
{
    return ready;
}



// ACQUISITION TIME


unsigned long GNSS::getAcquisitionTime()
{
    return acquisitionTime;
}



// POSITION


double GNSS::getLatitude()
{
    if (gps.location.isValid())
        return gps.location.lat();

    return 0.0;
}


double GNSS::getLongitude()
{
    if (gps.location.isValid())
        return gps.location.lng();

    return 0.0;
}


double GNSS::getAltitude()
{
    if (gps.altitude.isValid())
        return gps.altitude.meters();

    return 0.0;
}



// SATELLITES


uint8_t GNSS::getSatellites()
{
    if (gps.satellites.isValid())
        return gps.satellites.value();

    return 0;
}



// SPEED


float GNSS::getSpeed()
{
    if (gps.speed.isValid())
        return gps.speed.kmph();

    return 0.0f;
}



// COURSE


float GNSS::getCourse()
{
    if (gps.course.isValid())
        return gps.course.deg();

    return 0.0f;
}


bool GNSS::hasCourse()
{
    return gps.course.isValid() &&
           gps.course.age() <
               GNSS_MAX_COURSE_AGE_MS;
}



// TIME


void GNSS::getTime(
    char *buffer,
    size_t bufferSize
)
{
    if (buffer == nullptr ||
        bufferSize == 0)
    {
        return;
    }


    if (!gps.time.isValid())
    {
        strncpy(
            buffer,
            "NO_DATA",
            bufferSize
        );

        buffer[
            bufferSize - 1
        ] = '\0';

        return;
    }


    snprintf(
        buffer,
        bufferSize,
        "%02d:%02d:%02d",
        gps.time.hour(),
        gps.time.minute(),
        gps.time.second()
    );
}