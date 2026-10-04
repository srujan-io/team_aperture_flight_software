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
    inflationStartTime = 0;
}


// BEGIN

bool DeploymentController::begin()
{
    if (initialized)
    {
        return true;
    }

    mg90.attach(servoPin);

    // Initial locked position
    mg90.write(idleAngle);

    initialized = true;

    return true;
}


// START DEPLOYMENT

bool DeploymentController::startDeployment()
{
    if (!initialized)
    {
        return false;
    }

    // Deployment is one-shot
    if (deploying || deployed)
    {
        return false;
    }

    Serial.println("MG90: Deployment started.");

    // Move rack-and-pinion mechanism
    mg90.write(deployAngle);

    deploymentStartTime = millis();

    deploying = true;
    deployed = false;

    return true;
}


// UPDATE

void DeploymentController::update()
{
    if (!deploying)
    {
        return;
    }

    unsigned long elapsed =
        millis() - deploymentStartTime;

    // MG90 actuation complete
    if (elapsed >= MG90_ACTUATION_TIME_MS)
    {
        deploying = false;
        deployed = true;

        // Start paraglider inflation timer
        inflationStartTime = millis();

        Serial.println("MG90: Deployment complete.");
        Serial.println("Paraglider inflation started.");
    }
}


// DEPLOYMENT STATUS

bool DeploymentController::isDeploying() const
{
    return deploying;
}


bool DeploymentController::isDeployed() const
{
    return deployed;
}


// INFLATION STATUS

bool DeploymentController::isInflationComplete() const
{
    if (!deployed)
    {
        return false;
    }

    unsigned long inflationElapsed =
        millis() - inflationStartTime;

    return inflationElapsed >=
           PARAGLIDER_INFLATION_TIME_MS;
}