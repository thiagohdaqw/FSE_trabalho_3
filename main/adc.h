#ifndef __ADC_H__
#define __ADC_H__

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

typedef struct
{
    int number;
    adc_oneshot_unit_handle_t handle;
    adc_cali_handle_t calibration_handle;
    bool has_calibration;
} Unit;

Unit adc_init_unit(int adc_unit);
void adc_init_channel(int channel, int bitwidth, adc_atten_t atten, Unit *unit);

int adc_read(Unit *unit, int channel);

void adc_init_calibration(Unit *unit, adc_atten_t atten, int bitwidth);

void adc_deinit(Unit read_joystickunit);
#endif