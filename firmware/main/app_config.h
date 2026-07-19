#pragma once

#include <stdint.h>

#include "driver/adc.h"
#include "driver/gpio.h"

// ---------------------------------------------------------------------
// Pin configuration
// TODO: confirm final pin assignments once hardware layout is finalized
// ---------------------------------------------------------------------
#define WATER_SENSOR_ADC_CHANNEL   ADC1_CHANNEL_6  // GPIO34 (input-only, ADC1)
#define BUZZER_GPIO                 GPIO_NUM_25
#define STATUS_LED_GPIO             GPIO_NUM_26
#define CALIBRATION_MODE_GPIO       GPIO_NUM_0   // hold low at boot to stream raw ADC readings
#define EVENT_LOG_DUMP_GPIO         GPIO_NUM_0   // hold low to dump recent events over serial

// ---------------------------------------------------------------------
// Sensor thresholds
// TODO: Calibrate this against real serial readings from calibration mode:
// 1. Flash firmware, hold CALIBRATION_MODE_GPIO low at boot, and open monitor.
// 2. Record raw ADC values for dry, damp, wet, and submerged sensor states.
// 3. Set WATER_THRESHOLD_RAW between the highest non-flood reading and the
//    lowest reading that should trigger the alarm.
// ---------------------------------------------------------------------
#define WATER_THRESHOLD_RAW         2000    // placeholder only; replace after calibration
#define SENSOR_POLL_INTERVAL_MS     2000    // how often to sample the sensor
#define SENSOR_DEBOUNCE_SAMPLES     3       // consecutive samples required to confirm a reading
#define SENSOR_CALIBRATION_FORCE    0       // set to 1 to always stream raw ADC readings

// ---------------------------------------------------------------------
// Runtime diagnostics
// ---------------------------------------------------------------------
#define EVENT_LOG_CAPACITY          32
#define EVENT_LOG_DUMP_ON_BOOT      0       // set to 1 to dump RAM log at startup
#define EVENT_LOG_DUMP_GPIO_ENABLED 1       // hold EVENT_LOG_DUMP_GPIO low at boot to dump
#define EVENT_LOG_DUMP_POLL_MS      1000

// ---------------------------------------------------------------------
// Watchdog
// ---------------------------------------------------------------------
#define TASK_WATCHDOG_TIMEOUT_MS    10000

// ---------------------------------------------------------------------
// WiFi configuration
// TODO: Move credentials to NVS-backed provisioning before deployment.
// ESP-IDF's WiFi provisioning examples support BLE/SoftAP flows that store
// credentials in NVS:
// https://github.com/espressif/esp-idf/tree/master/examples/provisioning/wifi_prov_mgr
// ---------------------------------------------------------------------
#define WIFI_SSID                   "CHANGE_ME"
#define WIFI_PASSWORD                "CHANGE_ME"
#define WIFI_MAX_RETRY               5

// ---------------------------------------------------------------------
// Telegram alerting
// Fill these in locally. Do not commit real bot tokens or chat IDs.
// ---------------------------------------------------------------------
#define TELEGRAM_BOT_TOKEN          "CHANGE_ME"
#define TELEGRAM_CHAT_ID            "CHANGE_ME"
#define TELEGRAM_ALERT_MESSAGE      "Water detected by Smart Flood Detection System"

// ---------------------------------------------------------------------
// Event types passed between tasks via queue
// ---------------------------------------------------------------------
typedef enum {
    EVENT_WATER_NORMAL = 0,
    EVENT_WATER_DETECTED,
} system_event_t;
