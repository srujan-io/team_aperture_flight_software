#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <Arduino.h>
#include "gnss.h"

class NavigationValidator
{
public:
    NavigationValidator();

    void begin();
    void update(const GNSS& gnss);

    bool isValid() const;

    bool hasFix() const;
    bool hasEnoughSatellites() const;
    bool hasValidCourse() const;

private:
    bool navigationValid;

    bool fixValid;
    bool satellitesValid;
    bool courseValid;
};

#endif