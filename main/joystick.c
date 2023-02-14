#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp32/rom/uart.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"

#include "adc.h"
#include "joystick.h"
#include "states.h"

const static char *TAG = "JOYSTICK";

#define JOYSTICK_ADC_UNIT ADC_UNIT_1
#define JOYSTICK_X_CHANNEL ADC_CHANNEL_3
#define JOYSTICK_Y_CHANNEL ADC_CHANNEL_6
#define JOYSTICK_SWITCH 23
#define JOYSTICK_SENSIBILITY 200
#define JOYSTICK_ADC_ATTEN ADC_ATTEN_DB_11
#define JOYSTICK_ADC_BITWIDTH ADC_BITWIDTH_DEFAULT

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void joystick_set_percent(Joystick *joystick, int x_percent, int y_percent) {
    if (abs(x_percent) > 0 && abs(x_percent) < 30) {
        x_percent = 30 * ((x_percent > 0) - (x_percent < 0));
    }
    if (abs(x_percent) > 100) {
        x_percent = 100 * ((x_percent > 0) - (x_percent < 0));
    }
    if (abs(y_percent) > 0 && abs(y_percent) < 30) {
        y_percent = 30 * ((y_percent > 0) - (y_percent < 0));
    }
    if (abs(y_percent) > 100) {
        y_percent = 100 * ((y_percent > 0) - (y_percent < 0));
    }

    joystick->x_percent = x_percent;
    joystick->y_percent = y_percent;

    ESP_LOGI(TAG, "X: %d percent, Y: %d percent", x_percent, y_percent);
}

void joystick_read(void *params) {
    Joystick *joystick = (Joystick *)params;

    int adc_x, adc_x_last = 0, x, x_percent;
    int adc_y, adc_y_last = 0, y, y_percent;

    Unit unit = adc_init_unit(JOYSTICK_ADC_UNIT);

    adc_init_channel(JOYSTICK_X_CHANNEL, JOYSTICK_ADC_BITWIDTH, JOYSTICK_ADC_ATTEN, &unit);
    adc_init_channel(JOYSTICK_Y_CHANNEL, JOYSTICK_ADC_BITWIDTH, JOYSTICK_ADC_ATTEN, &unit);

    gpio_set_direction(JOYSTICK_SWITCH, GPIO_MODE_INPUT);
    gpio_pulldown_dis(JOYSTICK_SWITCH);
    gpio_pullup_en(JOYSTICK_SWITCH);

    adc_init_calibration(&unit, JOYSTICK_ADC_ATTEN, JOYSTICK_ADC_BITWIDTH);

    while (true)
    {
        adc_x = adc_read(&unit, JOYSTICK_X_CHANNEL);
        adc_y = adc_read(&unit, JOYSTICK_Y_CHANNEL);

        adc_x = (adc_x + adc_x_last) >> 1;
        adc_y = (adc_y + adc_y_last) >> 1;
        adc_x_last = adc_x;
        adc_y_last = adc_y;

        x = adc_x - 2048 + 115;
        y = adc_y - 2048 + 135;

        if (x > JOYSTICK_SENSIBILITY) {
            x_percent = x * 100 / 2161;
        } else if (x < -JOYSTICK_SENSIBILITY) {
            x_percent = x * 100 / 1933;
        } else {
            x_percent = 0;
        }
        if (y > JOYSTICK_SENSIBILITY) {
            y_percent = y * 100 / 2181;
        } else if (y < -JOYSTICK_SENSIBILITY) {
            y_percent = y * 100 / 1913;
        } else {
            y_percent = 0;
        }

        joystick_set_percent(joystick, y_percent, x_percent);
        joystick->power_switch = gpio_get_level(JOYSTICK_SWITCH);

        vTaskDelay(pdMS_TO_TICKS(250));
    }

    adc_deinit(unit);
}
