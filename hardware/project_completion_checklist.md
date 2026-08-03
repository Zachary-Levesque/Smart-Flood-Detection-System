# Completion Checklist

## Done

1. Firmware reads the Grove water level sensor over I2C.
2. Firmware controls the buzzer and LED.
3. Firmware sends Telegram alerts.
4. Firmware reconnects WiFi after drops.
5. Hardware list and wiring notes are included.

## Before Final Use

1. Set the real WiFi and Telegram values locally.
2. Calibrate `WATER_LEVEL_THRESHOLD_PERCENT`.
3. Test the sensor with water.
4. Confirm the buzzer and LED turn on.
5. Place the ESP32 in an enclosure above the floor.
