# Wiring and Pin Map

| Function | ESP32 Pin | Connected Part | Wiring |
|---|---:|---|---|
| Water sensor analog signal | GPIO34 / ADC1_CH6 | Sensor AO | Sensor AO to GPIO34 |
| Water sensor power | 3V3 | Sensor VCC | Sensor VCC to ESP32 3V3 |
| Water sensor ground | GND | Sensor GND | Sensor GND to ESP32 GND |
| Buzzer control | GPIO25 | Active buzzer or transistor driver input | GPIO25 to buzzer module IN, or through resistor to transistor base/gate |
| Buzzer power | 3V3 or 5V | Buzzer VCC | Match buzzer rating; share ground with ESP32 |
| Status LED | GPIO26 | LED anode through resistor | GPIO26 -> 220 ohm resistor -> LED anode; LED cathode to GND |
| Calibration/event log button | GPIO0 | Momentary button | GPIO0 to button, other side to GND; internal pull-up enabled |
| USB serial/power | USB port | Computer or USB adapter | Used for flashing, monitoring, and 5 V board power |

## Buzzer Driver Option

Use this option if the buzzer draws more current than an ESP32 GPIO can safely supply.

| Driver Node | Connection |
|---|---|
| GPIO25 | 1 kOhm to 4.7 kOhm resistor into transistor base/gate |
| Transistor emitter/source | GND |
| Transistor collector/drain | Buzzer negative |
| Buzzer positive | 3V3 or 5V supply matching buzzer rating |
| Diode | Across buzzer if inductive, cathode to positive supply, anode to transistor side |
| Grounds | ESP32 GND and buzzer supply GND tied together |

## Deployment Placement

- Mount the sensor at the lowest point where water should trigger an alert.
- Mount the electronics above likely water height.
- Keep mains power connections away from the floor and use a GFCI-protected outlet.
- Test GPIO0 behavior after installation; holding GPIO0 low during reset can enter bootloader mode.

