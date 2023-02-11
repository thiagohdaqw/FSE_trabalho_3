#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "soc/soc_caps.h"
#include "driver/ledc.h"
#include "esp_log.h"


#include "states.h"
//#define MOTOR_A1 GPIO_NUM_27
//#define MOTOR_A2 GPIO_NUM_26
//#define MOTOR_A_PWM 14 FUNCIONA


//#define MOTOR_A1 GPIO_NUM_33
//#define MOTOR_A2 GPIO_NUM_32
//#define MOTOR_A_PWM 25 FUNCIONA

//#define MOTOR_B1 GPIO_NUM_33
//#define MOTOR_B2 GPIO_NUM_32
//#define MOTOR_B_PWM 35


#define MOTOR_A_PWM 23
#define MOTOR_A1 22
#define MOTOR_A2 21

#define MOTOR_B_PWM 19
#define MOTOR_B1 18
#define MOTOR_B2 5



void control_motor(void *params)
{
    Joystick *joystick = (Joystick *)params;

    gpio_set_direction(MOTOR_A1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_A2, GPIO_MODE_OUTPUT);

    gpio_set_direction(MOTOR_B1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_B2, GPIO_MODE_OUTPUT);

    gpio_set_level(MOTOR_A1, 0);         // Turn the motorA1 on
    gpio_set_level(MOTOR_A2, 1);         // Turn the motorA2 off
    gpio_set_level(MOTOR_B1, 0);         // Turn the motorB1 on
    gpio_set_level(MOTOR_B2, 1);         // Turn the motorB2 off    
    
   

    // Configuração do Timer
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));


    // Configuração do Canal A
    ledc_channel_config_t channel_config_a = {
        .gpio_num = MOTOR_A_PWM,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config_a));

    // Configuração do Canal b
    ledc_channel_config_t channel_config_b = {
        .gpio_num = MOTOR_B_PWM,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config_b));


    int duty = 0;

    while (true)
    {
        if (joystick->y_percent != 0) {
            duty = abs(joystick->y_percent) * 255 / 100;
            if (joystick->y_percent > 0) {
                gpio_set_level(MOTOR_A1, 1);
                gpio_set_level(MOTOR_A2, 0);
                gpio_set_level(MOTOR_B1, 1);
                gpio_set_level(MOTOR_B2, 0);
            } else {
                gpio_set_level(MOTOR_A1, 0);
                gpio_set_level(MOTOR_A2, 1);
                gpio_set_level(MOTOR_B1, 0);
                gpio_set_level(MOTOR_B2, 1);
            }
        } else if (joystick->x_percent != 0) {
            duty = abs(joystick->x_percent) * 255 / 100;
            if (joystick->x_percent > 0) {
                gpio_set_level(MOTOR_A1, 0);
                gpio_set_level(MOTOR_A2, 1);
                gpio_set_level(MOTOR_B1, 1);
                gpio_set_level(MOTOR_B2, 0);
            } else {
                gpio_set_level(MOTOR_A1, 1);
                gpio_set_level(MOTOR_A2, 0);
                gpio_set_level(MOTOR_B1, 0);
                gpio_set_level(MOTOR_B2, 1);
            }
        } else {
            duty = 0;
        }

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

        ESP_LOGI("MOTOR", "Duty %d", duty);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

