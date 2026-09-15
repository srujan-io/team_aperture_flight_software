#ifndef KALMAN_ALTITUDE_H
#define KALMAN_ALTITUDE_H

class KalmanAltitude
{
public:
    KalmanAltitude();

    float update(float measurement);

    void reset(float altitude);

    float getAltitude();

private:
    float altitude;
    float uncertainty;

    float processNoise;
    float measurementNoise;
};

#endif