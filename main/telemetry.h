#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#include "states.h"

void telemetry_init(State *state);
void telemetry_send_car_attributes(State *state);

#endif
