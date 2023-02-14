#include "nvs_mem.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "states.h"
#include <stdio.h>
#include <string.h>

#define TAG "NVS"

int nvs_load_attribute(char *name, int32_t *value) {
    ESP_ERROR_CHECK(nvs_flash_init());

    nvs_handle particao_padrao_handle;

    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READONLY, &particao_padrao_handle);

    if (res_nvs != ESP_OK) {
        ESP_LOGE("NVS", "Error opening nvs");
    } else {
        esp_err_t res = nvs_get_i32(particao_padrao_handle, name, value);
        nvs_close(particao_padrao_handle);

        switch (res) {
        case ESP_OK:
            break;
        case ESP_ERR_NOT_FOUND:
            ESP_LOGE(TAG, "value not found");
            return -1;
        default:
            ESP_LOGE(TAG, "Error reading nvs (%s)", esp_err_to_name(res));
            return -1;
        }
    }
    return 1;
}

void nvs_save_attribute(char *name, int32_t value) {
    ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(nvs_flash_init_partition("DadosNVS"));

    nvs_handle particao_padrao_handle;

    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READWRITE, &particao_padrao_handle);
    // esp_err_t res_nvs = nvs_open_from_partition("DadosNVS", "armazenamento", NVS_READWRITE, &particao_padrao_handle);

    if (res_nvs == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE(TAG, "Namespace: armazenamento, não encontrado");
    }
    esp_err_t res = nvs_set_i32(particao_padrao_handle, name, value);
    if (res != ESP_OK) {
        ESP_LOGE(TAG, "Não foi possível escrever no NVS (%s)", esp_err_to_name(res));
    }
    nvs_commit(particao_padrao_handle);
    nvs_close(particao_padrao_handle);
    ESP_LOGI(TAG, "%s saved", name);
}

void nvs_save_car_state(State *state) {
    nvs_save_attribute("temperatura", state->temperature);
    nvs_save_attribute("humidity", state->humidity);
    nvs_save_attribute("mode", state->mode);
    nvs_save_attribute("motor_x", state->motor.x);
    nvs_save_attribute("motor_y", state->motor.y);
    nvs_save_attribute("motor_duty", state->motor.duty);
}

void nvs_save_joystick_state(State *state) {
    nvs_save_attribute("mode", state->mode);
}

void nvs_init() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void nvs_load_car_state(State *state) {
    int32_t value = 0;
    if (nvs_load_attribute("mode", &value) > 0) {
        state->mode = (char)value;
        ESP_LOGI(TAG, "Mode (%d) was loaded successfuly", state->mode);
    }
    if (nvs_load_attribute("humidity", &value) > 0) {
        state->humidity = (int)value;
        ESP_LOGI(TAG, "humidity(%d) value was loaded successfuly", state->humidity);
    }
    if (nvs_load_attribute("temperature", &value) > 0) {
        state->temperature = (int)value;
        ESP_LOGI(TAG, "temperature(%d) value was loaded successfuly", state->temperature);
    }
    if (nvs_load_attribute("motor_x", &value) > 0) {
        state->motor.x = (int)value;
        ESP_LOGI(TAG, "motor_x(%d) value was loaded successfuly", state->motor.x);
    }
    if (nvs_load_attribute("motor_y", &value) > 0) {
        state->motor.y = (int)value;
        ESP_LOGI(TAG, "motor_y(%d) value was loaded successfuly", state->motor.y);
    }
    if (nvs_load_attribute("motor_duty", &value) > 0) {
        state->motor.duty = (int)value;
        ESP_LOGI(TAG, "motor_duty(%d) value was loaded successfuly", state->motor.duty);
    }
}

void nvs_load_joystick_state(State *state) {
    int32_t value = 0;
    if (nvs_load_attribute("mode", &value) > 0) {
        state->mode = (char)value;
        ESP_LOGI(TAG, "Mode (%d) was loaded successfuly", state->mode);
    }
}

void nvs_load_state(State *state) {
    ESP_LOGI(TAG, "Loading persistence states");
    nvs_init();
#ifdef CONFIG_CAR
    nvs_load_car_state(state);
#endif
#ifdef CONFIG_JOYSTICK
    nvs_load_joystick_state(state);
#endif
}

void nvs_save_state(void *params) {
    State *state = (State *)params;

    while (true) {
#ifdef CONFIG_CAR
        nvs_save_car_state(state);
#endif
#ifdef CONFIG_JOYSTICK
        nvs_save_joystick_state(state);
#endif
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
