#ifndef _ROTARYENCODERINTERRUPT_H
#define _ROTARYENCODERINTERRUPT_H
#include <Arduino.h>


#define UPPPERLIMITOFOPTIONS  6

#define BLINKMODE 2
#define BRIGHTNESSMODE 5

#define BRIGHTNESSAMOUNT 10

extern void updatea();
extern void updateSwitch();
extern void initializeRotaryParamaters();
//extern byte firstMode;
//extern byte thirdMode;
//extern byte fifthMode;
extern byte whichMode[UPPPERLIMITOFOPTIONS];
extern byte modeCounter[UPPPERLIMITOFOPTIONS];
extern char counter;
extern byte switchCounter;



#endif
