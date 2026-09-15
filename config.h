#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// CANsat FLIGHT SOFTWARE
// GLOBAL CONFIGURATION
// ============================================================


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


#endif