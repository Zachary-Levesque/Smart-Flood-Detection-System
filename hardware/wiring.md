# Wiring

## Pin Map

1. Sensor SDA to GPIO4.
2. Sensor SCL to GPIO5.
3. Sensor VCC to 3V3.
4. Sensor GND to GND.
5. Buzzer signal to GPIO6.
6. LED signal to GPIO7 through a 220 ohm resistor.
7. LED ground to GND.
8. USB cable to ESP32 C3 Mini for power, flashing, and serial monitor.

## Simple Build Notes

1. Connect all grounds together.
2. Keep sensor wires short if possible.
3. Put the sensor where water would appear first.
4. Mount the ESP32 higher than the sensor.
5. Test the sensor, buzzer, and LED before putting the project in the enclosure.
6. Use the carrier PCB in `hardware/PCB` instead of a breadboard.
