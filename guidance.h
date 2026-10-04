#ifndef GUIDANCE_H
#define GUIDANCE_H

#include <Arduino.h>

class GuidanceController
{
public:

    GuidanceController();

    void begin();

    void update(
        double currentLatitude,
        double currentLongitude,
        float currentCourse,
        bool courseValid
    );

    float getDistanceToTarget() const;
    float getBearingToTarget() const;
    float getHeadingError() const;

    bool isCourseValid() const;


private:

    double targetLatitude;
    double targetLongitude;

    float distanceToTarget;
    float bearingToTarget;
    float headingError;

    bool courseValid;



    // CALCULATIONS


    float calculateDistance(
        double lat1,
        double lon1,
        double lat2,
        double lon2
    );

    float calculateBearing(
        double lat1,
        double lon1,
        double lat2,
        double lon2
    );

    float calculateHeadingError(
        float targetBearing,
        float currentCourse
    );
};

#endif