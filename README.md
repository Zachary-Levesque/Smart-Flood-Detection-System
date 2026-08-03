# Smart Flood Detection System

**ESP32 C3 Mini based IoT flood detection system.** It monitors water level in real time, triggers a local buzzer and LED alarm, sends a Telegram alert over WiFi, and keeps recent events available.

## Motivation

After experiencing a basement flood for the second time in 14 months, I wanted a system that could catch water leakage early and notify me immediately. This project applies embedded design, sensing, connectivity, and reliability thinking to a real home safety problem.

## Overview

This system continuously monitors water presence using a Seeed Studio Grove Water Level Sensor connected to an ESP32 C3 Mini over I2C. When water is detected above the calibrated threshold, the system:

1. Triggers the local buzzer and LED.
2. Sends a Telegram notification when WiFi and credentials are available.
3. Logs the event with a timestamp for later serial review.

The design prioritizes reliability and fail safe behavior.

## System Architecture

See [hardware/block_diagram.md](hardware/block_diagram.md) for the block diagram image.

## Hardware

1. **Microcontroller:** ESP32 C3 Mini.
2. **Sensor:** Seeed Studio Grove Water Level Sensor 101020635.
3. **Local alert:** Buzzer and status LED.
4. **Power:** 5 V USB wall adapter.
5. **Enclosure:** Simple plastic project box.

## Bill Of Materials

The simple part list is in [hardware/bill_of_materials.md](hardware/bill_of_materials.md).

## Firmware

Firmware is developed for the ESP32 C3 Mini using ESP IDF. It handles:

1. Periodic I2C sensor polling with debounce logic.
2. WiFi connection management and reconnect handling.
3. Threshold based water detection.
4. Local event logging.
5. Task watchdog coverage.
6. Telegram alert delivery when configured.

The local buzzer and LED path is intentionally independent of WiFi. Sensor events are delivered to the local alarm queue before any optional remote alert queue.

## Alerting And Connectivity

When the water threshold is exceeded, the firmware can send a Telegram bot notification directly over HTTPS using ESP IDF `esp_http_client`. If WiFi is unavailable or credentials are not configured, the remote alert is skipped and the local alarm remains active.

## Setup And Installation

Install and source the ESP IDF toolchain, then build, flash, and monitor the firmware from the `firmware` folder. Replace the `CHANGE_ME` values in `firmware/main/app_config.h` locally before flashing. Do not commit real WiFi or Telegram credentials.

## Wiring Diagram

See [hardware/wiring.md](hardware/wiring.md) for the simple pin map.

## Testing And Validation

To ensure this system performs reliably in a real flood scenario, testing includes:

1. Sensor accuracy testing with known dry, damp, wet, and submerged states.
2. False alarm testing with humidity and brief splashes.
3. Connectivity loss testing to confirm the local alarm still triggers without WiFi.
4. Power interruption testing with the selected backup power method.
5. End to end alert latency testing.

Use [hardware/test_plan.md](hardware/test_plan.md) for the simple test steps.

## Reliability And Fail Safe Design

Key design principles:

1. Local first alarms keep buzzer and LED behavior independent from WiFi.
2. Separate queues prevent network work from blocking the local alarm path.
3. Reconnect handling keeps WiFi recovery automatic.
4. Sensor debounce logic reduces false triggers.
5. Task watchdog coverage helps recover from firmware hangs.
6. Battery backup is recommended because power loss can happen during storms.

## Results

The prototype design and firmware are complete in this repository. The completed system is implemented and live in my basement!

## Repository Structure

```
Smart Flood Detection System
firmware    ESP32 C3 Mini source code
hardware    Simple block diagram, part list, wiring, and test plan
LICENSE
README.md
```
