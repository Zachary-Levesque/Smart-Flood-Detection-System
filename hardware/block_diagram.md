# Smart Flood Detection System Block Diagram

```mermaid
flowchart LR
    power["5 V USB Power Adapter<br/>with optional battery backup"] --> esp["ESP32 Dev Board<br/>3.3 V logic + WiFi"]
    water["Analog Water-Level Sensor<br/>GPIO34 / ADC1_CH6"] --> esp
    boot["Boot / Diagnostic Button<br/>GPIO0 pull-up"] --> esp
    esp --> buzzer["Active Buzzer<br/>GPIO25"]
    esp --> led["Status LED<br/>GPIO26 through 220 ohm resistor"]
    esp --> serial["USB Serial Monitor<br/>calibration + event log"]
    esp -. WiFi .-> router["Home WiFi Router"]
    router --> internet["Internet"]
    internet --> telegram["Telegram Bot API"]
    telegram --> phone["Phone Push Notification"]
```

## Signal Flow

1. The water sensor outputs an analog voltage proportional to water contact or depth.
2. The ESP32 samples the sensor on ADC1 channel 6, debounces the reading, and classifies the state as normal or water detected.
3. A detected-water event is sent to the local alarm queue before the remote alert queue.
4. The alarm task turns on the buzzer and status LED without depending on WiFi.
5. If WiFi is connected and Telegram credentials are configured, the alert task sends a remote notification.
6. State changes are stored in a RAM ring buffer and can be dumped over USB serial.

