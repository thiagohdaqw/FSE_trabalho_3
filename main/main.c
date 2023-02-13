#include <stdio.h>
#include <string.h>

#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "motor.h"
#include "states.h"
#include "led_rgb.h"
#include "joystick.h"
#include "infrared.h"
#include "telemetry.h"
#include "temperature.h"

#define TAG "APP"

State state;

void nvs_init() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void app_main(void) {
    nvs_init();

    telemetry_init(&state);
    led_rgb_init();

#ifdef CONFIG_CAR
    ESP_LOGI(TAG, "initiating as car controller");
    xTaskCreate(&motor_control, "motor", 2048, &state.joystick, 1, NULL);
    xTaskCreate(infrared_rx_task, "Infrared RX", 4096, &state.joystick, configMAX_PRIORITIES, NULL);
#endif

#ifdef CONFIG_JOYSTICK
    ESP_LOGI(TAG, "initiating as joystick controller");
    // xTaskCreate(&infrared_tx_task, "Infrared TX", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(&joystick_read, "Joystick", 2048, &state.joystick, 1, NULL);
#endif
}
