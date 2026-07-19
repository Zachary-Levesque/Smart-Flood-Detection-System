#pragma once

#include <stdint.h>

// ---------------------------------------------------------------------
// Pin configuration
// TODO: confirm final pin assignments once hardware layout is finalized
// ---------------------------------------------------------------------
#define WATER_SENSOR_ADC_CHANNEL   ADC1_CHANNEL_6  // GPIO34 (input-only, ADC1)
#define BUZZER_GPIO                 GPIO_NUM_25
#define STATUS_LED_GPIO             GPIO_NUM_26

// ---------------------------------------------------------------------
// Sensor thresholds
// TODO: calibrate against real sensor readings in air / dry / wet / submerged
// ---------------------------------------------------------------------
#define WATER_THRESHOLD_RAW         2000    // placeholder ADC threshold (0-4095)
#define SENSOR_POLL_INTERVAL_MS     2000    // how often to sample the sensor
#define SENSOR_DEBOUNCE_SAMPLES     3       // consecutive samples required to confirm a reading

// ---------------------------------------------------------------------
// WiFi configuration
// TODO: move credentials to NVS / a provisioning flow rather than hardcoding
// ---------------------------------------------------------------------
#define WIFI_SSID                   "CHANGE_ME"
#define WIFI_PASSWORD                "CHANGE_ME"
#define WIFI_MAX_RETRY               5

// ---------------------------------------------------------------------
// Event types passed between tasks via queue
// ---------------------------------------------------------------------
typedef enum {
    EVENT_WATER_NORMAL = 0,
    EVENT_WATER_DETECTED,
} system_event_t;
