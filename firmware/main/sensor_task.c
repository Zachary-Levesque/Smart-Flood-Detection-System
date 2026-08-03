#include "sensor_task.h"
#include "app_config.h"
#include "event_log.h"
#include "sensor_logic.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

static const char *TAG = "sensor_task";

typedef struct {
    QueueHandle_t alarm_event_queue;
    QueueHandle_t alert_event_queue;
} sensor_task_config_t;

static esp_err_t read_sensor_section(uint8_t address, uint8_t *data, size_t length)
{
    return i2c_master_read_from_device(I2C_PORT,
                                       address,
                                       data,
                                       length,
                                       pdMS_TO_TICKS(100));
}

static int count_touched_sections(const uint8_t *data, size_t length)
{
    int touched = 0;
    for (size_t i = 0; i < length; i++) {
        if (data[i] > GROVE_WATER_TOUCH_THRESHOLD) {
            touched++;
        }
    }
    return touched;
}

static int read_water_level_percent(void)
{
    uint8_t low_data[8] = {0};
    uint8_t high_data[12] = {0};

    esp_err_t low_err = read_sensor_section(GROVE_WATER_LOW_ADDR, low_data, sizeof(low_data));
    esp_err_t high_err = read_sensor_section(GROVE_WATER_HIGH_ADDR, high_data, sizeof(high_data));
    if (low_err != ESP_OK || high_err != ESP_OK) {
        ESP_LOGW(TAG, "Water sensor I2C read failed low=%s high=%s",
                 esp_err_to_name(low_err),
                 esp_err_to_name(high_err));
        return 0;
    }

    int touched = count_touched_sections(low_data, sizeof(low_data)) +
                  count_touched_sections(high_data, sizeof(high_data));
    return touched * 5;
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

static void configure_i2c(void)
{
    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, config.mode, 0, 0, 0));
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

    configure_i2c();
    configure_calibration_gpio();

    sensor_debounce_state_t debounce;
    sensor_debounce_init(&debounce);

    ESP_LOGI(TAG, "Sensor task started, polling every %d ms", SENSOR_POLL_INTERVAL_MS);

    while (1) {
        int level_percent = read_water_level_percent();

        if (calibration_mode_enabled()) {
            ESP_LOGI(TAG, "Calibration water level: %d percent", level_percent);
        }

        bool changed = false;
        system_event_t current_state = sensor_debounce_update(&debounce,
                                                              level_percent,
                                                              WATER_LEVEL_THRESHOLD_PERCENT,
                                                              SENSOR_DEBOUNCE_SAMPLES,
                                                              &changed);

        if (changed) {
            ESP_LOGI(TAG, "Water state changed: %s (level=%d percent)",
                     system_event_to_string(current_state),
                     level_percent);

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
