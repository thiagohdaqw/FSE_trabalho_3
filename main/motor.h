#ifndef __MOTOR_H__
#define __MOTOR_H__

typedef struct {
    int duty;
    int x;
    int y;
} Motor;

void motor_control(void *params);

#endif