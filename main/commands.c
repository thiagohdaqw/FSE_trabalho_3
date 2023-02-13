#include<string.h>
#include <stdlib.h>

#include "mqtt_client.h"
#include "esp_log.h"

#include "mqtt.h"
#include "states.h"
#include "commands.h"
#include "joystick.h"
#include "led_rgb.h"

#define TAG "COMMANDS"

extern State state;

static void commands_handle_wifi_ir_request(esp_mqtt_event_handle_t event) {
    ESP_LOGI(TAG, "Received wifi_ir command. TOPIC=%.*s", event->topic_len, event->topic);
    state.mode = state.mode == WIFI_MODE ? IR_MODE : WIFI_MODE;

    if (state.mode == WIFI_MODE) {
        led_rgb_blue();
    }
    if (state.mode == IR_MODE) {
        led_rgb_red();
    }
}

static void commands_joystick_position(char *joystick) {
    char *x = strstr(joystick, "joystick_x\":");
    char *y = strstr(joystick, "joystick_y\":");

    if (!x || !y) {
        return;
    }

    joystick_set_percent(&state.joystick, atoi(x+12), atoi(y+12));
}

static int commands_handle_car_request(esp_mqtt_event_handle_t event) {
    char *command;

    if (strstr(event->data, "c_wifi_ir")) {
        commands_handle_wifi_ir_request(event);
        return 1;
    }
    if ((command = strstr(event->data, "joystick_"))) {
        commands_joystick_position(command);
        return 1;
    }
    return 0;
}

static int commands_handle_joystick_request(esp_mqtt_event_handle_t event) {
    if (strstr(event->data, "j_wifi_ir")) {
        commands_handle_wifi_ir_request(event);
        return 1;
    }
    return 0;
}

void commands_handle_request(esp_mqtt_event_handle_t event) {
#ifdef CONFIG_CAR
    if (!commands_handle_car_request(event)) {
#endif
#ifdef CONFIG_JOYSTICK
    if (!commands_handle_joystick_request(event)) {
#endif
        ESP_LOGW(TAG, "Received command not found %.*s", event->data_len, event->data);
    }
}
