#include "flywheel.h"
#include "config.h"

#include <math.h>

FlywheelController::FlywheelController()
{
    enablePin = FLYWHEEL_EN_PIN;
    in1Pin = FLYWHEEL_IN1_PIN;
    in2Pin = FLYWHEEL_IN2_PIN;

    motorCommand = 0;

    initialized = false;
    active = false;

    kp = FLYWHEEL_KP;
}

bool FlywheelController::begin()
{
    if (initialized)
        return true;

    pinMode(enablePin, OUTPUT);
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);

    setMotor(0);

    initialized = true;
    active = true;

    return true;
}

void FlywheelController::update(float gyroZ)
{
    if (!initialized || !active)
        return;

    motorCommand = calculateCommand(gyroZ);

    setMotor(motorCommand);
}

int FlywheelController::calculateCommand(float gyroZ)
{
    // Desired Z-axis angular velocity = 0
    float error = -gyroZ;

    // Apply configured physical direction
    error *= FLYWHEEL_DIRECTION;

    // Ignore very small rotation rates
    if (fabs(error) <= FLYWHEEL_DEADBAND_DPS)
        return 0;

    // Proportional control
    float command = kp * error;

    // Limit command
    if (command > FLYWHEEL_MAX_PWM)
        command = FLYWHEEL_MAX_PWM;

    if (command < -FLYWHEEL_MAX_PWM)
        command = -FLYWHEEL_MAX_PWM;

    return (int)command;
}

void FlywheelController::setMotor(int command)
{
    if (command > 0)
    {
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);

        analogWrite(enablePin, command);
    }
    else if (command < 0)
    {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, HIGH);

        analogWrite(enablePin, -command);
    }
    else
    {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, LOW);

        analogWrite(enablePin, 0);
    }
}

void FlywheelController::stop()
{
    if (!initialized)
        return;

    motorCommand = 0;

    setMotor(0);

    active = false;
}

int FlywheelController::getMotorCommand() const
{
    return motorCommand;
}

bool FlywheelController::isActive() const
{
    return active;
}