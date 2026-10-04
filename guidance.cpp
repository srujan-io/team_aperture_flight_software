#include "guidance.h"
#include "config.h"

#include <math.h>



// EARTH


#define EARTH_RADIUS_M 6371000.0



// CONSTRUCTOR


GuidanceController::GuidanceController()
{
    targetLatitude =
        TARGET_LATITUDE;

    targetLongitude =
        TARGET_LONGITUDE;

    distanceToTarget = 0.0f;
    bearingToTarget = 0.0f;
    headingError = 0.0f;

    courseValid = false;
}



// BEGIN


void GuidanceController::begin()
{
    distanceToTarget = 0.0f;
    bearingToTarget = 0.0f;
    headingError = 0.0f;

    courseValid = false;
}



// UPDATE


void GuidanceController::update(
    double currentLatitude,
    double currentLongitude,
    float currentCourse,
    bool courseIsValid
)
{

    // Calculate distance to target


    distanceToTarget =
        calculateDistance(
            currentLatitude,
            currentLongitude,
            targetLatitude,
            targetLongitude
        );



    // Calculate bearing to target


    bearingToTarget =
        calculateBearing(
            currentLatitude,
            currentLongitude,
            targetLatitude,
            targetLongitude
        );



    // Validate course


    courseValid =
        courseIsValid;



    // Calculate heading error


    if (courseValid)
    {
        headingError =
            calculateHeadingError(
                bearingToTarget,
                currentCourse
            );
    }
    else
    {
        // Never provide a stale steering command
        // when course is invalid.

        headingError = 0.0f;
    }
}



// DISTANCE


float GuidanceController::calculateDistance(
    double lat1,
    double lon1,
    double lat2,
    double lon2
)
{
    const double lat1Rad =
        lat1 * DEG_TO_RAD;

    const double lat2Rad =
        lat2 * DEG_TO_RAD;

    const double deltaLat =
        (lat2 - lat1) * DEG_TO_RAD;

    const double deltaLon =
        (lon2 - lon1) * DEG_TO_RAD;


    // Haversine formula

    const double sinLat =
        sin(deltaLat / 2.0);

    const double sinLon =
        sin(deltaLon / 2.0);


    double a =
        sinLat * sinLat +

        cos(lat1Rad) *
        cos(lat2Rad) *
        sinLon * sinLon;


    // Protect against floating-point
    // rounding outside [0, 1].

    if (a < 0.0)
        a = 0.0;

    if (a > 1.0)
        a = 1.0;


    const double c =
        2.0 *
        atan2(
            sqrt(a),
            sqrt(1.0 - a)
        );


    return (float)(
        EARTH_RADIUS_M * c
    );
}



// BEARING


float GuidanceController::calculateBearing(
    double lat1,
    double lon1,
    double lat2,
    double lon2
)
{
    const double lat1Rad =
        lat1 * DEG_TO_RAD;

    const double lat2Rad =
        lat2 * DEG_TO_RAD;

    const double deltaLon =
        (lon2 - lon1) * DEG_TO_RAD;


    const double y =
        sin(deltaLon) *
        cos(lat2Rad);


    const double x =
        cos(lat1Rad) *
        sin(lat2Rad)

        -

        sin(lat1Rad) *
        cos(lat2Rad) *
        cos(deltaLon);


    double bearing =
        atan2(y, x) *
        RAD_TO_DEG;


    // Convert from [-180,180]
    // to [0,360)

    if (bearing < 0.0)
    {
        bearing += 360.0;
    }


    return (float)bearing;
}



// HEADING ERROR


float GuidanceController::calculateHeadingError(
    float targetBearing,
    float currentCourse
)
{
    float error =
        targetBearing - currentCourse;


    // Normalize to [-180,180]

    while (error > 180.0f)
    {
        error -= 360.0f;
    }


    while (error < -180.0f)
    {
        error += 360.0f;
    }


    return error;
}



// GET DISTANCE


float GuidanceController::getDistanceToTarget() const
{
    return distanceToTarget;
}



// GET TARGET BEARING


float GuidanceController::getBearingToTarget() const
{
    return bearingToTarget;
}



// GET HEADING ERROR


float GuidanceController::getHeadingError() const
{
    return headingError;
}



// COURSE VALIDITY


bool GuidanceController::isCourseValid() const
{
    return courseValid;
}