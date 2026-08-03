# PCB

This folder contains a simple carrier PCB for the Smart Flood Detection System.

## What The PCB Does

1. Connects the ESP32 C3 Mini to the Grove water level sensor.
2. Connects GPIO6 to the buzzer.
3. Connects GPIO7 to the LED through a 220 ohm resistor.
4. Keeps the circuit cleaner than a breadboard.

## Connectors

1. J1 is the ESP32 C3 Mini signal header.
2. J2 is the Grove sensor connector.
3. J3 is the buzzer connector.
4. R1 is the 220 ohm LED resistor.
5. D1 is the status LED.

## J1 Pin Order

1. 3V3
2. GND
3. GPIO4 SDA
4. GPIO5 SCL
5. GPIO6 buzzer
6. GPIO7 LED

## J2 Grove Pin Order

1. SCL
2. SDA
3. 3V3
4. GND

## Files

1. `gerbers` contains the fabrication files.
2. `board_notes.md` explains the layout in plain language.

Check the exact ESP32 C3 Mini board dimensions before ordering. This is a simple carrier board, not a custom ESP32 module layout.
