# Project Completion Checklist

## Completed in Repository

- ESP32 ESP-IDF firmware project structure
- Sensor polling task with ADC thresholding and debounce
- Network-independent local alarm task for buzzer and LED
- Separate remote alert task for Telegram notifications
- WiFi station manager with reconnect handling
- RAM event log with serial dump support
- Task watchdog setup for safety-critical tasks
- Block diagram source
- Full prototype bill of materials
- Wiring and pin map
- Validation test plan and calibration worksheet

## Must Be Done on Physical Hardware

- Capture real dry/damp/wet/submerged ADC readings
- Replace placeholder `WATER_THRESHOLD_RAW` with calibrated value
- Replace `CHANGE_ME` WiFi and Telegram settings locally before flashing
- Confirm the selected buzzer is safe for GPIO drive or use the documented transistor driver
- Move from breadboard to perfboard or PCB before long-term deployment
- Install electronics in an enclosure above expected water height
- Run the validation plan and document the results

