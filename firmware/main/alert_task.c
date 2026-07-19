#include "alert_task.h"
#include "app_config.h"
#include "event_log.h"
#include "wifi_manager.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_crt_bundle.h"

static const char *TAG = "alert_task";

static bool telegram_configured(void)
{
    return strcmp(TELEGRAM_BOT_TOKEN, "CHANGE_ME") != 0 &&
           strcmp(TELEGRAM_CHAT_ID, "CHANGE_ME") != 0;
}

static void send_telegram_alert(void)
{
    if (!telegram_configured()) {
        ESP_LOGW(TAG, "Telegram alert skipped: TELEGRAM_BOT_TOKEN/TELEGRAM_CHAT_ID not configured");
        return;
    }

    char url[256];
    int url_len = snprintf(url, sizeof(url),
                           "https://api.telegram.org/bot%s/sendMessage",
                           TELEGRAM_BOT_TOKEN);
    if (url_len < 0 || url_len >= (int) sizeof(url)) {
        ESP_LOGE(TAG, "Telegram URL buffer too small");
        return;
    }

    char body[384];
    int body_len = snprintf(body, sizeof(body),
                            "{\"chat_id\":\"%s\",\"text\":\"%s\"}",
                            TELEGRAM_CHAT_ID,
                            TELEGRAM_ALERT_MESSAGE);
    if (body_len < 0 || body_len >= (int) sizeof(body)) {
        ESP_LOGE(TAG, "Telegram request body buffer too small");
        return;
    }

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .timeout_ms = 5000,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return;
    }

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, body, body_len);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Telegram alert sent, HTTP status=%d",
                 esp_http_client_get_status_code(client));
    } else {
        ESP_LOGW(TAG, "Telegram alert failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

static void alert_task(void *pvParameters)
{
    QueueHandle_t event_queue = (QueueHandle_t) pvParameters;
    system_event_t event;

    ESP_LOGI(TAG, "Remote alert task started");

    while (1) {
        if (xQueueReceive(event_queue, &event, portMAX_DELAY) == pdTRUE) {
            if (event != EVENT_WATER_DETECTED) {
                continue;
            }

            if (!wifi_manager_is_connected()) {
                ESP_LOGW(TAG, "Remote alert skipped: WiFi is not connected");
                continue;
            }

            ESP_LOGI(TAG, "Sending remote alert for %s", system_event_to_string(event));
            send_telegram_alert();
        }
    }
}

void alert_task_start(QueueHandle_t event_queue)
{
    xTaskCreate(alert_task, "alert_task", 6144, (void *) event_queue, 4, NULL);
}
