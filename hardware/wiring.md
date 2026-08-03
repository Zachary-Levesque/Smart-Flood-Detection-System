# Wiring And Pin Map

<table>
<tr><th>Function</th><th>ESP32 Pin</th><th>Connected Part</th><th>Wiring</th></tr>
<tr><td>Water sensor analog signal</td><td>GPIO34 / ADC1_CH6</td><td>Sensor AO</td><td>Sensor AO to GPIO34</td></tr>
<tr><td>Water sensor power</td><td>3V3</td><td>Sensor VCC</td><td>Sensor VCC to ESP32 3V3</td></tr>
<tr><td>Water sensor ground</td><td>GND</td><td>Sensor GND</td><td>Sensor GND to ESP32 GND</td></tr>
<tr><td>Buzzer control</td><td>GPIO25</td><td>Active buzzer or transistor driver input</td><td>GPIO25 to buzzer module input, or through resistor to transistor base or gate</td></tr>
<tr><td>Buzzer power</td><td>3V3 or 5V</td><td>Buzzer VCC</td><td>Match buzzer rating and share ground with ESP32</td></tr>
<tr><td>Status LED</td><td>GPIO26</td><td>LED anode through resistor</td><td>GPIO26 to 220 ohm resistor to LED anode. LED cathode to GND</td></tr>
<tr><td>Calibration and event log button</td><td>GPIO0</td><td>Momentary button</td><td>GPIO0 to button, other side to GND. Internal pullup enabled</td></tr>
<tr><td>USB serial and power</td><td>USB port</td><td>Computer or USB adapter</td><td>Used for flashing, monitoring, and 5 V board power</td></tr>
</table>

## Buzzer Driver Option

Use this option if the buzzer draws more current than an ESP32 GPIO can safely supply.

<table>
<tr><th>Driver Node</th><th>Connection</th></tr>
<tr><td>GPIO25</td><td>1 kOhm to 4.7 kOhm resistor into transistor base or gate</td></tr>
<tr><td>Transistor emitter or source</td><td>GND</td></tr>
<tr><td>Transistor collector or drain</td><td>Buzzer negative</td></tr>
<tr><td>Buzzer positive</td><td>3V3 or 5V supply matching buzzer rating</td></tr>
<tr><td>Diode</td><td>Across buzzer if inductive, cathode to positive supply, anode to transistor side</td></tr>
<tr><td>Grounds</td><td>ESP32 GND and buzzer supply GND tied together</td></tr>
</table>

## Deployment Placement

1. Mount the sensor at the lowest point where water should trigger an alert.
2. Mount the electronics above likely water height.
3. Keep mains power connections away from the floor and use a GFCI protected outlet.
4. Test GPIO0 behavior after installation. Holding GPIO0 low during reset can enter bootloader mode.
