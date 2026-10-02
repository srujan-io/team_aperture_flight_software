#include "flywheel.h"
#include "config.h"

#include <Arduino.h>
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
    {
        return true;
    }

    pinMode(enablePin, OUTPUT);
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);

    stop();

    initialized = true;

    return true;
}


void FlywheelController::update(float gyroZ)
{
    if (!initialized)
    {
        return;
    }

    active = true;

    int command = calculateCommand(gyroZ);

    setMotor(command);
}


int FlywheelController::calculateCommand(float gyroZ)
{
    // Desired angular rate is 0 °/s.
    float error = -gyroZ;

    // Deadband around zero angular rate.
    if (fabs(error) <= FLYWHEEL_DEADBAND_DPS)
    {
        return 0;
    }

    // Proportional control.
    float output = kp * error;

    // Limit output.
    if (output > FLYWHEEL_MAX_PWM)
    {
        output = FLYWHEEL_MAX_PWM;
    }

    if (output < -FLYWHEEL_MAX_PWM)
    {
        output = -FLYWHEEL_MAX_PWM;
    }

    return (int)output;
}


void FlywheelController::setMotor(int command)
{
    motorCommand = command;

    if (command == 0)
    {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, LOW);

        analogWrite(enablePin, 0);

        return;
    }

    if (command > 0)
    {
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);
    }
    else
    {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, HIGH);
    }

    int pwm = abs(command);

    if (pwm > FLYWHEEL_MAX_PWM)
    {
        pwm = FLYWHEEL_MAX_PWM;
    }

    analogWrite(enablePin, pwm);
}


void FlywheelController::stop()
{
    motorCommand = 0;
    active = false;

    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);

    analogWrite(enablePin, 0);
}


int FlywheelController::getMotorCommand() const
{
    return motorCommand;
}


bool FlywheelController::isActive() const
{
    return active;
}