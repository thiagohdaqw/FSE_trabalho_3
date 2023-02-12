#ifndef __STATES_H__
#define __STATES_H__

#include "motor.h"
#include "joystick.h"

typedef struct {
    Joystick joystick;
    Motor motor;
    int temperature;
    int humidity;
} State;

#endif