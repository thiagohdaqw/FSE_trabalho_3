#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"

#include "adc.h"
#include "dht11.h"
#include "states.h"
#include "temperature.h"

#define TAG "TEMPERATURE"

#define TEMPERATURE_GPIO 25
#define TEMPERATURE_MAX_TRIES 10

void temperature_init() { DHT11_init(TEMPERATURE_GPIO); }

void temperature_read(State *state) {
    static struct dht11_reading dht;

    for (int i = 0; i < TEMPERATURE_MAX_TRIES; i++) {
        dht = DHT11_read();
        if (dht.status == DHT11_OK) {
            ESP_LOGI(TAG, "Temperature %d, Humidity: %d", dht.temperature, dht.humidity);
            state->temperature = dht.temperature;
            state->humidity = dht.humidity;
            return;
        } else {
            ESP_LOGE(TAG, "Error reading temperature. Trying again...");
        }
    }
    ESP_LOGE(TAG, "Error reading temperature. Max tries(%d) reached", TEMPERATURE_MAX_TRIES);
}
