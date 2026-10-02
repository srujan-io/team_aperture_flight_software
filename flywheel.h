#ifndef FLYWHEEL_H
#define FLYWHEEL_H

#include <Arduino.h>

class FlywheelController
{
public:

    FlywheelController();

    bool begin();

    void update(float gyroZ);

    void stop();

    int getMotorCommand() const;

    bool isActive() const;

private:

    int enablePin;
    int in1Pin;
    int in2Pin;

    int motorCommand;

    bool initialized;
    bool active;

    float kp;

    int calculateCommand(float gyroZ);

    void setMotor(int command);
};

#endif