# Block Diagram

```
Water Sensor
    to
ESP32
    to
Buzzer and LED

ESP32
    to
WiFi
    to
Telegram Alert

USB Power
    to
ESP32
```

The water sensor goes to the ESP32. The ESP32 turns on the local buzzer and LED when water is detected. If WiFi is working, the ESP32 also sends a Telegram alert.
