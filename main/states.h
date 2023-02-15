#ifndef __STATES_H__
#define __STATES_H__

#include "motor.h"
#include "joystick.h"

#define WIFI_MODE 0
#define IR_MODE 1

typedef struct
{
    Joystick joystick;
    Motor motor;
    int temperature;
    int humidity;
    char mode;
    int low_power;
} State;

#endif
