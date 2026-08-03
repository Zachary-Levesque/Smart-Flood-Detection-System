#pragma once

#include <stdint.h>

#include "driver/gpio.h"
#include "driver/i2c.h"

// =====================================================================
// Pin configuration
// Prototype pin assignments. These match hardware/wiring.md.
// =====================================================================
#define I2C_PORT                    I2C_NUM_0
#define I2C_SDA_GPIO                GPIO_NUM_4
#define I2C_SCL_GPIO                GPIO_NUM_5
#define BUZZER_GPIO                 GPIO_NUM_6
#define STATUS_LED_GPIO             GPIO_NUM_7
#define CALIBRATION_MODE_GPIO       GPIO_NUM_0   // hold low at boot to stream level readings
#define EVENT_LOG_DUMP_GPIO         GPIO_NUM_0   // hold low to dump recent events over serial

// =====================================================================
// Sensor thresholds
// Calibrate this against real level readings from calibration mode:
// 1. Flash firmware, hold CALIBRATION_MODE_GPIO low at boot, and open monitor.
// 2. Record water level percent for dry, damp, wet, and submerged states.
// 3. Set WATER_LEVEL_THRESHOLD_PERCENT to the level that should trigger alarm.
// =====================================================================
#define GROVE_WATER_LOW_ADDR        0x77
#define GROVE_WATER_HIGH_ADDR       0x78
#define GROVE_WATER_TOUCH_THRESHOLD 100
#define WATER_LEVEL_THRESHOLD_PERCENT 10
#define SENSOR_POLL_INTERVAL_MS     2000    // how often to sample the sensor
#define SENSOR_DEBOUNCE_SAMPLES     3       // consecutive samples required to confirm a reading
#define SENSOR_CALIBRATION_FORCE    0       // set to 1 to always stream level readings

// =====================================================================
// Runtime diagnostics
// =====================================================================
#define EVENT_LOG_CAPACITY          32
#define EVENT_LOG_DUMP_ON_BOOT      0       // set to 1 to dump RAM log at startup
#define EVENT_LOG_DUMP_GPIO_ENABLED 1       // hold EVENT_LOG_DUMP_GPIO low at boot to dump
#define EVENT_LOG_DUMP_POLL_MS      1000

// =====================================================================
// Watchdog
// =====================================================================
#define TASK_WATCHDOG_TIMEOUT_MS    10000

// =====================================================================
// WiFi configuration
// Prototype credentials. For production, move credentials to NVS backed
// provisioning before deployment.
// ESP IDF WiFi provisioning examples support BLE or SoftAP flows that store
// credentials in NVS.
// =====================================================================
#define WIFI_SSID                   "CHANGE_ME"
#define WIFI_PASSWORD                "CHANGE_ME"
#define WIFI_MAX_RETRY               5
#define WIFI_RETRY_BACKOFF_MS        30000

// =====================================================================
// Telegram alerting
// Fill these in locally. Do not commit real bot tokens or chat IDs.
// =====================================================================
#define TELEGRAM_BOT_TOKEN          "CHANGE_ME"
#define TELEGRAM_CHAT_ID            "CHANGE_ME"
#define TELEGRAM_ALERT_MESSAGE      "Water detected by Smart Flood Detection System"

// =====================================================================
// Event types passed between tasks via queue
// =====================================================================
typedef enum {
    EVENT_WATER_NORMAL = 0,
    EVENT_WATER_DETECTED,
} system_event_t;
