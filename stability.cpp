#include "stability.h"
#include "config.h"

#include <math.h>

StabilityController::StabilityController()
{
    state = STABILITY_STABLE;

    recoveryStartTime = 0;

    steeringAuthority = 1.0f;
    flywheelAuthority = 1.0f;
}

void StabilityController::begin()
{
    state = STABILITY_STABLE;

    recoveryStartTime = 0;

    steeringAuthority = 1.0f;
    flywheelAuthority = 1.0f;
}

void StabilityController::update(const IMUData& imuData)
{
    updateState(imuData);

    // Flywheel remains available in all states.
    flywheelAuthority = 1.0f;

    // Steering is only allowed when stable.
    if (state == STABILITY_STABLE)
    {
        steeringAuthority = 1.0f;
    }
    else
    {
        steeringAuthority = 0.0f;
    }
}

void StabilityController::updateState(const IMUData& imuData)
{
    if (outsideLimits(imuData))
    {
        state = STABILITY_UNSTABLE;
        recoveryStartTime = 0;

        return;
    }

    if (state == STABILITY_UNSTABLE)
    {
        if (insideRecoveryLimits(imuData))
        {
            state = STABILITY_RECOVERING;
            recoveryStartTime = millis();
        }

        return;
    }

    if (state == STABILITY_RECOVERING)
    {
        if (!insideRecoveryLimits(imuData))
        {
            state = STABILITY_UNSTABLE;
            recoveryStartTime = 0;

            return;
        }

        if (millis() - recoveryStartTime >=
            STABILITY_RECOVERY_TIME_MS)
        {
            state = STABILITY_STABLE;
            recoveryStartTime = 0;
        }

        return;
    }
}

bool StabilityController::outsideLimits(
    const IMUData& imuData) const
{
    if (fabs(imuData.roll) >
        STABILITY_ROLL_LIMIT_DEG)
    {
        return true;
    }

    if (fabs(imuData.pitch) >
        STABILITY_PITCH_LIMIT_DEG)
    {
        return true;
    }

    if (fabs(imuData.gz) >
        STABILITY_YAW_RATE_LIMIT_DPS)
    {
        return true;
    }

    return false;
}

bool StabilityController::insideRecoveryLimits(
    const IMUData& imuData) const
{
    if (fabs(imuData.roll) >
        STABILITY_ROLL_RECOVERY_DEG)
    {
        return false;
    }

    if (fabs(imuData.pitch) >
        STABILITY_PITCH_RECOVERY_DEG)
    {
        return false;
    }

    if (fabs(imuData.gz) >
        STABILITY_YAW_RECOVERY_DPS)
    {
        return false;
    }

    return true;
}

StabilityState StabilityController::getState() const
{
    return state;
}

bool StabilityController::isStable() const
{
    return state == STABILITY_STABLE;
}

bool StabilityController::isUnstable() const
{
    return state == STABILITY_UNSTABLE;
}

bool StabilityController::isRecovering() const
{
    return state == STABILITY_RECOVERING;
}

float StabilityController::getSteeringAuthority() const
{
    return steeringAuthority;
}

float StabilityController::getFlywheelAuthority() const
{
    return flywheelAuthority;
}