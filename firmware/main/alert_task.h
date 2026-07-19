#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Starts the remote alert task. It consumes its own queue so network
// alerting cannot steal events from or block the local alarm path.
void alert_task_start(QueueHandle_t event_queue);
