#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Starts the sensor polling task. Events are pushed to event_queue
// whenever the water state changes (debounced).
void sensor_task_start(QueueHandle_t event_queue);
