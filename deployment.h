#ifndef DEPLOYMENT_H
#define DEPLOYMENT_H

#include <Arduino.h>
#include <Servo.h>

class DeploymentController
{
public:

    DeploymentController();

    bool begin();

    void update();

    bool startDeployment();

    bool isDeploying() const;
    bool isDeployed() const;
    bool isInflationComplete() const;

private:

    Servo mg90;

    int servoPin;
    int idleAngle;
    int deployAngle;

    bool initialized;
    bool deploying;
    bool deployed;

    unsigned long deploymentStartTime;
    unsigned long inflationStartTime;
};

#endif