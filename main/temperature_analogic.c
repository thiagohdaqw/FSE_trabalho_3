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

const static char *TAG = "TEMPERATURE ANALOGIC";

#define TEMP_ANALOGIC_ADC_UNIT              ADC_UNIT_2
#define TEMP_ANALOGIC_CHANNEL               ADC_CHANNEL_4
#define TEMP_ANALOGIC_ADC_ATTEN             ADC_ATTEN_DB_11
#define TEMP_ANALOGIC_ADC_BITWIDTH          ADC_BITWIDTH_DEFAULT


static int temperature;

void read_temperature_analogic(void *params)
{
    Unit unit = adc_init_unit(TEMP_ANALOGIC_ADC_UNIT);

    adc_init_channel(TEMP_ANALOGIC_CHANNEL, TEMP_ANALOGIC_ADC_BITWIDTH, TEMP_ANALOGIC_ADC_ATTEN, &unit);

    adc_init_calibration(&unit, TEMP_ANALOGIC_ADC_ATTEN, TEMP_ANALOGIC_ADC_BITWIDTH);

    while (true)
    {
        temperature = adc_read(&unit, TEMP_ANALOGIC_CHANNEL);

        ESP_LOGI(TAG, "Temperature %d", temperature);

        vTaskDelay(pdMS_TO_TICKS(2 * 1000));
    }

    adc_deinit(unit);
}
