#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// CANsat FLIGHT SOFTWARE
// GLOBAL CONFIGURATION
// ============================================================


// ===============================
// Launch Pad / Target Coordinates
// ===============================

#define TARGET_LATITUDE     26.717535
#define TARGET_LONGITUDE    84.301543


// ============================================================
// SYSTEM
// ============================================================

#define TEAM_ID "CAN-7USAT-024"

#define SERIAL_BAUD_RATE 115200



// ============================================================
// TASK TIMING
// ============================================================

// Sensor / data logging rate
#define SENSOR_UPDATE_INTERVAL_MS       100UL     // 10 Hz

// Telemetry transmission rate
#define TELEMETRY_UPDATE_INTERVAL_MS    1000UL    // 1 Hz


// ============================================================
// TELEMETRY
// ============================================================

#define TELEMETRY_RATE_HZ 1

#define TEAM_ID_LENGTH          32
#define GNSS_TIME_LENGTH        16
#define FLIGHT_STATE_LENGTH     24


// ============================================================
// FLIGHT ALTITUDE THRESHOLDS
// ============================================================

// Paraglider deployment threshold
#define PARAGLIDER_DEPLOY_ALTITUDE_M    600.0f
#define MAX_VALID_ALTITUDE_M 2000.0f


// ============================================================
// PARAGLIDER DEPLOYMENT
// ============================================================

// Time allowed for canopy inflation
#define PARAGLIDER_INFLATION_TIME_MS    5000UL

// Minimum inflation time
#define PARAGLIDER_MIN_INFLATION_MS     3000UL


// ============================================================
// FLIGHT STATES
// ============================================================

#define STATE_BOOT              0
#define STATE_TEST_MODE         1
#define STATE_LAUNCH_PAD        2
#define STATE_ASCENT             3
#define STATE_ROCKET_DEPLOY      4
#define STATE_DESCENT            5
#define STATE_PARAGLIDER_DEPLOY  6
#define STATE_PARAGLIDE_ACTIVE   7
#define STATE_IMPACT             8


// ============================================================
// FLIGHT STATE NAMES
// ============================================================

#define STATE_NAME_BOOT              "BOOT"
#define STATE_NAME_TEST_MODE         "TEST_MODE"
#define STATE_NAME_LAUNCH_PAD        "LAUNCH_PAD"
#define STATE_NAME_ASCENT            "ASCENT"
#define STATE_NAME_ROCKET_DEPLOY     "ROCKET_DEPLOY"
#define STATE_NAME_DESCENT           "DESCENT"
#define STATE_NAME_PARAGLIDER_DEPLOY "PARAGLIDER_DEPLOY"
#define STATE_NAME_PARAGLIDE_ACTIVE  "PARAGLIDE_ACTIVE"
#define STATE_NAME_IMPACT            "IMPACT"


// ============================================================
// GENERAL FLIGHT PARAMETERS
// ============================================================

// Used later by launch detection logic.
// Actual threshold should be finalized using IMU testing.
#define LAUNCH_DETECTION_ALTITUDE_M   10.0f

#define PARAGLIDER_OPERATIONAL_ALTITUDE_M  600.0f

// Adjustable deployment margin above operational altitude.
// Final value will be determined through testing.
#define PARAGLIDER_DEPLOYMENT_MARGIN_M     10.0f

#define PARAGLIDER_DEPLOYMENT_TRIGGER_M \
    (PARAGLIDER_OPERATIONAL_ALTITUDE_M + PARAGLIDER_DEPLOYMENT_MARGIN_M)

// MG90 deployment configuration
#define MG90_SERVO_PIN                     15
#define MG90_IDLE_ANGLE                   0
#define MG90_DEPLOY_ANGLE                90

// Time allowed for the MG90/rack mechanism to complete its movement.
// This will be measured and adjusted during testing.
#define MG90_ACTUATION_TIME_MS          1000UL


// ===============================
// Flywheel stabilization
// ===============================

// L298 motor driver pins
#define FLYWHEEL_EN_PIN             6
#define FLYWHEEL_IN1_PIN            7
#define FLYWHEEL_IN2_PIN            8

// Motor direction.
// Change to -1 if physical motor direction is reversed.
#define FLYWHEEL_DIRECTION          1

// Maximum motor command
#define FLYWHEEL_MAX_PWM            255

// Gyro Z rate considered stable.
// Units: degrees/second
#define FLYWHEEL_DEADBAND_DPS       2.0f

// Maximum gyro rate for full flywheel authority.
// Units: degrees/second
#define FLYWHEEL_MAX_RATE_DPS       30.0f

// ===============================
// Flywheel / L298
// ===============================

#define FLYWHEEL_EN_PIN             6
#define FLYWHEEL_IN1_PIN            7
#define FLYWHEEL_IN2_PIN            8

#define FLYWHEEL_MAX_PWM            255

#define FLYWHEEL_DEADBAND_DPS       2.0f

#define FLYWHEEL_KP                 5.0f


// ===============================
// Stability Control
// ===============================

// Roll/pitch limits
#define STABILITY_ROLL_LIMIT_DEG       30.0f
#define STABILITY_PITCH_LIMIT_DEG      30.0f

// Angular-rate limits
#define STABILITY_YAW_RATE_LIMIT_DPS   45.0f

// Recovery thresholds.
// These are deliberately lower than the unstable thresholds,
// providing hysteresis.
#define STABILITY_ROLL_RECOVERY_DEG    20.0f
#define STABILITY_PITCH_RECOVERY_DEG   20.0f
#define STABILITY_YAW_RECOVERY_DPS     25.0f

#define STABILITY_RECOVERY_TIME_MS      1000UL

// ===============================
// OT90MR Steering
// ===============================
#define STEERING_SERVO_PIN             16

#define STEERING_CENTER_ANGLE         90
#define STEERING_MIN_ANGLE             0
#define STEERING_MAX_ANGLE           180

#define STEERING_DEADBAND_DEG         10.0f
#define STEERING_MAX_ERROR_DEG        90.0f

#define STEERING_MIN_DEFLECTION       15
#define STEERING_MAX_DEFLECTION       45

#define STEERING_UPDATE_INTERVAL_MS   100UL

// ===============================
// Stability Control
// ===============================
#define STABILITY_ROLL_LIMIT_DEG        30.0f
#define STABILITY_PITCH_LIMIT_DEG       30.0f
#define STABILITY_YAW_RATE_LIMIT_DPS    45.0f

#define STABILITY_ROLL_RECOVERY_DEG     20.0f
#define STABILITY_PITCH_RECOVERY_DEG    20.0f
#define STABILITY_YAW_RECOVERY_DPS      25.0f

#define STABILITY_RECOVERY_TIME_MS      1000UL

#endif