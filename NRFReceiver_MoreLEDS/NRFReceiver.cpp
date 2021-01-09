#include "NRFReceiver.h"

int NUM_LEDS = MAXLEDS;

RF24 myRadio (7, 8);
//slave 1
//byte addresses[6] = {'j', 'j', 'k', 'a', 'b', 'e'};

//slave 2
byte addresses[6] ={'j', 'j', 'k', 'a', 'b', 'F'};

byte BRIGHTNESS = 255;
