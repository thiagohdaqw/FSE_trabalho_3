#ifndef __MQTT_H__
#define __MQTT_H__

void mqtt_start();

void mqtt_send_message(char *topic, char *msg);
void mqtt_send_telemetry(char *msg);
void mqtt_send_attributes(char *msg);
void mqtt_send_request_response(int request_id);

#endif