# Firmware

This is the ESP IDF project for the Smart Flood Detection System, targeting the ESP32.

## Task Architecture

The firmware is split into independent FreeRTOS tasks. Sensor state changes are sent to dedicated queues so the local alarm path remains isolated from networking.

1. `sensor_task` polls the water sensor with ADC, debounces readings, logs state changes, and sends each `system_event_t` to the local alarm queue first and the remote alert queue second.
2. `alarm_task` consumes its dedicated queue and drives the local buzzer and LED. It has no dependency on WiFi or networking.
3. `alert_task` consumes a separate queue and sends Telegram HTTPS alerts only when `wifi_manager_is_connected()` is true.
4. `wifi_manager` connects to the configured access point and handles automatic reconnect after drops.
5. `event_log` stores timestamped water state events in a RAM ring buffer and can dump the log over serial.

This separation is deliberate. The primary safety function is the local alarm, and it must not depend on remote alerting.

## Production Hardening

1. WiFi credentials are hardcoded in `app_config.h` for the prototype. A production build should use NVS backed WiFi provisioning with BLE or SoftAP setup.
2. `WATER_THRESHOLD_RAW` must be set from raw ADC readings captured for dry, damp, wet, and submerged states.
3. Long term deployment should use perfboard or a PCB in an enclosure.

## Calibration And Diagnostics

1. Hold `CALIBRATION_MODE_GPIO` low at boot, or set `SENSOR_CALIBRATION_FORCE` to `1`, to stream raw ADC readings over serial while the firmware runs.
2. Hold `EVENT_LOG_DUMP_GPIO` low, or set `EVENT_LOG_DUMP_ON_BOOT` to `1`, to dump the RAM event log over serial.
3. `sensor_logic.c/.h` contains pure threshold and debounce helpers intended for reuse from an ESP IDF unit test app.

## Remote Alerting

Telegram alerting is implemented in `alert_task`. Fill in `TELEGRAM_BOT_TOKEN` and `TELEGRAM_CHAT_ID` in `app_config.h` locally before flashing. Real credentials should not be committed.

## Building

Requires the ESP IDF toolchain installed and sourced.

```
idf.py set target esp32
idf.py menuconfig
idf.py build
idf.py flash monitor
```

## File Overview

```
firmware
CMakeLists.txt          top level project build file
sdkconfig.defaults      default build configuration
main
  CMakeLists.txt        component build file
  app_config.h          pins, thresholds, WiFi config, shared types
  main.c                app_main wires up tasks
  sensor_task.c/.h      water sensor polling and event fanout
  sensor_logic.c/.h     unit testable threshold and debounce helpers
  alarm_task.c/.h       local buzzer and LED
  alert_task.c/.h       Telegram HTTPS alerting
  event_log.c/.h        RAM event log and serial dump
  wifi_manager.c/.h     WiFi connection and reconnect handling
```
