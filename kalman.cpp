#include "kalman.h"

Kalman::Kalman()
{
    Q_angle = 0.001f;
    Q_bias = 0.003f;
    R_measure = 0.03f;

    angle = 0.0f;
    bias = 0.0f;
    rate = 0.0f;

    P[0][0] = 0.0f;
    P[0][1] = 0.0f;
    P[1][0] = 0.0f;
    P[1][1] = 0.0f;
}


float Kalman::update(float newAngle, float newRate, float dt)
{
    if(dt <= 0)
        return angle;

    if(dt > 0.1f)
        dt = 0.1f;


    // Prediction
    rate = newRate - bias;
    angle += dt * rate;


    // Update covariance
    P[0][0] += dt * (dt * P[1][1] 
              - P[0][1] 
              - P[1][0] 
              + Q_angle);

    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;


    // Innovation
    float S = P[0][0] + R_measure;


    // Kalman gain
    float K0 = P[0][0] / S;
    float K1 = P[1][0] / S;


    // Measurement difference
    float y = newAngle - angle;


    // Update estimate
    angle += K0 * y;
    bias += K1 * y;


    // Update covariance
    float P00 = P[0][0];
    float P01 = P[0][1];

    P[0][0] -= K0 * P00;
    P[0][1] -= K0 * P01;
    P[1][0] -= K1 * P00;
    P[1][1] -= K1 * P01;


    return angle;
}


void Kalman::setAngle(float newAngle)
{
    angle = newAngle;
}


float Kalman::getAngle()
{
    return angle;
}