#ifndef KALMAN_H
#define KALMAN_H

class Kalman
{
public:
    Kalman();

    float update(float newAngle, float newRate, float dt);

    void setAngle(float angle);
    float getAngle();

private:

    // noise
    float Q_angle;
    float Q_bias;

    // measured noise
    float R_measure;

    float angle;
    float bias;
    float rate;

    float P[2][2];
};

#endif