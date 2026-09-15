#include "kalmanaltitude.h"

KalmanAltitude::KalmanAltitude()
{
    altitude = 0.0f;
    uncertainty = 1.0f;

    processNoise = 0.1f;
    measurementNoise = 4.0f;
}

float KalmanAltitude::update(float measurement)
{
    // Prediction
    uncertainty += processNoise;

    // Kalman gain
    float gain =
        uncertainty /
        (uncertainty + measurementNoise);

    // Correction
    altitude +=
        gain * (measurement - altitude);

    // Update uncertainty
    uncertainty =
        (1.0f - gain) * uncertainty;

    return altitude;
}

void KalmanAltitude::reset(float value)
{
    altitude = value;
    uncertainty = 1.0f;
}

float KalmanAltitude::getAltitude()
{
    return altitude;
}