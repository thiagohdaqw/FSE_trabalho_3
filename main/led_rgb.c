#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <inttypes.h>
#include "sdkconfig.h"
#include "esp_chip_info.h"

#include "driver/gpio.h"
#include "led_rgb.h"

void led_rgb_init(){
    gpio_set_direction(LEDBLUE, GPIO_MODE_OUTPUT);
    gpio_set_direction(LEDGREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LEDRED, GPIO_MODE_OUTPUT);
}

void led_rgb_blue(){
    led_rgb_color_on(0);
}

void led_rgb_green(){
    led_rgb_color_on(1);
}

void led_rgb_red(){
    led_rgb_color_on(2);
}


void led_rgb_cyan(){
    led_rgb_color_on(3);
}


void led_rgb_magenta(){
    led_rgb_color_on(4);
}

void led_rgb_greenishYellow(){
    led_rgb_color_on(5);
}

void led_rgb_white(){  
    led_rgb_color_on(6);
}

void led_rgb_off(){
    led_rgb_color_on(7);
}

void led_rgb_color_on(int color)
{
    led_rgb_init();
    int estado = 1;
    switch(color){
        case 0: //AZUL
            gpio_set_level(LEDBLUE, estado);
            gpio_set_level(LEDGREEN, 0);
            gpio_set_level(LEDRED, 0);                    
            break;
        case 1: //VERDE
            gpio_set_level(LEDBLUE, 0);
            gpio_set_level(LEDGREEN, estado);
            gpio_set_level(LEDRED, 0);    
            break;
        case 2: //VERMELhO
            gpio_set_level(LEDBLUE, 0);
            gpio_set_level(LEDGREEN, 0);
            gpio_set_level(LEDRED, estado);             
            break;
        case 3: //cyan
            gpio_set_level(LEDBLUE, estado);
            gpio_set_level(LEDGREEN, estado);
            gpio_set_level(LEDRED, 0);                 
            break;
        case 4: //MAGENTA
            gpio_set_level(LEDBLUE, estado);
            gpio_set_level(LEDGREEN, 0);
            gpio_set_level(LEDRED, estado);              
            break;
        case 5: //AMARELO ESVERDEADO
            gpio_set_level(LEDBLUE, 0);
            gpio_set_level(LEDGREEN, estado);
            gpio_set_level(LEDRED, estado);             
            break;
        case 6: //BRANCO
            gpio_set_level(LEDBLUE, estado);
            gpio_set_level(LEDGREEN, estado);
            gpio_set_level(LEDRED, estado);               
            break;
        default: //DESLIGADO
            gpio_set_level(LEDBLUE, 0);
            gpio_set_level(LEDGREEN, 0);
            gpio_set_level(LEDRED, 0);   
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
                gpio_set_level(LEDBLUE, 1);
                gpio_set_level(LEDGREEN, 0);
                gpio_set_level(LEDRED, 0);        
                contador+=1;            
                break;
            case 1: //VERDE
                gpio_set_level(LEDBLUE, 0);
                gpio_set_level(LEDGREEN, 1);
                gpio_set_level(LEDRED, 0);    
                contador+=1;                            
                break;
            case 2: //VERMELhO
                gpio_set_level(LEDBLUE, 0);
                gpio_set_level(LEDGREEN, 0);
                gpio_set_level(LEDRED, 1);             
                contador+=1;                            
                break;
            case 3: //cyan
                gpio_set_level(LEDBLUE, 1);
                gpio_set_level(LEDGREEN, 1);
                gpio_set_level(LEDRED, 0);       
                contador+=1;                                      
                break;
            case 4: //MAGENTA
                gpio_set_level(LEDBLUE, 1);
                gpio_set_level(LEDGREEN, 0);
                gpio_set_level(LEDRED, 1);              
                contador+=1;                            
                break;
            case 5: //AMARELO ESVERDEADO
                gpio_set_level(LEDBLUE, 0);
                gpio_set_level(LEDGREEN, 1);
                gpio_set_level(LEDRED, 1);             
                contador+=1;                            
                break;
            case 6: //BRANCO
                gpio_set_level(LEDBLUE, 1);
                gpio_set_level(LEDGREEN, 1);
                gpio_set_level(LEDRED, 1);               
                contador+=1;                            
                break;
            default: //DESLIGADO
                gpio_set_level(LEDBLUE, 0);
                gpio_set_level(LEDGREEN, 0);
                gpio_set_level(LEDRED, 0);           
                contador=0;            
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}