# Test And Validation Plan

Record the date, firmware commit, threshold value, and result for each test before deployment.

<table>
<tr><th>Test</th><th>Procedure</th><th>Expected Result</th><th>Pass Or Fail</th></tr>
<tr><td>Dry boot</td><td>Power the unit with a dry sensor</td><td>LED and buzzer remain off. Serial logs startup</td><td></td></tr>
<tr><td>Calibration readings</td><td>Hold GPIO0 low with sensor dry, damp, wet, and submerged</td><td>Serial monitor prints distinct raw ADC ranges</td><td></td></tr>
<tr><td>Threshold trigger</td><td>Wet the sensor above the calibrated threshold for 3 samples</td><td>Buzzer and LED turn on after debounce delay</td><td></td></tr>
<tr><td>Recovery</td><td>Dry the sensor below threshold</td><td>Buzzer and LED turn off after readings return below threshold</td><td></td></tr>
<tr><td>Splash rejection</td><td>Briefly touch the sensor for less than 3 samples</td><td>Alarm does not latch from a transient splash</td><td></td></tr>
<tr><td>WiFi unavailable</td><td>Boot with router off or wrong credentials</td><td>Local alarm still triggers. Remote alert is skipped</td><td></td></tr>
<tr><td>WiFi reconnect</td><td>Start with WiFi unavailable, then restore router</td><td>Firmware reconnects automatically and later remote alerts work</td><td></td></tr>
<tr><td>Telegram alert</td><td>Configure bot token and chat ID, then wet sensor</td><td>Phone receives Telegram message</td><td></td></tr>
<tr><td>Event log dump</td><td>Create events, hold GPIO0 low while running</td><td>Serial monitor prints recent event ring buffer</td><td></td></tr>
<tr><td>Power interruption</td><td>Unplug and reconnect power, or switch to USB backup</td><td>Unit restarts and resumes monitoring</td><td></td></tr>
<tr><td>24 hour soak test</td><td>Run dry for 24 hours in target location</td><td>No false alarms, resets, or overheating</td><td></td></tr>
</table>

## Calibration Worksheet

<table>
<tr><th>Sensor State</th><th>Raw ADC Reading 1</th><th>Reading 2</th><th>Reading 3</th><th>Notes</th></tr>
<tr><td>Dry</td><td></td><td></td><td></td><td></td></tr>
<tr><td>Humid or damp nearby</td><td></td><td></td><td></td><td></td></tr>
<tr><td>Light water contact</td><td></td><td></td><td></td><td></td></tr>
<tr><td>Submerged or alarm condition</td><td></td><td></td><td></td><td></td></tr>
</table>

Set `WATER_THRESHOLD_RAW` in `firmware/main/app_config.h` between the highest safe non flood value and the lowest value that must trigger the alarm.
