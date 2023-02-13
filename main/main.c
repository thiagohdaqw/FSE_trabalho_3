#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

int32_t le_valor_nvs()
{
    // Inicia o acesso à partição padrão nvs
    ESP_ERROR_CHECK(nvs_flash_init());

    // Inicia o acesso à partição personalizada
    // ESP_ERROR_CHECK(nvs_flash_init_partition("DadosNVS"));

    int32_t valor = 0;
    nvs_handle particao_padrao_handle;
    
    // Abre o acesso à partição nvs
    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READONLY, &particao_padrao_handle);
    
    // Abre o acesso à partição DadosNVS
    // esp_err_t res_nvs = nvs_open_from_partition("DadosNVS", "armazenamento", NVS_READONLY, &particao_padrao_handle);
    


    if(res_nvs == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    }
    else
    {
        esp_err_t res = nvs_get_i32(particao_padrao_handle, "contador", &valor);

        switch (res)
        {
        case ESP_OK:
            printf("Valor armazenado: %d\n", (int) valor);
            break;
        case ESP_ERR_NOT_FOUND:
            ESP_LOGE("NVS", "Valor não encontrado");
            return -1;
        default:
            ESP_LOGE("NVS", "Erro ao acessar o NVS (%s)", esp_err_to_name(res));
            return -1;
            break;
        }

        nvs_close(particao_padrao_handle);
    }
    return valor;
}

void grava_valor_nvs(int32_t valor)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(nvs_flash_init_partition("DadosNVS"));

    nvs_handle particao_padrao_handle;

    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READWRITE, &particao_padrao_handle);
    // esp_err_t res_nvs = nvs_open_from_partition("DadosNVS", "armazenamento", NVS_READWRITE, &particao_padrao_handle);
    
    if(res_nvs == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    }
    esp_err_t res = nvs_set_i32(particao_padrao_handle, "contador", valor + 1);
    if(res != ESP_OK)
    {
        ESP_LOGE("NVS", "Não foi possível escrever no NVS (%s)", esp_err_to_name(res));
    }
    nvs_commit(particao_padrao_handle);
    nvs_close(particao_padrao_handle);
}


void app_main(void)
{
    int32_t valor_lido = 0;
    valor_lido = le_valor_nvs();
    if(valor_lido == -1){
        valor_lido = 0;
    }
    grava_valor_nvs(valor_lido);
}




/*#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "joystick.h"
#include "infrared.h"
#include "temperature_analogic.h"
#include "temperature_dht11.h"
#include "motor.h"
#include "states.h"
#include "ledRGB.h"
#include "nvsMem.h"
const static char *TAG = "APP";

Joystick joystick;

void app_main(void)
{
#ifdef CONFIG_CAR
    ESP_LOGI(TAG, "Initing as car controller");
    xTaskCreate(&control_motor,"motor", 2048, &joystick, 1, NULL);
    //xTaskCreate(&read_joystick, "Joystick", 2048, &joystick, 1, NULL);
    xTaskCreate(infrared_rx_task, "Infrared RX", 4096, &joystick, configMAX_PRIORITIES, NULL);
    xTaskCreate(&ledMagenta, "control_ledRGB", 2048, NULL, 1, NULL);
    xTaskCreate(&nvs_main, "nvsMem", 2048, NULL, 1, NULL);
#endif

#ifdef CONFIG_JOYSTICK
    ESP_LOGI(TAG, "Initing as joystick controller");
    xTaskCreate(infrared_tx_task, "Infrared TX", 4096, &joystick, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(&read_joystick, "Joystick", 2048, &joystick, 1, NULL);
#endif

#ifdef CONFIG_MONITOR
    ESP_LOGI(TAG, "Initing as monitor controller");
    // xTaskCreate(&read_temperature_dht11, "DHT11 Temperature", 2048, NULL, 1, NULL);
    // xTaskCreate(&read_temperature_analogic, "Analogic Temperature", 2048, NULL, 1, NULL);
    xTaskCreate(infrared_tx_task, "Infrared TX", 4096, &joystick, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(infrared_rx_task, "Infrared RX", 4096, NULL, configMAX_PRIORITIES, NULL);
#endif
}
*/