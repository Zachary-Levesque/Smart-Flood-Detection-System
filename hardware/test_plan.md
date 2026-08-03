# Test and Validation Plan

Record the date, firmware commit, threshold value, and result for each test before deployment.

| Test | Procedure | Expected Result | Pass/Fail |
|---|---|---|---|
| Dry boot | Power the unit with a dry sensor | LED and buzzer remain off; serial logs startup | |
| Calibration readings | Hold GPIO0 low with sensor dry, damp, wet, and submerged | Serial monitor prints distinct raw ADC ranges | |
| Threshold trigger | Wet the sensor above the calibrated threshold for 3 samples | Buzzer and LED turn on after debounce delay | |
| Recovery | Dry the sensor below threshold | Buzzer and LED turn off after readings return below threshold | |
| Splash rejection | Briefly touch the sensor for less than 3 samples | Alarm does not latch from a transient splash | |
| WiFi unavailable | Boot with router off or wrong credentials | Local alarm still triggers; remote alert is skipped | |
| WiFi reconnect | Start with WiFi unavailable, then restore router | Firmware reconnects automatically and later remote alerts work | |
| Telegram alert | Configure bot token/chat ID and wet sensor | Phone receives Telegram message | |
| Event log dump | Create events, hold GPIO0 low while running | Serial monitor prints recent event ring buffer | |
| Power interruption | Unplug/replug or switch to USB backup | Unit restarts and resumes monitoring | |
| 24-hour soak test | Run dry for 24 hours in target location | No false alarms, resets, or overheating | |

## Calibration Worksheet

| Sensor State | Raw ADC Reading 1 | Reading 2 | Reading 3 | Notes |
|---|---:|---:|---:|---|
| Dry | | | | |
| Humid / damp nearby | | | | |
| Light water contact | | | | |
| Submerged / alarm condition | | | | |

Set `WATER_THRESHOLD_RAW` in `firmware/main/app_config.h` between the highest safe non-flood value and the lowest value that must trigger the alarm.

