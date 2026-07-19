# Firmware — ESP-IDF

This is the ESP-IDF project for the Smart Flood Detection System, targeting the ESP32.

## Task architecture

The firmware is split into three independent FreeRTOS tasks, connected by a shared event queue:

- **`sensor_task`** — polls the water sensor via ADC on a fixed interval, debounces readings to avoid false positives, and pushes a `system_event_t` to the queue whenever the water state changes.
- **`alarm_task`** — consumes events from the queue and drives the local buzzer/LED. This task has **no dependency on WiFi or networking** — it will fire the physical alarm even if the network is completely down. This is the core fail-safe guarantee of the system.
- **`wifi_manager`** — connects to the configured access point and handles automatic reconnection on drop. Runs independently and does not block sensor/alarm logic.

This separation (sensing → local action, decoupled from networking) is a deliberate design choice: the system's primary safety function (the local alarm) must not depend on its secondary function (remote alerting).

## Not yet implemented

- **Remote alerting task** — a task that watches for `EVENT_WATER_DETECTED` and, when WiFi is connected, pushes a notification via MQTT/HTTP to a backend or third-party alert service. Stubbed as a TODO in `main.c`.
- **WiFi credential provisioning** — currently hardcoded in `app_config.h` for prototyping; should move to NVS-based provisioning (e.g. via `esp_wifi_provisioning` or a BLE/SoftAP provisioning flow) before this is deployed long-term.
- **Sensor calibration** — `WATER_THRESHOLD_RAW` in `app_config.h` is a placeholder and needs to be calibrated against real sensor readings (dry / damp / wet / submerged).
- **Exponential backoff on WiFi reconnection** — current retry logic is a fixed-count retry; noted as a TODO in `wifi_manager.c`.

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
    ├── sensor_task.c/.h      # water sensor polling + debouncing
    ├── alarm_task.c/.h       # local buzzer/LED, network-independent
    └── wifi_manager.c/.h     # WiFi connection + reconnection handling
```
