#include "guidance.h"
#include "config.h"

#include <math.h>

#define EARTH_RADIUS_M 6371000.0

GuidanceController::GuidanceController()
{
    targetLatitude = TARGET_LATITUDE;
    targetLongitude = TARGET_LONGITUDE;

    distanceToTarget = 0.0f;
    bearingToTarget = 0.0f;
    headingError = 0.0f;

    courseValid = false;
}

void GuidanceController::begin()
{
    distanceToTarget = 0.0f;
    bearingToTarget = 0.0f;
    headingError = 0.0f;

    courseValid = false;
}

void GuidanceController::update(
    double currentLatitude,
    double currentLongitude,
    float currentCourse,
    bool courseIsValid
)
{
    distanceToTarget =
        calculateDistance(
            currentLatitude,
            currentLongitude,
            targetLatitude,
            targetLongitude
        );

    bearingToTarget =
        calculateBearing(
            currentLatitude,
            currentLongitude,
            targetLatitude,
            targetLongitude
        );

    courseValid = courseIsValid;

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
        headingError = 0.0f;
    }
}

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

    const double a =
        sin(deltaLat / 2.0) *
        sin(deltaLat / 2.0) +

        cos(lat1Rad) *
        cos(lat2Rad) *
        sin(deltaLon / 2.0) *
        sin(deltaLon / 2.0);

    const double c =
        2.0 *
        atan2(
            sqrt(a),
            sqrt(1.0 - a)
        );

    return (float)(EARTH_RADIUS_M * c);
}

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

    if (bearing < 0.0)
        bearing += 360.0;

    return (float)bearing;
}

float GuidanceController::calculateHeadingError(
    float targetBearing,
    float currentCourse
)
{
    float error =
        targetBearing - currentCourse;

    // Normalize to [-180, +180]
    while (error > 180.0f)
        error -= 360.0f;

    while (error < -180.0f)
        error += 360.0f;

    return error;
}

float GuidanceController::getDistanceToTarget() const
{
    return distanceToTarget;
}

float GuidanceController::getBearingToTarget() const
{
    return bearingToTarget;
}

float GuidanceController::getHeadingError() const
{
    return headingError;
}