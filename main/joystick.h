#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <stdint.h>

typedef struct
{
    int8_t x_percent;
    int8_t y_percent;
    int power_switch;
} Joystick;

void joystick_read(void *params);
void joystick_set_percent(Joystick *joystick, int x_percent, int y_percent);

#endif
