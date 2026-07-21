# Smart-Flood-Detection-System

**ESP32-based IoT flood detection system** — real-time water-level sensing with WiFi connectivity, push/app alerts, and a focus on reliable, fail-safe operation for basement flood prevention.

![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Status](https://img.shields.io/badge/status-in%20development-yellow)
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
 │  Water Sensor    │─────▶│    ESP32     │─────▶│  WiFi / MQTT      │
 │ (analog/digital) │      │  (firmware)  │      │  Broker / Backend  │
 └─────────────────┘      └──────┬───────┘      └─────────┬────────┘
                                  │                         │
                          ┌───────▼───────┐        ┌────────▼────────┐
                          │ Local Alarm    │        │  Push Alert      │
                          │ (buzzer/LED)   │        │ (app/Telegram/   │
                          └────────────────┘        │  Pushover/etc.)  │
                                                     └──────────────────┘
```

*(Architecture diagram will be replaced with a detailed schematic as the hardware design is finalized.)*

## Hardware

- **Microcontroller:** ESP32 (WiFi + BLE, dual-core, sufficient ADC pins for analog sensing)
- **Sensor:** Water-level / moisture sensor (analog output for graduated water-level detection rather than simple binary wet/dry)
- **Local alert:** Buzzer and/or LED indicator
- **Power:** [To be finalized — evaluating mains adapter with battery backup for outage resilience]
- **Enclosure:** [To be finalized — IP-rated enclosure for basement/humid environment]

## Bill of Materials

| Component | Purpose | Qty |
|---|---|---|
| ESP32 Dev Board | Main controller, WiFi connectivity | 1 |
| Water-level sensor | Detects water presence/depth | 1 |
| Buzzer | Local audible alarm | 1 |
| LED | Visual status indicator | 1–2 |
| Resistors | Sensor/LED circuit protection | Assorted |
| Breadboard / PCB | Prototyping / final assembly | 1 |
| Enclosure | Environmental protection | 1 |
| Power supply | Mains + battery backup | 1 |

*(Full BOM with part numbers and cost will be added as components are finalized.)*

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

# Flash firmware to ESP32
# (instructions to be added once firmware toolchain is finalized)
```

Detailed step-by-step setup instructions — including NVS-based WiFi provisioning and final alert service setup — will be added as the build progresses. For now, WiFi credentials and Telegram placeholders live in `firmware/main/app_config.h`.

## Wiring Diagram

*(To be added — will include a labeled schematic showing sensor-to-ESP32 connections, power distribution, and buzzer/LED wiring.)*

## Testing & Validation

To ensure this system performs reliably in a real flood scenario, testing will include:

- **Sensor accuracy testing** — validating detection thresholds against known water depths
- **False-positive testing** — exposure to humidity/condensation without standing water
- **Connectivity loss testing** — verifying local alarm still triggers if WiFi is unavailable
- **Power loss testing** — verifying behavior on power interruption (battery backup validation)
- **End-to-end alert latency** — time from water detection to notification received

Results and test logs will be documented here as validation is completed.

## Reliability & Fail-Safe Design

A flood detector is only useful if it works when it matters most. Key design principles:

- **Local-first alarms** — the buzzer/LED triggers independently of WiFi connectivity, so a network outage doesn't silence the alert
- **Isolated event queues** — remote alerting consumes a separate queue, so network code cannot steal or block alarm events
- **Connection recovery** — firmware automatically attempts WiFi reconnection rather than requiring a manual reset
- **Sensor validation** — averaging/debouncing logic to avoid missed or false detections
- **Watchdog coverage** — the safety-critical sensor and alarm tasks are registered with the ESP-IDF task watchdog
- **Power resilience** — evaluating battery backup so the system stays online during a power outage (a common co-occurrence with flooding events)

## Results

*(To be populated with real deployment data: response times, uptime, any detected events, and photos/videos of the working system.)*

## Lessons Learned

*(To be populated as the project progresses — documenting design trade-offs, debugging challenges, and what I'd do differently.)*

## Roadmap

- [ ] Finalize sensor selection and calibrate thresholds using serial raw-ADC calibration mode
- [x] Build firmware task architecture for sensing + local alarm
- [x] Implement WiFi connectivity and reconnection logic
- [x] Integrate Telegram push notification task
- [x] Add local RAM event logging and serial dump trigger
- [x] Add task watchdog coverage for sensor and alarm tasks
- [ ] Add NVS-based WiFi credential provisioning
- [ ] Design and test enclosure for basement environment
- [ ] Add battery backup for power-loss resilience
- [ ] Conduct full end-to-end validation testing
- [ ] Document results and finalize README

## Repository Structure

```
Smart-Flood-Detection-System/
├── firmware/        # ESP32 source code
├── hardware/         # Schematics, BOM, wiring diagrams
├── backend/          # Notification/cloud service (if applicable)
├── docs/             # Architecture diagrams, design notes, test results
├── LICENSE
└── README.md
```
