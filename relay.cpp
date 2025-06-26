#include "relay.h"

DigitalOut relayIn(PC_8);

volatile bool isOn = false;
void toggle_relay(){
    isOn ? relayIn = 0 : relayIn = 1;
    isOn = !isOn;
    ThisThread::sleep_for(2000);
    isOn ? relayIn = 0 : relayIn = 1;
}