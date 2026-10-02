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

    // --------------------------------
    // Steering authority
    // --------------------------------

    if (state == STABILITY_STABLE)
    {
        steeringAuthority = 1.0f;
    }
    else
    {
        steeringAuthority = 0.0f;
    }

    // --------------------------------
    // Flywheel remains available
    // in every stability state
    // --------------------------------

    flywheelAuthority = 1.0f;
}

void StabilityController::updateState(
    const IMUData& imuData
)
{
    // ==================================
    // STABLE
    // ==================================

    if (state == STABILITY_STABLE)
    {
        if (outsideLimits(imuData))
        {
            state = STABILITY_UNSTABLE;
            recoveryStartTime = 0;
        }

        return;
    }


    // ==================================
    // UNSTABLE
    // ==================================

    if (state == STABILITY_UNSTABLE)
    {
        if (insideRecoveryLimits(imuData))
        {
            state = STABILITY_RECOVERING;
            recoveryStartTime = millis();
        }

        return;
    }


    // ==================================
    // RECOVERING
    // ==================================

    if (state == STABILITY_RECOVERING)
    {
        // If instability returns,
        // immediately go back to unstable.

        if (outsideLimits(imuData))
        {
            state = STABILITY_UNSTABLE;
            recoveryStartTime = 0;
            return;
        }


        // Must remain inside recovery
        // limits continuously.

        if (
            millis() - recoveryStartTime
            >= STABILITY_RECOVERY_TIME_MS
        )
        {
            state = STABILITY_STABLE;
            recoveryStartTime = 0;
        }
    }
}

bool StabilityController::outsideLimits(
    const IMUData& imuData
) const
{
    if (
        fabs(imuData.roll) >
        STABILITY_ROLL_LIMIT_DEG
    )
    {
        return true;
    }

    if (
        fabs(imuData.pitch) >
        STABILITY_PITCH_LIMIT_DEG
    )
    {
        return true;
    }

    if (
        fabs(imuData.gz) >
        STABILITY_YAW_RATE_LIMIT_DPS
    )
    {
        return true;
    }

    return false;
}

bool StabilityController::insideRecoveryLimits(
    const IMUData& imuData
) const
{
    if (
        fabs(imuData.roll) >
        STABILITY_ROLL_RECOVERY_DEG
    )
    {
        return false;
    }

    if (
        fabs(imuData.pitch) >
        STABILITY_PITCH_RECOVERY_DEG
    )
    {
        return false;
    }

    if (
        fabs(imuData.gz) >
        STABILITY_YAW_RECOVERY_DPS
    )
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