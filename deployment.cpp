#include "deployment.h"
#include "config.h"

DeploymentController::DeploymentController()
{
    servoPin = MG90_SERVO_PIN;
    idleAngle = MG90_IDLE_ANGLE;
    deployAngle = MG90_DEPLOY_ANGLE;

    initialized = false;
    deploying = false;
    deployed = false;

    deploymentStartTime = 0;
}


bool DeploymentController::begin()
{
    if (initialized)
    {
        return true;
    }

    mg90.attach(servoPin);
    mg90.write(idleAngle);

    initialized = true;

    return true;
}


bool DeploymentController::startDeployment()
{
    if (!initialized)
    {
        return false;
    }

    // Deployment is one-shot.
    if (deploying || deployed)
    {
        return false;
    }

    mg90.write(deployAngle);

    deploymentStartTime = millis();
    deploying = true;

    return true;
}


void DeploymentController::update()
{
    if (!deploying)
    {
        return;
    }

    unsigned long elapsed =
        millis() - deploymentStartTime;

    if (elapsed >= MG90_ACTUATION_TIME_MS)
    {
        deploying = false;
        deployed = true;
    }
}


bool DeploymentController::isDeploying() const
{
    return deploying;
}


bool DeploymentController::isDeployed() const
{
    return deployed;
}