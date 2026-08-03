# Smart Flood Detection System

**ESP32 based IoT flood detection system.** It monitors water level in real time, triggers a local buzzer and LED alarm, sends an optional Telegram alert over WiFi, and keeps recent events available over serial.

## Motivation

After experiencing a basement flood firsthand, I wanted a system that could catch water intrusion early and notify me immediately. This project applies embedded design, sensing, connectivity, and reliability thinking to a real home safety problem.

## Overview

This system continuously monitors water presence using an analog water sensor connected to an ESP32. When water is detected above the calibrated threshold, the system:

1. Triggers the local buzzer and LED.
2. Sends a Telegram notification when WiFi and credentials are available.
3. Logs the event with a timestamp for later serial review.

The design prioritizes reliability and fail safe behavior. The local alarm does not depend on WiFi, Telegram, or internet access.

## System Architecture

```
Water Sensor  to  ESP32 Firmware  to  WiFi and Telegram
                         |
                         v
                 Local Buzzer and LED
```

See [hardware/block_diagram.md](hardware/block_diagram.md) for the block diagram.

## Hardware

1. **Microcontroller:** ESP32 development board with WiFi and ADC input.
2. **Sensor:** Analog water level or moisture sensor.
3. **Local alert:** Buzzer and status LED.
4. **Power:** 5 V USB wall adapter, with USB battery backup or UPS recommended.
5. **Enclosure:** Plastic project enclosure with cable glands or grommets. Keep electronics above expected water level.

## Bill Of Materials

The full prototype part list is maintained in [hardware/bill_of_materials.md](hardware/bill_of_materials.md). It includes the ESP32 board, analog water sensor, buzzer, LED, resistors, optional buzzer driver parts, wiring, power supply, backup power, enclosure, strain relief, and mounting hardware.

## Firmware

Firmware is developed for the ESP32 using ESP IDF. It handles:

1. Periodic sensor polling with debounce logic.
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

See [hardware/wiring.md](hardware/wiring.md) for the pin map, buzzer driver option, and deployment placement notes.

## Testing And Validation

To ensure this system performs reliably in a real flood scenario, testing includes:

1. Sensor accuracy testing with known dry, damp, wet, and submerged states.
2. False alarm testing with humidity and brief splashes.
3. Connectivity loss testing to confirm the local alarm still triggers without WiFi.
4. Power interruption testing with the selected backup power method.
5. End to end alert latency testing.

Use [hardware/test_plan.md](hardware/test_plan.md) to record validation results before deployment.

## Reliability And Fail Safe Design

Key design principles:

1. Local first alarms keep buzzer and LED behavior independent from WiFi.
2. Separate queues prevent network work from blocking the local alarm path.
3. Reconnect handling keeps WiFi recovery automatic.
4. Sensor debounce logic reduces false triggers.
5. Task watchdog coverage helps recover from firmware hangs.
6. Battery backup is recommended because power loss can happen during storms.

## Results

The prototype design and firmware are complete in this repository. Physical validation must be recorded after assembling the hardware because the final ADC threshold depends on the exact sensor, placement, and water conditions.

## Lessons Learned

The most important design decision is keeping the local alarm path independent from network alerting. Remote notifications are useful, but the buzzer and LED must still activate if WiFi, Telegram, or the internet is unavailable.

## Completion Status

1. Done: Document block diagram, BOM, wiring, and validation plan.
2. Done: Build firmware task architecture for sensing and local alarm.
3. Done: Implement WiFi connectivity and reconnect logic.
4. Done: Integrate Telegram push notification task.
5. Done: Add local RAM event logging and serial dump trigger.
6. Done: Add task watchdog coverage for sensor and alarm tasks.

## Physical Deployment Checklist

1. Calibrate `WATER_THRESHOLD_RAW` using serial raw ADC calibration mode.
2. Add NVS based WiFi credential provisioning for a production version.
3. Design and test enclosure for basement environment.
4. Add battery backup for power loss resilience.
5. Conduct full end to end validation testing.
6. Document measured response time, uptime, and alert delivery results.

## Repository Structure

```
Smart Flood Detection System
firmware    ESP32 source code
hardware    Block diagram, BOM, wiring, validation plan, and checklist
LICENSE
README.md
```
