# Smart-Flood-Detection-System

**ESP32-based IoT flood detection system** — real-time water-level sensing with WiFi connectivity, push/app alerts, and a focus on reliable, fail-safe operation for basement flood prevention.

![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Status](https://img.shields.io/badge/status-prototype%20complete-green)
![License](https://img.shields.io/badge/license-MIT-green)

---
## Motivation

After experiencing a basement flood firsthand, I wanted to build a system that could catch water intrusion early — before it causes significant damage — and notify me immediately, regardless of where I am. Rather than buying an off-the-shelf sensor, I built this project to apply systems engineering and embedded design principles to a real-world problem, and to explore the full stack of an IoT product: sensing, firmware, connectivity, and alerting.

## Overview

This system continuously monitors water presence/level in a basement using a water sensor connected to an ESP32 microcontroller. When water is detected above a defined threshold, the system:

1. Triggers a local alarm (buzzer/LED)
2. Sends a real-time alert to a connected app/notification service over WiFi
3. Logs the event with a timestamp for later review

The design prioritizes **reliability** and **fail-safe behavior** — a flood detector that silently fails is worse than no detector at all, so significant attention is paid to power reliability, connectivity loss handling, and sensor validation.

## System Architecture

```
 ┌─────────────────┐      ┌──────────────┐      ┌──────────────────┐
 │  Water Sensor    │─────▶│    ESP32     │─────▶│ WiFi / HTTPS      │
 │ (analog input)   │      │  (firmware)  │      │ Telegram Bot API   │
 └─────────────────┘      └──────┬───────┘      └─────────┬────────┘
                                  │                         │
                          ┌───────▼───────┐        ┌────────▼────────┐
                          │ Local Alarm    │        │  Push Alert      │
                          │ (buzzer/LED)   │        │ (app/Telegram/   │
                          └────────────────┘        │  Pushover/etc.)  │
                                                     └──────────────────┘
```

See [hardware/block_diagram.svg](hardware/block_diagram.svg) for the visual block diagram and [hardware/block_diagram.md](hardware/block_diagram.md) for the editable Mermaid source.

## Hardware

- **Microcontroller:** ESP32 (WiFi + BLE, dual-core, sufficient ADC pins for analog sensing)
- **Sensor:** Water-level / moisture sensor (analog output for graduated water-level detection rather than simple binary wet/dry)
- **Local alert:** Buzzer and/or LED indicator
- **Power:** 5 V USB wall adapter, with USB battery backup/UPS recommended for outage resilience
- **Enclosure:** Plastic project enclosure with cable glands or grommets; keep electronics above expected water level

## Bill of Materials

The full prototype part list is maintained in [hardware/bill_of_materials.md](hardware/bill_of_materials.md). It includes the ESP32 board, analog water sensor, buzzer, LED, resistors, optional buzzer driver parts, wiring, power supply, backup power, enclosure, strain relief, and mounting hardware.

## Firmware

Firmware is developed for the ESP32 using ESP-IDF with the following responsibilities:

- Periodic sensor polling with debouncing/averaging to avoid false positives from splashes or condensation
- WiFi connection management, including reconnection logic on dropped connections
- Threshold-based alert triggering with configurable sensitivity
- Local event logging (timestamped) for post-event review
- Watchdog timer to recover from firmware hangs

The local buzzer/LED path is intentionally independent of WiFi: sensor events are delivered to a dedicated alarm queue before any optional remote-alert queue.

## Alerting & Connectivity

When the water threshold is exceeded, the firmware can send a Telegram bot notification directly over HTTPS using ESP-IDF's `esp_http_client`. If WiFi is unavailable or credentials are not configured, the remote alert is skipped and the local alarm remains unaffected.

## Setup & Installation

```bash
# Clone the repository
git clone https://github.com/<your-username>/Smart-Flood-Detection-System.git
cd Smart-Flood-Detection-System

# Build and flash firmware to ESP32
cd firmware
. $HOME/esp/esp-idf/export.sh
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

For this prototype, WiFi credentials and Telegram placeholders live in `firmware/main/app_config.h`. Replace the `CHANGE_ME` values locally before flashing and do not commit real credentials.

## Wiring Diagram

See [hardware/wiring.md](hardware/wiring.md) for the pin map, buzzer driver option, and deployment placement notes.

## Testing & Validation

To ensure this system performs reliably in a real flood scenario, testing will include:

- **Sensor accuracy testing** — validating detection thresholds against known water depths
- **False-positive testing** — exposure to humidity/condensation without standing water
- **Connectivity loss testing** — verifying local alarm still triggers if WiFi is unavailable
- **Power loss testing** — verifying behavior on power interruption (battery backup validation)
- **End-to-end alert latency** — time from water detection to notification received

Use [docs/test_plan.md](docs/test_plan.md) to record validation results before deployment.

## Reliability & Fail-Safe Design

A flood detector is only useful if it works when it matters most. Key design principles:

- **Local-first alarms** — the buzzer/LED triggers independently of WiFi connectivity, so a network outage doesn't silence the alert
- **Isolated event queues** — remote alerting consumes a separate queue, so network code cannot steal or block alarm events
- **Connection recovery** — firmware automatically attempts WiFi reconnection rather than requiring a manual reset
- **Sensor validation** — averaging/debouncing logic to avoid missed or false detections
- **Watchdog coverage** — the safety-critical sensor and alarm tasks are registered with the ESP-IDF task watchdog
- **Power resilience** — evaluating battery backup so the system stays online during a power outage (a common co-occurrence with flooding events)

## Results

The prototype design and firmware are complete in this repository. Physical validation must be recorded after assembling the hardware because the final ADC threshold depends on the exact sensor, placement, and water conditions.

## Lessons Learned

The most important design decision is keeping the local alarm path independent from network alerting. Remote notifications are useful, but the buzzer and LED must still activate if WiFi, Telegram, or the internet is unavailable.

## Completion Status

- [x] Document block diagram, BOM, wiring, and validation plan
- [x] Build firmware task architecture for sensing + local alarm
- [x] Implement WiFi connectivity and reconnection logic
- [x] Integrate Telegram push notification task
- [x] Add local RAM event logging and serial dump trigger
- [x] Add task watchdog coverage for sensor and alarm tasks

## Physical Deployment Checklist

- [ ] Calibrate `WATER_THRESHOLD_RAW` using serial raw-ADC calibration mode
- [ ] Add NVS-based WiFi credential provisioning for a production version
- [ ] Design and test enclosure for basement environment
- [ ] Add battery backup for power-loss resilience
- [ ] Conduct full end-to-end validation testing
- [ ] Document measured response time, uptime, and alert delivery results

## Repository Structure

```
Smart-Flood-Detection-System/
├── firmware/        # ESP32 source code
├── hardware/        # Block diagram, BOM, wiring and pin map
├── docs/            # Completion checklist and validation test plan
├── LICENSE
└── README.md
```
