#ifndef KALMAN_H
#define KALMAN_H

class Kalman
{
public:
    Kalman();

    float update(float newAngle, float newRate, float dt);

    void setAngle(float angle);
    float getAngle() const;

private:

    // Process noise
    float Q_angle;
    float Q_bias;

    // Measurement noise
    float R_measure;

    // State
    float angle;
    float bias;
    float rate;

    // Error covariance matrix
    float P[2][2];
};

#endif