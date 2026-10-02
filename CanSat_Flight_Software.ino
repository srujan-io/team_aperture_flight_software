#include "stability.h"

StabilityController stability;

void printState()
{
    Serial.print("State: ");

    if (stability.isStable())
        Serial.println("STABLE");
    else if (stability.isUnstable())
        Serial.println("UNSTABLE");
    else if (stability.isRecovering())
        Serial.println("RECOVERING");

    Serial.print("Steering authority: ");
    Serial.println(stability.getSteeringAuthority());

    Serial.print("Flywheel authority: ");
    Serial.println(stability.getFlywheelAuthority());

    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    delay(2000);

    stability.begin();

    Serial.println("==============================");
    Serial.println("STABILITY CONTROLLER TEST");
    Serial.println("==============================");

    // --------------------------------
    // Test 1: Normal stable condition
    // --------------------------------

    IMUData imu;

    imu.roll = 0.0f;
    imu.pitch = 0.0f;
    imu.gz = 0.0f;

    stability.update(imu);

    Serial.println("TEST 1: Stable");
    printState();


    // --------------------------------
    // Test 2: Excessive yaw rate
    // --------------------------------

    imu.gz = 60.0f;

    stability.update(imu);

    Serial.println("TEST 2: Excessive yaw rate");
    printState();


    // --------------------------------
    // Test 3: Enter recovery limits
    // --------------------------------

    imu.gz = 10.0f;
    imu.roll = 5.0f;
    imu.pitch = 5.0f;

    stability.update(imu);

    Serial.println("TEST 3: Recovery condition");
    printState();


    // --------------------------------
    // Test 4:
    // Wait for recovery timer
    // --------------------------------

    Serial.println("TEST 4: Waiting 1 second...");

    delay(1100);

    stability.update(imu);

    printState();
}

void loop()
{
}