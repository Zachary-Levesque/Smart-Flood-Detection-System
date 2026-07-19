#include "app_config.h"
#include "sensor_task.h"
#include "alarm_task.h"
#include "wifi_manager.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_log.h"

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "Smart Flood Detection System starting up");

    // Shared queue: sensor_task produces state-change events,
    // alarm_task consumes them. Decoupling these two via a queue
    // (rather than a shared flag) keeps each task's responsibility
    // isolated and makes the local alarm path independent of
    // anything else in the system.
    QueueHandle_t event_queue = xQueueCreate(10, sizeof(system_event_t));
    if (event_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create event queue, halting");
        return;
    }

    // WiFi is started but never gated on — the local alarm and
    // sensor logic run regardless of network state. This is the
    // core fail-safe design principle of this project: connectivity
    // is a "nice to have" for remote alerting, not a dependency for
    // the physical safety mechanism.
    wifi_manager_start();

    alarm_task_start(event_queue);
    sensor_task_start(event_queue);

    // TODO: add a networking task that watches the same event_queue
    // (or a second queue fed by sensor_task) and, when
    // wifi_manager_is_connected() is true, pushes an alert via
    // MQTT/HTTP to the notification backend.
}
