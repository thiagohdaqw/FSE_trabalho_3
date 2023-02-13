#include<string.h>

#include "mqtt_client.h"
#include "esp_log.h"

#include "commands.h"
#include "states.h"
#include "mqtt.h"

#define TAG "COMMANDS"

extern State state;

static void commands_handle_wifi_ir_request(esp_mqtt_event_handle_t event) {
    ESP_LOGI(TAG, "Received wifi_ir command");
    state.mode = state.mode == WIFI_MODE ? IR_MODE : WIFI_MODE;
    mqtt_send_request_response(event->msg_id);
}

static int commands_handle_car_request(esp_mqtt_event_handle_t event) {
    if (strstr(event->data, "c_wifi_ir")) {
        commands_handle_wifi_ir_request(event);
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
