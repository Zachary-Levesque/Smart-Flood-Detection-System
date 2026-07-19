# Firmware — ESP-IDF

This is the ESP-IDF project for the Smart Flood Detection System, targeting the ESP32.

## Task architecture

The firmware is split into independent FreeRTOS tasks. Sensor state changes are fanned out to dedicated queues so the local alarm path remains isolated from networking:

- **`sensor_task`** — polls the water sensor via ADC on a fixed interval, debounces readings to avoid false positives, logs state changes, and pushes each `system_event_t` to the local alarm queue first and the remote alert queue second.
- **`alarm_task`** — consumes events from its dedicated queue and drives the local buzzer/LED. This task has **no dependency on WiFi or networking** — it will fire the physical alarm even if the network is completely down. This is the core fail-safe guarantee of the system.
- **`alert_task`** — consumes events from a separate queue and sends Telegram HTTPS alerts only when `wifi_manager_is_connected()` is true. Failed or skipped remote alerts do not affect local alarm behavior.
- **`wifi_manager`** — connects to the configured access point and handles automatic reconnection on drop. Runs independently and does not block sensor/alarm logic.
- **`event_log`** — stores timestamped water-state events in a small RAM ring buffer and can dump the log over serial when the configured dump GPIO/build flag is active.

This separation (sensing → local action, decoupled from networking) is a deliberate design choice: the system's primary safety function (the local alarm) must not depend on its secondary function (remote alerting).

## Not yet implemented

- **WiFi credential provisioning** — currently hardcoded in `app_config.h` for prototyping; a TODO points to ESP-IDF NVS-backed WiFi provisioning examples for a later BLE/SoftAP setup.
- **Final sensor thresholds** — calibration support is present, but `WATER_THRESHOLD_RAW` remains a placeholder until raw ADC readings are captured for dry / damp / wet / submerged states.
- **Exponential backoff on WiFi reconnection** — current retry logic is a fixed-count retry; noted as a TODO in `wifi_manager.c`.

## Calibration and diagnostics

- Hold `CALIBRATION_MODE_GPIO` low at boot, or set `SENSOR_CALIBRATION_FORCE` to `1`, to stream raw ADC readings over serial while the firmware runs.
- Hold `EVENT_LOG_DUMP_GPIO` low, or set `EVENT_LOG_DUMP_ON_BOOT` to `1`, to dump the RAM event log over serial.
- `sensor_logic.c/.h` contains pure threshold/debounce helpers intended to be reused from the ESP-IDF unit test app.

## Remote alerting

Telegram alerting is implemented in `alert_task`. Fill in `TELEGRAM_BOT_TOKEN` and `TELEGRAM_CHAT_ID` in `app_config.h` locally before flashing; real credentials should not be committed.

## Building

Requires the ESP-IDF toolchain installed and sourced (`. $HOME/esp/esp-idf/export.sh`).

```bash
idf.py set-target esp32
idf.py menuconfig   # optional, to review sdkconfig
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## File overview

```
firmware/
├── CMakeLists.txt          # top-level project build file
├── sdkconfig.defaults       # default build configuration
└── main/
    ├── CMakeLists.txt       # component build file
    ├── app_config.h         # pins, thresholds, WiFi config, shared types
    ├── main.c                # app_main — wires up tasks
    ├── sensor_task.c/.h      # water sensor polling + event fanout
    ├── sensor_logic.c/.h     # unit-testable threshold/debounce helpers
    ├── alarm_task.c/.h       # local buzzer/LED, network-independent
    ├── alert_task.c/.h       # Telegram HTTPS alerting
    ├── event_log.c/.h        # RAM event log + serial dump
    └── wifi_manager.c/.h     # WiFi connection + reconnection handling
```
