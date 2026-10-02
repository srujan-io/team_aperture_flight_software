#ifndef STABILITY_H
#define STABILITY_H

#include <Arduino.h>
#include "imu.h"

enum StabilityState
{
    STABILITY_STABLE = 0,
    STABILITY_UNSTABLE,
    STABILITY_RECOVERING
};

class StabilityController
{
public:
    StabilityController();

    void begin();
    void update(const IMUData& imuData);

    StabilityState getState() const;

    bool isStable() const;
    bool isUnstable() const;
    bool isRecovering() const;

    float getSteeringAuthority() const;
    float getFlywheelAuthority() const;

private:
    StabilityState state;

    unsigned long recoveryStartTime;

    float steeringAuthority;
    float flywheelAuthority;

    bool outsideLimits(const IMUData& imuData) const;
    bool insideRecoveryLimits(const IMUData& imuData) const;

    void updateState(const IMUData& imuData);
};

#endif