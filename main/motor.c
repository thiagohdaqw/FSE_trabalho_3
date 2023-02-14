#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"

#include "states.h"
#include "motor.h"

#define MOTOR_A_PWM 23
#define MOTOR_A1 22
#define MOTOR_A2 21

#define MOTOR_B_PWM 19
#define MOTOR_B1 18
#define MOTOR_B2 5

void motor_control(void *params) {
    State *state = (State *)params;
    Joystick *joystick = &state->joystick;
    Motor *motor = &state->motor;

    gpio_set_direction(MOTOR_A1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_A2, GPIO_MODE_OUTPUT);

    gpio_set_direction(MOTOR_B1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_B2, GPIO_MODE_OUTPUT);

    // Configuração do Timer
    ledc_timer_config_t timer_config = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                        .duty_resolution = LEDC_TIMER_8_BIT,
                                        .timer_num = LEDC_TIMER_0,
                                        .freq_hz = 1000,
                                        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    // Configuração do Canal A
    ledc_channel_config_t channel_config_a = {.gpio_num = MOTOR_A_PWM,
                                              .speed_mode = LEDC_LOW_SPEED_MODE,
                                              .channel = LEDC_CHANNEL_4,
                                              .timer_sel = LEDC_TIMER_0,
                                              .duty = 0,
                                              .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config_a));

    // Configuração do Canal b
    ledc_channel_config_t channel_config_b = {.gpio_num = MOTOR_B_PWM,
                                              .speed_mode = LEDC_LOW_SPEED_MODE,
                                              .channel = LEDC_CHANNEL_5,
                                              .timer_sel = LEDC_TIMER_0,
                                              .duty = 0,
                                              .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config_b));

    while (true) {
        if (joystick->y_percent != 0) {
            motor->duty = abs(joystick->y_percent) * 255 / 100;
            if (joystick->y_percent > 0) {
                gpio_set_level(MOTOR_A1, 1);
                gpio_set_level(MOTOR_A2, 0);
                gpio_set_level(MOTOR_B1, 1);
                gpio_set_level(MOTOR_B2, 0);
                motor->y = 1;
            } else {
                gpio_set_level(MOTOR_A1, 0);
                gpio_set_level(MOTOR_A2, 1);
                gpio_set_level(MOTOR_B1, 0);
                gpio_set_level(MOTOR_B2, 1);
                motor->y = -1;
            }
            motor->x = 0;
        } else if (joystick->x_percent != 0) {
            motor->duty = abs(joystick->x_percent) * 255 / 100;
            if (joystick->x_percent > 0) {
                gpio_set_level(MOTOR_A1, 0);
                gpio_set_level(MOTOR_A2, 1);
                gpio_set_level(MOTOR_B1, 1);
                gpio_set_level(MOTOR_B2, 0);
                motor->x = 1;
            } else {
                gpio_set_level(MOTOR_A1, 1);
                gpio_set_level(MOTOR_A2, 0);
                gpio_set_level(MOTOR_B1, 0);
                gpio_set_level(MOTOR_B2, 1);
                motor->x = -1;
            }
            motor->y = 0;
        } else {
            motor->duty = 0;
            motor->x = 0;
            motor->y = 0;
        }

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, motor->duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4);

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, motor->duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5);

        ESP_LOGI("MOTOR", "Duty %d X=%d Y=%d", motor->duty, motor->x, motor->y);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
