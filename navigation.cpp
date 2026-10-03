#include "navigation.h"
#include "config.h"

NavigationValidator::NavigationValidator()
{
    navigationValid = false;

    fixValid = false;
    satellitesValid = false;
    courseValid = false;
}

void NavigationValidator::begin()
{
    navigationValid = false;

    fixValid = false;
    satellitesValid = false;
    courseValid = false;
}

void NavigationValidator::update(const GNSS& gnss)
{
    // --------------------------------
    // Position fix
    // --------------------------------

    fixValid = gnss.hasFix();


    // --------------------------------
    // Satellite count
    // --------------------------------

    satellitesValid =
        gnss.getSatellites() >= GNSS_MIN_SATELLITES;


    // --------------------------------
    // Course over ground
    // --------------------------------

    courseValid = gnss.hasCourse();


    // --------------------------------
    // Final navigation decision
    // --------------------------------

    navigationValid =
        fixValid &&
        satellitesValid &&
        courseValid;
}

bool NavigationValidator::isValid() const
{
    return navigationValid;
}

bool NavigationValidator::hasFix() const
{
    return fixValid;
}

bool NavigationValidator::hasEnoughSatellites() const
{
    return satellitesValid;
}

bool NavigationValidator::hasValidCourse() const
{
    return courseValid;
}