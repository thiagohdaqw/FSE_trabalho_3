#ifndef __LEDRGB_H__
#define __LEDRGB_H__

#define LEDBLUE     33
#define LEDGREEN    32
#define LEDRED      27

void led_rgb_init();
void led_rgb_color_on(int color);
void led_rgb_blue();   
void led_rgb_green();    
void led_rgb_red();
void led_rgb_cyan();    
void led_rgb_magenta();
void led_rgb_greenishYellow();
void led_rgb_off();
void led_rgb_white();
void led_rgb_all_colors(void *params);

#endif