# Smart Flood Detection System Block Diagram

## Visual Flow

```
5 V USB Power Adapter with optional battery backup
        to
ESP32 Dev Board with 3.3 V logic and WiFi

Analog Water Level Sensor on GPIO34
        to
ESP32 ADC input

ESP32 local alarm queue
        to
GPIO25 active buzzer
        and
GPIO26 status LED

ESP32 WiFi
        to
Home WiFi Router
        to
Telegram Bot API
        to
Phone Push Notification

ESP32 USB Serial
        to
Calibration readings and event log dump
```

## Signal Flow

1. The water sensor outputs an analog voltage proportional to water contact or depth.
2. The ESP32 samples the sensor on ADC1 channel 6, debounces the reading, and classifies the state as normal or water detected.
3. A water event is sent to the local alarm queue before the remote alert queue.
4. The alarm task turns on the buzzer and status LED without depending on WiFi.
5. If WiFi is connected and Telegram credentials are configured, the alert task sends a remote notification.
6. State changes are stored in a RAM ring buffer and can be dumped over USB serial.
