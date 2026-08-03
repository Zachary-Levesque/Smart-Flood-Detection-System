# Wiring

## Pin Map

1. Water sensor signal to GPIO34.
2. Water sensor VCC to 3V3.
3. Water sensor GND to GND.
4. Buzzer signal to GPIO25.
5. LED signal to GPIO26 through a 220 ohm resistor.
6. LED ground to GND.
7. USB cable to ESP32 for power, flashing, and serial monitor.

## Simple Build Notes

1. Connect all grounds together.
2. Keep sensor wires short if possible.
3. Put the sensor where water would appear first.
4. Mount the ESP32 higher than the sensor.
5. Test the buzzer and LED before putting the project in the enclosure.
