#include "alarm_task.h"
#include "app_config.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

static const char *TAG = "alarm_task";

static void configure_outputs(void)
{
    gpio_reset_pin(BUZZER_GPIO);
    gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);

    gpio_reset_pin(STATUS_LED_GPIO);
    gpio_set_direction(STATUS_LED_GPIO, GPIO_MODE_OUTPUT);
}

static void set_alarm_state(bool active)
{
    gpio_set_level(BUZZER_GPIO, active ? 1 : 0);
    gpio_set_level(STATUS_LED_GPIO, active ? 1 : 0);
}

// This task is intentionally simple and self-contained: it only
// depends on the event queue, not on WiFi/network state, so a local
// alarm still fires during a network outage.
static void alarm_task(void *pvParameters)
{
    QueueHandle_t event_queue = (QueueHandle_t) pvParameters;
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));

    configure_outputs();
    set_alarm_state(false);

    ESP_LOGI(TAG, "Alarm task started");

    system_event_t event;
    while (1) {
        if (xQueueReceive(event_queue, &event, pdMS_TO_TICKS(1000)) == pdTRUE) {
            bool active = (event == EVENT_WATER_DETECTED);
            ESP_LOGI(TAG, "Setting local alarm state: %s", active ? "ON" : "OFF");
            set_alarm_state(active);
        }
        ESP_ERROR_CHECK(esp_task_wdt_reset());
    }
}

void alarm_task_start(QueueHandle_t event_queue)
{
    xTaskCreate(alarm_task, "alarm_task", 2048, (void *) event_queue, 6, NULL);
}
