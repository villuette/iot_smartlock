#include "DigitalIn.h"
#include "PinNames.h"
#include "PinNamesTypes.h"
#include "mbed.h"
#include <cstdio>
#include <cstring>
#include "relay.h"

char get_key();

int handle_password_input(char next);

void process_keyboard();