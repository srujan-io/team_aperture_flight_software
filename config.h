#ifndef CONFIG_H
#define CONFIG_H

// CANSAT FLIGHT SOFTWARE
// GLOBAL CONFIGURATION


// SYSTEM

#define TEAM_ID                 "CAN-7USAT-024"
#define SERIAL_BAUD_RATE       115200


// TASK TIMING

#define SENSOR_UPDATE_INTERVAL_MS       100UL
#define TELEMETRY_UPDATE_INTERVAL_MS    1000UL


// TELEMETRY

#define TELEMETRY_RATE_HZ       1
#define TEAM_ID_LENGTH          32
#define GNSS_TIME_LENGTH        16
#define FLIGHT_STATE_LENGTH     24


// TARGET COORDINATES

#define TARGET_LATITUDE         26.717535
#define TARGET_LONGITUDE        84.301543


// FLIGHT ALTITUDE PARAMETERS

// Maximum altitude considered valid by software
#define MAX_VALID_ALTITUDE_M               2000.0f

// Normal operational altitude for paraglider
#define PARAGLIDER_OPERATIONAL_ALTITUDE_M  600.0f

// Deployment margin above operational altitude
#define PARAGLIDER_DEPLOYMENT_MARGIN_M     10.0f

// Actual deployment trigger altitude
#define PARAGLIDER_DEPLOYMENT_TRIGGER_M \
    (PARAGLIDER_OPERATIONAL_ALTITUDE_M + \
     PARAGLIDER_DEPLOYMENT_MARGIN_M)


// DROP / DESCENT DETECTION

// Minimum downward vertical velocity used as an initial
// descent criterion.
// Units: m/s
#define DESCENT_VELOCITY_THRESHOLD_MS      -2.0f

// Number of consecutive samples required to confirm
// sustained descent.
#define DESCENT_CONFIRMATION_SAMPLES       3

// Number of consecutive samples required by the
// initial drop detector.
#define DROP_CONFIRMATION_SAMPLES          3


// PARAGLIDER DEPLOYMENT

// Time allowed for canopy inflation
#define PARAGLIDER_INFLATION_TIME_MS       5000UL

// Minimum inflation time
#define PARAGLIDER_MIN_INFLATION_MS        3000UL


// MG90 DEPLOYMENT SERVO

#define MG90_SERVO_PIN                     15
#define MG90_IDLE_ANGLE                   0
#define MG90_DEPLOY_ANGLE                 90

// Time allowed for rack mechanism to complete movement.
// Adjust after physical testing.
#define MG90_ACTUATION_TIME_MS             1000UL


// FLIGHT STATES

#define STATE_BOOT              0
#define STATE_TEST_MODE         1
#define STATE_LAUNCH_PAD        2
#define STATE_DROP_DETECTED     3
#define STATE_DESCENT           4
#define STATE_PARAGLIDER_DEPLOY 5
#define STATE_PARAGLIDE_ACTIVE  6
#define STATE_IMPACT            7


// FLIGHT STATE NAMES

#define STATE_NAME_BOOT               "BOOT"
#define STATE_NAME_TEST_MODE          "TEST_MODE"
#define STATE_NAME_LAUNCH_PAD         "LAUNCH_PAD"
#define STATE_NAME_DROP_DETECTED      "DROP_DETECTED"
#define STATE_NAME_DESCENT            "DESCENT"
#define STATE_NAME_PARAGLIDER_DEPLOY  "PARAGLIDER_DEPLOY"
#define STATE_NAME_PARAGLIDE_ACTIVE   "PARAGLIDE_ACTIVE"
#define STATE_NAME_IMPACT             "IMPACT"


// FLYWHEEL STABILIZATION

// L298 motor driver pins
#define FLYWHEEL_EN_PIN            6
#define FLYWHEEL_IN1_PIN           7
#define FLYWHEEL_IN2_PIN           8

// Motor direction.
// Change to -1 after physical direction testing if required.
#define FLYWHEEL_DIRECTION         1

// Maximum motor command
#define FLYWHEEL_MAX_PWM           255

// Gyro Z rate below this value is treated as stable.
// Units: degrees/second
#define FLYWHEEL_DEADBAND_DPS      2.0f

// Proportional gain for flywheel control
#define FLYWHEEL_KP                5.0f

// Maximum gyro rate used for full flywheel authority.
// Units: degrees/second
#define FLYWHEEL_MAX_RATE_DPS      30.0f


// STABILITY CONTROL

// Instability thresholds
#define STABILITY_ROLL_LIMIT_DEG        30.0f
#define STABILITY_PITCH_LIMIT_DEG       30.0f
#define STABILITY_YAW_RATE_LIMIT_DPS    45.0f

// Recovery thresholds
#define STABILITY_ROLL_RECOVERY_DEG     20.0f
#define STABILITY_PITCH_RECOVERY_DEG    20.0f
#define STABILITY_YAW_RECOVERY_DPS      25.0f

// Time required inside recovery limits before
// returning to STABLE.
#define STABILITY_RECOVERY_TIME_MS      1000UL


// OT90MR STEERING

#define STEERING_SERVO_PIN             16
#define STEERING_CENTER_ANGLE          90
#define STEERING_MIN_ANGLE             0
#define STEERING_MAX_ANGLE             180

#define STEERING_DEADBAND_DEG          10.0f
#define STEERING_MAX_ERROR_DEG         90.0f

#define STEERING_MIN_DEFLECTION        15
#define STEERING_MAX_DEFLECTION        45

#define STEERING_UPDATE_INTERVAL_MS    100UL


// GNSS / NAVIGATION SAFETY

// Minimum satellites required for active steering
#define GNSS_MIN_SATELLITES            6

// Maximum age allowed for COG data
// Units: milliseconds
#define GNSS_MAX_COURSE_AGE_MS         2000UL

//mission manager

#define STATE_BOOT              0
#define STATE_TEST_MODE         1
#define STATE_LAUNCH_PAD        2
#define STATE_DROP_DETECTED     3
#define STATE_DESCENT            4
#define STATE_PARAGLIDER_DEPLOY 5
#define STATE_PARAGLIDE_ACTIVE  6
#define STATE_IMPACT            7


#endif