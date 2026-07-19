#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Starts the alarm task, which listens on event_queue and drives the
// local buzzer/LED. This runs independently of WiFi connectivity, so
// the physical alarm still fires even if the network is down.
void alarm_task_start(QueueHandle_t event_queue);
