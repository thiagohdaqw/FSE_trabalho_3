#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include "mqtt.h"
#include "wifi.h"
#include "motor.h"
#include "states.h"
#include "infrared.h"
#include "joystick.h"
#include "temperature_dht11.h"

const static char *TAG = "APP";

Joystick joystick;

SemaphoreHandle_t conexaoWifiSemaphore;
SemaphoreHandle_t conexaoMQTTSemaphore;

void conectadoWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
    }
  }
}

void trataComunicacaoComServidor(void * params)
{
  char mensagem[50];
  if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY))
  {
    while(true)
    {
       float temperatura = 20.0 + (float)rand()/(float)(RAND_MAX/10.0);
       sprintf(mensagem, "{\"temperature\": %.2f}", temperatura);
       mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
       vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
}

void wifi_init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
}

void app_main(void)
{
    wifi_init();

#ifdef CONFIG_CAR
    ESP_LOGI(TAG, "Initing as car controller");
    xTaskCreate(&control_motor,"motor", 2048, &joystick, 1, NULL);
    xTaskCreate(&read_joystick, "Joystick", 2048, &joystick, 1, NULL);
#endif

#ifdef CONFIG_JOYSTICK
    ESP_LOGI(TAG, "Initing as joystick controller");
    xTaskCreate(&read_joystick, "Joystick", 2048, NULL, 1, NULL);
    xTaskCreate(&read_temperature_dht11, "DHT11 Temperature", 2048, NULL, 1, NULL);
#endif

#ifdef CONFIG_MONITOR
    ESP_LOGI(TAG, "Initing as monitor controller");
    xTaskCreate(infrared_rx_task, "Infrared RX", 4096, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(infrared_tx_task, "Infrared TX", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
#endif
}