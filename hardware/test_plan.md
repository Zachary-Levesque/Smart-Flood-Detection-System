# Test Plan

1. Power on the ESP32 with the sensor dry. The buzzer and LED should stay off.
2. Touch the sensor with water. The buzzer and LED should turn on.
3. Dry the sensor. The buzzer and LED should turn off.
4. Turn off WiFi and wet the sensor. The local alarm should still work.
5. Turn WiFi back on and confirm the Telegram alert works.
6. Leave the system running for a few hours and check for false alarms.
