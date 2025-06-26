

#include "mbed.h"
#include "ultrasonic.h"
#include <cstdio>
#include "my_mqtt.h"

 void dist(int distance)
{
    //put code here to happen when the distance is changed
    printf("Distance changed to %dmm\r\n", distance);
}

ultrasonic mu(D3, D4, .1, 1, &dist);    //Set the trigger pin to D8 and the echo pin to D9
                                        //have updates every .1 seconds and a timeout after 1
                                        //second, and call dist when the distance changes

int getDistance(){
    return mu.getCurrentDistance();
}
void check_presence(){
    printf("sending distance data:");
    mu.startUpdates();//start mesuring the distance
    char buffer[256];
    sprintf(buffer, "%d", getDistance());
    mqtt_send(buffer, strlen(buffer), "distance");
}
