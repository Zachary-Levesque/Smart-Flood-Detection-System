# Board Notes

Board size is 50 mm by 40 mm.

The ESP32 C3 Mini plugs in through the J1 header or connects with short wires. The Grove sensor plugs into J2. The buzzer plugs into J3. The LED and resistor are mounted on the board.

This PCB is meant to replace the breadboard wiring for the current project.

Before ordering, confirm these points:

1. Your ESP32 C3 Mini exposes GPIO4, GPIO5, GPIO6, GPIO7, 3V3, and GND.
2. Your Grove cable pin order matches SCL, SDA, 3V3, and GND.
3. Your buzzer module can be driven from GPIO6.
4. Your LED uses a 220 ohm resistor.
