#include "wifi_manager.h"
#include "app_config.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

static const char *TAG = "wifi_manager";

static EventGroupHandle_t s_wifi_event_group;
static const int WIFI_CONNECTED_BIT = BIT0;
static int s_retry_count = 0;
static volatile bool s_connected = false;
static volatile bool s_backoff_retry_pending = false;

static void reconnect_backoff_task(void *pvParameters)
{
    (void) pvParameters;

    while (1) {
        if (s_backoff_retry_pending && !s_connected) {
            ESP_LOGI(TAG, "Waiting %d ms before next WiFi reconnect attempt", WIFI_RETRY_BACKOFF_MS);
            vTaskDelay(pdMS_TO_TICKS(WIFI_RETRY_BACKOFF_MS));

            if (!s_connected) {
                s_retry_count = 0;
                s_backoff_retry_pending = false;
                ESP_LOGI(TAG, "Retrying WiFi connection after backoff");
                esp_wifi_connect();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void event_handler(void *arg, esp_event_base_t event_base,
                           int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        s_connected = false;
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);

        // Reconnection logic: this is the core of the "fail-safe"
        // connectivity requirement. We never give up permanently —
        // a flood detector that stops retrying after N failures is
        // a flood detector that can silently go offline for good.
        if (s_retry_count < WIFI_MAX_RETRY) {
            esp_wifi_connect();
            s_retry_count++;
            ESP_LOGW(TAG, "WiFi disconnected, retrying (%d/%d)", s_retry_count, WIFI_MAX_RETRY);
        } else {
            s_backoff_retry_pending = true;
            ESP_LOGW(TAG, "Max immediate retries reached, reconnect will continue after backoff");
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        s_retry_count = 0;
        s_backoff_retry_pending = false;
        s_connected = true;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        ESP_LOGI(TAG, "Connected, got IP");
    }
}

void wifi_manager_start(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    xTaskCreate(reconnect_backoff_task, "wifi_reconnect", 3072, NULL, 3, NULL);

    ESP_LOGI(TAG, "WiFi manager started, connecting to SSID: %s", WIFI_SSID);
}

bool wifi_manager_is_connected(void)
{
    return s_connected;
}
