#ifndef __NVSMEM_H__
#define __NVSMEM_H__

#include "states.h"
void nvs_load_state(State *state);
void nvs_save_state(void *params);

#endif