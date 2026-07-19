#include "event_log.h"

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"

static const char *TAG = "event_log";

typedef struct {
    int64_t timestamp_ms;
    system_event_t event;
} event_log_entry_t;

static event_log_entry_t s_entries[EVENT_LOG_CAPACITY];
static size_t s_next_index;
static size_t s_count;
static portMUX_TYPE s_lock = portMUX_INITIALIZER_UNLOCKED;

const char *system_event_to_string(system_event_t event)
{
    switch (event) {
    case EVENT_WATER_NORMAL:
        return "EVENT_WATER_NORMAL";
    case EVENT_WATER_DETECTED:
        return "EVENT_WATER_DETECTED";
    default:
        return "EVENT_UNKNOWN";
    }
}

void event_log_init(void)
{
    portENTER_CRITICAL(&s_lock);
    memset(s_entries, 0, sizeof(s_entries));
    s_next_index = 0;
    s_count = 0;
    portEXIT_CRITICAL(&s_lock);
}

static bool dump_requested(void)
{
#if EVENT_LOG_DUMP_ON_BOOT
    return true;
#elif EVENT_LOG_DUMP_GPIO_ENABLED
    return gpio_get_level(EVENT_LOG_DUMP_GPIO) == 0;
#else
    return false;
#endif
}

static void event_log_dump_task(void *pvParameters)
{
    (void) pvParameters;

#if EVENT_LOG_DUMP_GPIO_ENABLED
    gpio_reset_pin(EVENT_LOG_DUMP_GPIO);
    gpio_set_direction(EVENT_LOG_DUMP_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(EVENT_LOG_DUMP_GPIO, GPIO_PULLUP_ONLY);
#endif

    bool dumped_while_pressed = false;
    while (1) {
        bool requested = dump_requested();
        if (requested && !dumped_while_pressed) {
            event_log_dump_to_serial();
            dumped_while_pressed = true;
        } else if (!requested) {
            dumped_while_pressed = false;
        }

        vTaskDelay(pdMS_TO_TICKS(EVENT_LOG_DUMP_POLL_MS));
    }
}

void event_log_dump_task_start(void)
{
    xTaskCreate(event_log_dump_task, "event_log_dump", 2048, NULL, 2, NULL);
}

void event_log_record(system_event_t event)
{
    portENTER_CRITICAL(&s_lock);
    s_entries[s_next_index].timestamp_ms = esp_timer_get_time() / 1000;
    s_entries[s_next_index].event = event;
    s_next_index = (s_next_index + 1) % EVENT_LOG_CAPACITY;
    if (s_count < EVENT_LOG_CAPACITY) {
        s_count++;
    }
    portEXIT_CRITICAL(&s_lock);
}

void event_log_dump_to_serial(void)
{
    event_log_entry_t snapshot[EVENT_LOG_CAPACITY];
    size_t count;
    size_t start;

    portENTER_CRITICAL(&s_lock);
    count = s_count;
    start = (s_next_index + EVENT_LOG_CAPACITY - s_count) % EVENT_LOG_CAPACITY;
    for (size_t i = 0; i < count; i++) {
        snapshot[i] = s_entries[(start + i) % EVENT_LOG_CAPACITY];
    }
    portEXIT_CRITICAL(&s_lock);

    ESP_LOGI(TAG, "Event log dump: %u entries", (unsigned int) count);
    for (size_t i = 0; i < count; i++) {
        ESP_LOGI(TAG, "[%02u] t=%" PRId64 " ms event=%s",
                 (unsigned int) i,
                 snapshot[i].timestamp_ms,
                 system_event_to_string(snapshot[i].event));
    }
}
