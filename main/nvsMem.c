#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs.h"
#include "nvsMem.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "states.h"
#define TAG "NVS"

int nvs_load_attribute(char *name, int32_t *value)
{
    // Inicia o acesso à partição padrão nvs
    ESP_ERROR_CHECK(nvs_flash_init());

    // Inicia o acesso à partição personalizada
    nvs_handle particao_padrao_handle;

    // Abre o acesso à partição DadosNVS
    esp_err_t res_nvs = nvs_open_from_partition("DadosNVS", "armazenamento", NVS_READONLY, &particao_padrao_handle);

    if (res_nvs == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    }
    else
    {
        esp_err_t res = nvs_get_i32(particao_padrao_handle, name, &value);

        switch (res)
        {
        case ESP_OK:
            break;
        case ESP_ERR_NOT_FOUND:
            ESP_LOGE("NVS", "value não encontrado");
            return -1;
        default:
            ESP_LOGE("NVS", "Erro ao acessar o NVS (%s)", esp_err_to_name(res));
            return -1;
            break;
        }

        nvs_close(particao_padrao_handle);
    }
    return 1;
}

void nvs_save_attribute(char *name, int32_t value)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(nvs_flash_init_partition("DadosNVS"));

    nvs_handle particao_padrao_handle;

    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READWRITE, &particao_padrao_handle);
    // esp_err_t res_nvs = nvs_open_from_partition("DadosNVS", "armazenamento", NVS_READWRITE, &particao_padrao_handle);

    if (res_nvs == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    }
    esp_err_t res = nvs_set_i32(particao_padrao_handle, name, value);
    if (res != ESP_OK)
    {
        ESP_LOGE("NVS", "Não foi possível escrever no NVS (%s)", esp_err_to_name(res));
    }
    nvs_commit(particao_padrao_handle);
    nvs_close(particao_padrao_handle);
}

void nvs_save_car_state(State *state)
{
    nvs_save_attribute("temperatura", state->temperature);
    nvs_save_attribute("humidity", state->humidity);
    nvs_save_attribute("mode", state->mode);
    nvs_save_attribute("motor_x", state->motor.x);
    nvs_save_attribute("motor_y", state->motor.y);
    nvs_save_attribute("motor_duty", state->motor.duty);
}

void nvs_save_joystick_state(State *state)
{
    nvs_save_attribute("mode", state->mode);
    nvs_save_attribute("joystick_x_percent", state->joystick.x_percent);
    nvs_save_attribute("joystick_y_percent", state->joystick.y_percent);
}

void nvs_init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void nvs_load_car_state(State *state)
{
    int32_t value = 0;
    if (nvs_load_attribute("mode",&value) > 0){
        state->mode = value;
        ESP_LOGI(TAG,"The mode value was loaded successfuly");
    }
    if(nvs_load_attribute("humidity",&value) > 0){
        state->humidity = value;
        ESP_LOGI(TAG,"The humidity value was loaded successfuly");
    }
}

void nvs_load_joystick_state(State *state){

}

void nvs_load_state(State *state)
{
    nvs_init();
#ifdef CONFIG_CAR
    nvs_load_car_state(state);
#endif
#ifdef CONFIG_JOYSTICK
    nvs_load_joystick_state(state);
#endif
}
