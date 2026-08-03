# Project Completion Checklist

## Completed In Repository

1. ESP32 ESP IDF firmware project structure.
2. Sensor polling task with ADC thresholding and debounce.
3. Network independent local alarm task for buzzer and LED.
4. Separate remote alert task for Telegram notifications.
5. WiFi station manager with reconnect handling.
6. RAM event log with serial dump support.
7. Task watchdog setup for safety critical tasks.
8. Block diagram source.
9. Full prototype bill of materials.
10. Wiring and pin map.
11. Validation test plan and calibration worksheet.

## Must Be Done On Physical Hardware

1. Capture real dry, damp, wet, and submerged ADC readings.
2. Replace placeholder `WATER_THRESHOLD_RAW` with calibrated value.
3. Replace `CHANGE_ME` WiFi and Telegram settings locally before flashing.
4. Confirm the selected buzzer is safe for GPIO drive or use the documented transistor driver.
5. Move from breadboard to perfboard or PCB before long term deployment.
6. Install electronics in an enclosure above expected water height.
7. Run the validation plan and document the results.
