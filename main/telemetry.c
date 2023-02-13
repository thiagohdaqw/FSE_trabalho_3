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

#include "mqtt.h"
#include "wifi.h"
#include "states.h"
#include "telemetry.h"
#include "temperature.h"

#define TAG "TELEMETRY"

SemaphoreHandle_t wifiConnectionSemaphore;
SemaphoreHandle_t mqttConnectionSemaphore;

static void mqtt_loop(void *params) {
    while (true) {
        if (xSemaphoreTake(wifiConnectionSemaphore, portMAX_DELAY)) {
            mqtt_start();
        }
    }
}

void telemetry_send_car(State *state) {
    static char message[100];
 
    temperature_read(state);
    sprintf(message,
        "{\"temperature\":%d,\"humidity\":%d,\"motor_x\":%d,\"motor_y\":%d,\"motor_duty\":%d}",
        state->temperature,
        state->humidity,
        state->motor.x,
        state->motor.y,
        state->motor.duty
    );
    mqtt_send_telemetry(message);
}

void telemetry_send_car_attributes(State *state) {
    char message[50];

    sprintf(message,
        "{\"motor_deg\":%d,\"car_mode\":%d}",
        (state->motor.x != 0 ? (state->motor.x > 0 ? 90 : 270) : 0)
        + (state->motor.y >= 0 ? 0 : 180),
        state->mode
    );
    mqtt_send_attributes(message);
}

void telemetry_send_joystick_attributes(State *state) {
    char message[50];
    sprintf(message,
        "{\"joystick_deg\":%d,\"joystick_mode\":%d}",
        (state->joystick.x_percent != 0 ? (state->joystick.x_percent > 0 ? 90 : 270) : 0)
        + (state->joystick.y_percent >= 0 ? 0 : 180),
        state->mode
    );
    mqtt_send_attributes(message);
}

void telemetry_send_joystick_status(int online, State *state) {
    char message[30];

    sprintf(message,
        "{\"joystick_online\":%d}",
        online
    );
    mqtt_send_attributes(message);
    telemetry_send_joystick_attributes(state);
}

void telemetry_send_car_status(int online, State *state) {
    char message[30];

    sprintf(message,
        "{\"car_online\":%d}",
        online
    );
    mqtt_send_attributes(message);
    telemetry_send_car_attributes(state);
}

void telemetry_send_joystick(State *state) {
    char message[100];

    sprintf(message,
        "{\"joystick_x\":%d,\"joystick_y\":%d}",
        state->joystick.x_percent,
        state->joystick.y_percent
    );
    mqtt_send_message("v1/devices/me/telemetry/joystick", message);
}

static void telemetry_loop(void *params) {
    State *state = (State *)params;

    if (xSemaphoreTake(mqttConnectionSemaphore, portMAX_DELAY)) {
#if CONFIG_CAR
        telemetry_send_car_status(true, state);
#endif
#if CONFIG_JOYSTICK
        telemetry_send_joystick_status(true, state);
#endif

        while (true) {
#if CONFIG_CAR
            ESP_LOGI(TAG, "Sending car telemetry");
            telemetry_send_car(state);
            telemetry_send_car_attributes(state);
#endif
#if CONFIG_JOYSTICK
            ESP_LOGI(TAG, "Sending joystick telemetry");
            telemetry_send_joystick(state);
            telemetry_send_joystick_attributes(state);
#endif
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void telemetry_init(State *state) {
    wifiConnectionSemaphore = xSemaphoreCreateBinary();
    mqttConnectionSemaphore = xSemaphoreCreateBinary();
    
    wifi_start();

#if CONFIG_CAR
    // temperature_init();
#endif

    ESP_LOGI(TAG, "initiating Telemetry");
    xTaskCreate(&mqtt_loop, "MQTT Loop", 2048, NULL, 1, NULL);
    xTaskCreate(&telemetry_loop, "Telemetry Loop", 2048, state, 1, NULL);
}