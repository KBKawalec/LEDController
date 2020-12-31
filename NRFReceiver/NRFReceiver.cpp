#include "NRFReceiver.h"

int NUM_LEDS = 30;

RF24 myRadio (7, 8);
byte addresses[6] = {'j', 'j', 'k', 'a', 'b', 'e'};

byte BRIGHTNESS = 255;
