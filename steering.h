#ifndef STEERING_H
#define STEERING_H

#include <Arduino.h>
#include <Servo.h>

class SteeringController
{
public:
    SteeringController();

    bool begin();

    void update(float headingError);

    void center();
    void stop();

    int getCommand() const;
    int getAngle() const;

private:
    Servo steeringServo;

    int servoPin;
    int centerAngle;
    int minAngle;
    int maxAngle;

    int currentAngle;
    int steeringCommand;

    bool initialized;

    int calculateCommand(float headingError);
    int commandToAngle(int command);
};

#endif