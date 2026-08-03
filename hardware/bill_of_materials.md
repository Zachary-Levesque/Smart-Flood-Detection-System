# Bill Of Materials

This list covers every item needed for the working prototype. Exact vendor substitutions are acceptable if the electrical ratings and pin compatibility match.

<table>
<tr><th>Item</th><th>Example Or Specification</th><th>Qty</th><th>Used For</th><th>Notes</th></tr>
<tr><td>ESP32 development board</td><td>ESP32 WROOM 32 DevKit style board, 3.3 V GPIO</td><td>1</td><td>Main controller and WiFi</td><td>Must expose GPIO34, GPIO25, GPIO26, 3V3, GND, and USB serial</td></tr>
<tr><td>Analog water level sensor</td><td>Resistive analog water sensor module, 3.3 V compatible</td><td>1</td><td>Water detection input</td><td>Connect analog output to GPIO34 / ADC1_CH6</td></tr>
<tr><td>Active buzzer</td><td>3.3 V or 5 V active buzzer module</td><td>1</td><td>Local audible alarm</td><td>Use a transistor driver if buzzer current exceeds ESP32 GPIO rating</td></tr>
<tr><td>Status LED</td><td>3 mm or 5 mm LED</td><td>1</td><td>Visual alarm and status indicator</td><td>Any standard red LED is fine</td></tr>
<tr><td>LED resistor</td><td>220 ohm to 330 ohm, 1/4 W</td><td>1</td><td>LED current limiting</td><td>Required in series with the LED</td></tr>
<tr><td>Buzzer driver transistor</td><td>2N2222, PN2222, or logic level MOSFET</td><td>1</td><td>Safe buzzer switching</td><td>Recommended if buzzer is not a low current GPIO safe module</td></tr>
<tr><td>Buzzer flyback diode</td><td>1N4148 or 1N400x</td><td>1</td><td>Transient protection</td><td>Required for magnetic buzzers or relay style loads</td></tr>
<tr><td>Buzzer base or gate resistor</td><td>1 kOhm to 4.7 kOhm</td><td>1</td><td>Transistor input limiting</td><td>Use with BJT or MOSFET gate as appropriate</td></tr>
<tr><td>Diagnostic button</td><td>Momentary push button</td><td>1</td><td>Calibration and event log trigger on GPIO0</td><td>GPIO0 is also a boot strap pin. Do not hold low while flashing</td></tr>
<tr><td>Jumper wires</td><td>Dupont wires</td><td>10 to 15</td><td>Prototype wiring</td><td>Use short runs for ADC signal stability</td></tr>
<tr><td>Breadboard or perfboard</td><td>Solderless breadboard for prototype, perfboard for final build</td><td>1</td><td>Mounting and interconnect</td><td>Perfboard is preferred for deployment</td></tr>
<tr><td>USB cable</td><td>Data capable USB cable matching ESP32 board</td><td>1</td><td>Power, flashing, serial monitor</td><td>Charge only cables will not flash firmware</td></tr>
<tr><td>5 V USB wall adapter</td><td>5 V, at least 1 A</td><td>1</td><td>Primary power</td><td>Use a reputable adapter for continuous operation</td></tr>
<tr><td>USB battery backup or UPS</td><td>5 V USB UPS or power bank with pass through support</td><td>1</td><td>Outage resilience</td><td>Optional but recommended for basement flood monitoring</td></tr>
<tr><td>Enclosure</td><td>Plastic project box, preferably gasketed or IP rated</td><td>1</td><td>Environmental protection</td><td>Keep sensor exposed while protecting electronics</td></tr>
<tr><td>Cable gland or grommet</td><td>Sized for sensor and power cable</td><td>1 to 2</td><td>Enclosure strain relief</td><td>Helps keep moisture away from electronics</td></tr>
<tr><td>Mounting hardware</td><td>Screws, standoffs, adhesive pads, zip ties</td><td>Assorted</td><td>Secure installation</td><td>Avoid placing ESP32 directly on damp surfaces</td></tr>
<tr><td>Labels</td><td>Small adhesive labels or heat shrink labels</td><td>Assorted</td><td>Maintenance</td><td>Label power, sensor, buzzer, and diagnostic button connections</td></tr>
</table>

## Final Assembly Notes

1. Power the ESP32 through USB or the board regulated 5 V input, not directly from 5 V into a 3.3 V GPIO.
2. Keep the water sensor low and exposed, but mount the ESP32, buzzer electronics, and power wiring above expected water level.
3. For long sensor leads, route the signal wire away from power cables and consider twisted pair or shielded cable for the analog signal and ground.
4. Do not deploy a bare breadboard in a damp basement. Move the circuit to perfboard or a PCB and use strain relief before long term use.
