#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "mqtt_client.h"

void commands_handle_request(esp_mqtt_event_handle_t event);

#endif