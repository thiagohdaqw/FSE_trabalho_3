#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#include "adc.h"
#include "dht11.h"
#include "temperature_dht11.h"

const static char *TAG = "TEMPERATURE DHT11";

#define TEMP_DHT11_GPIO 25

void read_temperature_dht11(void *params)
{
    DHT11_init(TEMP_DHT11_GPIO);
    struct dht11_reading dht;

    while (true)
    {
        dht = DHT11_read();

        if (dht.status == DHT11_OK)
        {
            ESP_LOGI(TAG, "Temperature %d, Humidity: %d", dht.temperature, dht.humidity);
        }
        else
        {
            ESP_LOGE(TAG, "Error reading temperature");
        }

        vTaskDelay(pdMS_TO_TICKS(1 * 1000));
    }
}
