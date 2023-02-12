#ifndef __LEDRGB_H__
#define __LEDRGB_H__
#define LEDBLUE GPIO_NUM_27
#define LEDGREEN GPIO_NUM_26
#define LEDRED GPIO_NUM_25

void defineLeds();
void turnColorOn(int color);
void ledBlue(void *params);   
void ledGreen(void *params);    
void ledRed(void *params);
void ledCyan(void *params);    
void ledMagenta(void *params);
void ledGreenishYellow(void *params);
void ledsOff(void *params);
void ledWhite(void *params);
void turnAllColors(void *params);

#endif