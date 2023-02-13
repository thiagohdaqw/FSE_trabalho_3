#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

typedef struct {
    int x_percent;
    int y_percent;
    int power_switch;
} Joystick;

void joystick_read(void *params);
void joystick_set_percent(Joystick *joystick, int x_percent, int y_percent);

#endif
