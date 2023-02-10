#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "joystick.h"
#include "temperature_analogic.h"
#include "temperature_dht11.h"

const static char *TAG = "APP";

void app_main(void)
{

#ifdef CONFIG_CAR
    ESP_LOGI(TAG, "Initing as car controller");
#endif

#ifdef CONFIG_JOYSTICK
    ESP_LOGI(TAG, "Initing as joystick controller");
    xTaskCreate(&read_joystick, "Joystick", 2048, NULL, 1, NULL);
    xTaskCreate(&read_temperature_analogic, "Analogic Temperature", 2048, NULL, 1, NULL);
    xTaskCreate(&read_temperature_dht11, "DHT11 Temperature", 2048, NULL, 1, NULL);
#endif

#ifdef CONFIG_MONITOR
    ESP_LOGI(TAG, "Initing as monitor controller");
#endif
}
