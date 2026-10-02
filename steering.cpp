#include "steering.h"
#include "config.h"

#include <math.h>

SteeringController::SteeringController()
{
    servoPin = STEERING_SERVO_PIN;

    centerAngle = STEERING_CENTER_ANGLE;

    minAngle = STEERING_MIN_ANGLE;
    maxAngle = STEERING_MAX_ANGLE;

    currentAngle = centerAngle;
    steeringCommand = 0;

    initialized = false;
}

bool SteeringController::begin()
{
    if (initialized)
        return true;

    steeringServo.attach(servoPin);

    center();

    initialized = true;

    return true;
}

void SteeringController::update(float headingError)
{
    if (!initialized)
        return;

    steeringCommand =
        calculateCommand(headingError);

    currentAngle =
        commandToAngle(steeringCommand);

    steeringServo.write(currentAngle);
}

int SteeringController::calculateCommand(float headingError)
{
    float errorMagnitude =
        fabs(headingError);

    // -------------------------------
    // Deadband
    // -------------------------------

    if (errorMagnitude <= STEERING_DEADBAND_DEG)
        return 0;

    // -------------------------------
    // Limit error used for control
    // -------------------------------

    if (errorMagnitude > STEERING_MAX_ERROR_DEG)
        errorMagnitude = STEERING_MAX_ERROR_DEG;

    // -------------------------------
    // Normalize 0 → 1
    // -------------------------------

    float normalized =
        (errorMagnitude - STEERING_DEADBAND_DEG) /
        (STEERING_MAX_ERROR_DEG - STEERING_DEADBAND_DEG);

    // -------------------------------
    // Map to servo deflection
    // -------------------------------

    float deflection =
        STEERING_MIN_DEFLECTION +
        normalized *
        (
            STEERING_MAX_DEFLECTION -
            STEERING_MIN_DEFLECTION
        );

    // -------------------------------
    // Direction
    //
    // Positive error = one direction
    // Negative error = opposite
    // -------------------------------

    if (headingError > 0.0f)
        return (int)deflection;

    return -(int)deflection;
}

int SteeringController::commandToAngle(int command)
{
    int angle =
        centerAngle + command;

    if (angle < minAngle)
        angle = minAngle;

    if (angle > maxAngle)
        angle = maxAngle;

    return angle;
}

void SteeringController::center()
{
    currentAngle = centerAngle;
    steeringCommand = 0;

    if (initialized)
        steeringServo.write(centerAngle);
}

void SteeringController::stop()
{
    center();
}

int SteeringController::getCommand() const
{
    return steeringCommand;
}

int SteeringController::getAngle() const
{
    return currentAngle;
}