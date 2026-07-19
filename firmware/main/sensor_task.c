#include "sensor_task.h"
#include "app_config.h"
#include "event_log.h"
#include "sensor_logic.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

static const char *TAG = "sensor_task";

typedef struct {
    QueueHandle_t alarm_event_queue;
    QueueHandle_t alert_event_queue;
} sensor_task_config_t;

// Reads the raw ADC value from the water sensor.
static int read_water_sensor_raw(void)
{
    return adc1_get_raw(WATER_SENSOR_ADC_CHANNEL);
}

static bool calibration_mode_enabled(void)
{
#if SENSOR_CALIBRATION_FORCE
    return true;
#else
    return gpio_get_level(CALIBRATION_MODE_GPIO) == 0;
#endif
}

static void configure_calibration_gpio(void)
{
    gpio_reset_pin(CALIBRATION_MODE_GPIO);
    gpio_set_direction(CALIBRATION_MODE_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(CALIBRATION_MODE_GPIO, GPIO_PULLUP_ONLY);
}

static void publish_event(QueueHandle_t queue, system_event_t event, const char *queue_name)
{
    if (queue == NULL) {
        return;
    }

    if (xQueueSend(queue, &event, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGW(TAG, "Failed to enqueue event for %s (queue full)", queue_name);
    }
}

static void sensor_task(void *pvParameters)
{
    sensor_task_config_t *config = (sensor_task_config_t *) pvParameters;
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(WATER_SENSOR_ADC_CHANNEL, ADC_ATTEN_DB_11);
    configure_calibration_gpio();

    sensor_debounce_state_t debounce;
    sensor_debounce_init(&debounce);

    ESP_LOGI(TAG, "Sensor task started, polling every %d ms", SENSOR_POLL_INTERVAL_MS);

    while (1) {
        int raw = read_water_sensor_raw();

        if (calibration_mode_enabled()) {
            ESP_LOGI(TAG, "Calibration raw ADC reading: %d", raw);
        }

        bool changed = false;
        system_event_t current_state = sensor_debounce_update(&debounce,
                                                              raw,
                                                              WATER_THRESHOLD_RAW,
                                                              SENSOR_DEBOUNCE_SAMPLES,
                                                              &changed);

        if (changed) {
            ESP_LOGI(TAG, "Water state changed: %s (raw=%d)",
                     system_event_to_string(current_state),
                     raw);

            event_log_record(current_state);
            publish_event(config->alarm_event_queue, current_state, "alarm");
            publish_event(config->alert_event_queue, current_state, "alert");
        }

        ESP_ERROR_CHECK(esp_task_wdt_reset());
        vTaskDelay(pdMS_TO_TICKS(SENSOR_POLL_INTERVAL_MS));
    }
}

void sensor_task_start(QueueHandle_t alarm_event_queue, QueueHandle_t alert_event_queue)
{
    static sensor_task_config_t config;
    config.alarm_event_queue = alarm_event_queue;
    config.alert_event_queue = alert_event_queue;

    xTaskCreate(sensor_task, "sensor_task", 4096, (void *) &config, 5, NULL);
}
