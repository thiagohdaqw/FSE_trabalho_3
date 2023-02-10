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

#include "joystick.h"
#include "adc.h"

const static char *TAG = "JOYSTICK";

#define JOYSTICK_ADC_UNIT ADC_UNIT_1
#define JOYSTICK_X_CHANNEL ADC_CHANNEL_3
#define JOYSTICK_Y_CHANNEL ADC_CHANNEL_0
#define JOYSTICK_ADC_ATTEN ADC_ATTEN_DB_11
#define JOYSTICK_ADC_BITWIDTH ADC_BITWIDTH_DEFAULT

static int adc_x, adc_x_last = 0;
static int adc_y, adc_y_last = 0;

void read_joystick(void *params)
{
    printf("hello\n");
    Unit unit = adc_init_unit(JOYSTICK_ADC_UNIT);

    adc_init_channel(JOYSTICK_X_CHANNEL, JOYSTICK_ADC_BITWIDTH, JOYSTICK_ADC_ATTEN, &unit);
    adc_init_channel(JOYSTICK_Y_CHANNEL, JOYSTICK_ADC_BITWIDTH, JOYSTICK_ADC_ATTEN, &unit);

    adc_init_calibration(&unit, JOYSTICK_ADC_ATTEN, JOYSTICK_ADC_BITWIDTH);

    while (true)
    {
        adc_x = adc_read(&unit, JOYSTICK_X_CHANNEL);
        adc_y = adc_read(&unit, JOYSTICK_Y_CHANNEL);

        // Media movel
        adc_x = (adc_x + adc_x_last) >> 1;
        adc_y = (adc_y + adc_y_last) >> 1;

        ESP_LOGI(TAG, "X: %d, Y: %d", adc_x - 2048 + 115, adc_y - 2048 + 135);

        adc_x_last = adc_x;
        adc_y_last = adc_y;

        vTaskDelay(pdMS_TO_TICKS(250));
    }

    adc_deinit(unit);
}
