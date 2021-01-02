#ifndef _ROTARYENCODERINTERRUPT_H
#define _ROTARYENCODERINTERRUPT_H
#include <Arduino.h>


#define UPPPER_LIMIT_OF_OPTIONS  6

#define BLINK_MODE 2
#define BRIGHTNESS_MODE 5

#define BRIGHTNESS_AMOUNT 10
#define BLINK_START 50
#define BRIGHTNESS_START 255

extern void buttonInterrupt();
extern void rotaryInterrupt();
extern void initializeRotaryParamaters();
//extern byte firstMode;
//extern byte thirdMode;
//extern byte fifthMode;
extern byte whichMode[UPPPER_LIMIT_OF_OPTIONS];
extern byte modeCounter[UPPPER_LIMIT_OF_OPTIONS];
extern char counter;
extern byte switchCounter;



#endif
