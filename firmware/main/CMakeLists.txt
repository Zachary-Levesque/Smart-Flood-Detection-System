idf_component_register(
    SRCS
        "main.c"
        "sensor_task.c"
        "alarm_task.c"
        "wifi_manager.c"
    INCLUDE_DIRS "."
    REQUIRES
        esp_wifi
        esp_event
        nvs_flash
        driver
        esp_timer
)
