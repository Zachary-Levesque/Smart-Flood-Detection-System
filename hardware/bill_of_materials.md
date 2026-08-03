# Bill of Materials

This list covers every item needed for the working prototype. Exact vendor substitutions are acceptable if the electrical ratings and pin compatibility match.

| Item | Example / Specification | Qty | Used For | Notes |
|---|---:|---:|---|---|
| ESP32 development board | ESP32-WROOM-32 DevKit style board, 3.3 V GPIO | 1 | Main controller and WiFi | Must expose GPIO34, GPIO25, GPIO26, 3V3, GND, and USB serial |
| Analog water-level sensor | Resistive analog water sensor module, 3.3 V compatible | 1 | Water detection input | Connect analog output to GPIO34 / ADC1_CH6 |
| Active buzzer | 3.3 V or 5 V active buzzer module | 1 | Local audible alarm | Use a transistor driver if buzzer current exceeds ESP32 GPIO rating |
| Status LED | 3 mm or 5 mm LED | 1 | Visual alarm/status indicator | Any standard red LED is fine |
| LED resistor | 220 ohm to 330 ohm, 1/4 W | 1 | LED current limiting | Required in series with the LED |
| Buzzer driver transistor | 2N2222, PN2222, or logic-level MOSFET | 1 | Safe buzzer switching | Recommended if buzzer is not a low-current GPIO-safe module |
| Buzzer flyback diode | 1N4148 or 1N400x | 1 | Inductive transient protection | Required for magnetic buzzers or relay-style loads |
| Buzzer base/gate resistor | 1 kOhm to 4.7 kOhm | 1 | Transistor input limiting | Use with BJT or MOSFET gate as appropriate |
| Pull-up/diagnostic button | Momentary push button | 1 | Calibration/event-log trigger on GPIO0 | GPIO0 is also a boot strap pin; do not hold low while flashing |
| Jumper wires | Male-to-male / male-to-female Dupont wires | 10-15 | Prototype wiring | Use short runs for ADC signal stability |
| Breadboard or perfboard | Solderless breadboard for prototype, perfboard for final | 1 | Mounting and interconnect | Perfboard is preferred for deployment |
| USB cable | Data-capable USB cable matching ESP32 board | 1 | Power, flashing, serial monitor | Charge-only cables will not flash firmware |
| 5 V USB wall adapter | 5 V, at least 1 A | 1 | Primary power | Use a reputable adapter for continuous operation |
| USB battery backup / UPS | 5 V USB UPS or power bank with pass-through support | 1 | Outage resilience | Optional but recommended for basement flood monitoring |
| Enclosure | Plastic project box, preferably gasketed/IP-rated | 1 | Environmental protection | Keep sensor exposed while protecting electronics |
| Cable gland or grommet | Sized for sensor/power cable | 1-2 | Enclosure strain relief | Helps keep moisture away from electronics |
| Mounting hardware | Screws, standoffs, adhesive pads, zip ties | Assorted | Secure installation | Avoid placing ESP32 directly on damp surfaces |
| Labels | Small adhesive labels or heat-shrink labels | Assorted | Maintenance | Label power, sensor, buzzer, and diagnostic button connections |

## Final Assembly Notes

- Power the ESP32 through USB or the board's regulated 5 V input, not directly from 5 V into a 3.3 V GPIO.
- Keep the water sensor low and exposed, but mount the ESP32, buzzer electronics, and power wiring above expected water level.
- For long sensor leads, route the signal wire away from power cables and consider twisted pair or shielded cable for the analog signal and ground.
- Do not deploy a bare breadboard in a damp basement. Move the circuit to perfboard or a PCB and use strain relief before long-term use.

