#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <inttypes.h>
#include "sdkconfig.h"
#include "esp_chip_info.h"

#include "driver/gpio.h"
#include "ledRGB.h"

void defineLeds(){
    gpio_set_direction(LEDBLUE, GPIO_MODE_OUTPUT);
    gpio_set_direction(LEDGREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LEDRED, GPIO_MODE_OUTPUT);
}

void ledBlue(void *params){
    turnColorOn(0);
}

void ledGreen(void *params){
    turnColorOn(1);
}

void ledRed(void *params){
    turnColorOn(2);
}


void ledCyan(void *params){
    turnColorOn(3);
}


void ledMagenta(void *params){
    turnColorOn(4);
}

void ledGreenishYellow(void *params){
    turnColorOn(5);
}

void ledWhite(void *params){  
    turnColorOn(6);
}

void ledsOff(void *params){
    turnColorOn(7);
}

void turnColorOn(int color)
{
    defineLeds();
    int estado = 0;
    while (true)
    {        
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
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        estado = !estado;
    }
}


void turnAllColors(void *params)
{
    defineLeds();
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