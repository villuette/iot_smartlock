#include "mbed.h"
#include "MQTTClientMbedOs.h"
#include "MQTTmbed.h"
#include "distance.h"
#include "rfid.h"

void mqtt_demo(NetworkInterface *net);
void mqtt_send(void* databuf, int len, const char* topic);
