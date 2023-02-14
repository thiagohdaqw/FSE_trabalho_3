#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <inttypes.h>
#include "sdkconfig.h"
#include "esp_chip_info.h"

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "led_rgb.h"

void led_rgb_init(){
    // Configuração do Timer
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_1,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    // Configuração do Canal
    ledc_channel_config_t channel_config_blue = {
        .gpio_num = LEDBLUE,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config_blue));

    // Configuração do Canal
    ledc_channel_config_t channel_config_green = {
        .gpio_num = LEDGREEN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config_green));

    // Configuração do Canal
    ledc_channel_config_t channel_config_red = {
        .gpio_num = LEDRED,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_2,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config_red));

    ledc_fade_func_install(0);
}

void led_rgb_blue(){
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 255, 1000, LEDC_FADE_WAIT_DONE);
}

void led_rgb_green(){
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);            
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 255, 1000, LEDC_FADE_WAIT_DONE);
}

void led_rgb_red(){
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);        
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 255, 1000, LEDC_FADE_WAIT_DONE);
}


void led_rgb_cyan(){
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 255, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 255, 1000, LEDC_FADE_WAIT_DONE);}


void led_rgb_magenta(){
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 255, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 255, 1000, LEDC_FADE_WAIT_DONE); }

void led_rgb_greenishYellow(){
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 255, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 255, 1000, LEDC_FADE_WAIT_DONE); }

void led_rgb_white(){  
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 255, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 255, 1000, LEDC_FADE_WAIT_DONE);    
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 255, 1000, LEDC_FADE_WAIT_DONE); }

void led_rgb_off(){
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, 0, 1000, LEDC_FADE_WAIT_DONE);}


void led_rgb_colors_on(int color)
{
    led_rgb_init();
    switch(color){
        case 0: //AZUL
            led_rgb_blue();     
            break;
        case 1: //VERDE
            led_rgb_green();          
            break;
        case 2: //VERMELhO
            led_rgb_red();                  
            break;
        case 3: //cyan
            led_rgb_cyan();            
            break;
        case 4: //MAGENTA
            led_rgb_magenta();                  
            break;
        case 5: //AMARELO ESVERDEADO
            led_rgb_greenishYellow();                   
            break;
        case 6: //BRANCO
            led_rgb_white();                     
            break;
        default: //DESLIGADO
            led_rgb_off();         
    }
}

void led_rgb_all_colors(void *params)
{
    led_rgb_init();
    int contador = 0;
    while (true)
    {        
        switch(contador){
            case 0: //AZUL
                led_rgb_blue();     
                contador+=1;            
                break;
            case 1: //VERDE
                led_rgb_green();  
                contador+=1;                            
                break;
            case 2: //VERMELhO
                led_rgb_red();          
                contador+=1;                            
                break;
            case 3: //cyan
                led_rgb_cyan();    
                contador+=1;                                      
                break;
            case 4: //MAGENTA
                led_rgb_magenta();          
                contador+=1;                            
                break;
            case 5: //AMARELO ESVERDEADO
                led_rgb_greenishYellow();           
                contador+=1;                            
                break;
            case 6: //BRANCO
                led_rgb_white();             
                contador+=1;                            
                break;
            default: //DESLIGADO
                led_rgb_off();         
                contador=0;            
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}