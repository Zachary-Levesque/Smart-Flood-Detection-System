#include "sensor_task.h"
#include "app_config.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"

static const char *TAG = "sensor_task";

// Reads the raw ADC value from the water sensor.
static int read_water_sensor_raw(void)
{
    return adc1_get_raw(WATER_SENSOR_ADC_CHANNEL);
}

static void sensor_task(void *pvParameters)
{
    QueueHandle_t event_queue = (QueueHandle_t) pvParameters;

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(WATER_SENSOR_ADC_CHANNEL, ADC_ATTEN_DB_11);

    system_event_t last_reported_state = EVENT_WATER_NORMAL;
    int consecutive_wet_samples = 0;

    ESP_LOGI(TAG, "Sensor task started, polling every %d ms", SENSOR_POLL_INTERVAL_MS);

    while (1) {
        int raw = read_water_sensor_raw();
        bool is_wet = raw >= WATER_THRESHOLD_RAW;

        // Debounce: require several consecutive "wet" samples before
        // treating this as a real event, to avoid false positives from
        // noise, splashes, or condensation.
        if (is_wet) {
            consecutive_wet_samples++;
        } else {
            consecutive_wet_samples = 0;
        }

        system_event_t current_state =
            (consecutive_wet_samples >= SENSOR_DEBOUNCE_SAMPLES)
                ? EVENT_WATER_DETECTED
                : EVENT_WATER_NORMAL;

        if (current_state != last_reported_state) {
            ESP_LOGI(TAG, "Water state changed: %s (raw=%d)",
                     current_state == EVENT_WATER_DETECTED ? "DETECTED" : "NORMAL",
                     raw);

            if (xQueueSend(event_queue, &current_state, pdMS_TO_TICKS(100)) != pdTRUE) {
                ESP_LOGW(TAG, "Failed to enqueue event (queue full)");
            }
            last_reported_state = current_state;
        }

        vTaskDelay(pdMS_TO_TICKS(SENSOR_POLL_INTERVAL_MS));
    }
}

void sensor_task_start(QueueHandle_t event_queue)
{
    xTaskCreate(sensor_task, "sensor_task", 4096, (void *) event_queue, 5, NULL);
}
