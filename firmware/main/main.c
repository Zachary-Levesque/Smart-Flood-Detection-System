#include "app_config.h"
#include "alert_task.h"
#include "sensor_task.h"
#include "alarm_task.h"
#include "event_log.h"
#include "wifi_manager.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

static void configure_task_watchdog(void)
{
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = TASK_WATCHDOG_TIMEOUT_MS,
        .idle_core_mask = 0,
        .trigger_panic = true,
    };

    esp_err_t err = esp_task_wdt_init(&wdt_config);
    if (err == ESP_ERR_INVALID_STATE) {
        ESP_LOGW("main", "Task watchdog already initialized");
    } else {
        ESP_ERROR_CHECK(err);
    }
}

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "Smart Flood Detection System starting up");
    event_log_init();
    configure_task_watchdog();

    // Dedicated alarm queue: sensor_task publishes local-alarm events here
    // first. Remote alerting has a separate queue so network work cannot
    // consume, gate, or delay the local alarm path.
    QueueHandle_t alarm_event_queue = xQueueCreate(10, sizeof(system_event_t));
    QueueHandle_t alert_event_queue = xQueueCreate(10, sizeof(system_event_t));
    if (alarm_event_queue == NULL || alert_event_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create event queues, halting");
        return;
    }

    // WiFi is started but never gated on — the local alarm and
    // sensor logic run regardless of network state. This is the
    // core fail-safe design principle of this project: connectivity
    // is a "nice to have" for remote alerting, not a dependency for
    // the physical safety mechanism.
    wifi_manager_start();

    event_log_dump_task_start();
    alarm_task_start(alarm_event_queue);
    alert_task_start(alert_event_queue);
    sensor_task_start(alarm_event_queue, alert_event_queue);
}
