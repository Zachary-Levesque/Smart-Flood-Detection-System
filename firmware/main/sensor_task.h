#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Starts the sensor polling task. Debounced state changes are pushed to
// alarm_event_queue first, then optionally to alert_event_queue.
void sensor_task_start(QueueHandle_t alarm_event_queue, QueueHandle_t alert_event_queue);
