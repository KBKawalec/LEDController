#ifndef _NRFRECEIVER_H
#define _NRFRECEIVER_H
#include <Arduino.h>


#include <SPI.h>
#include "RF24.h"
#include <FastLED.h>


#define LED_PIN     2


extern int NUM_LEDS;
extern CRGB *leds;
extern RF24 myRadio;
extern byte addresses[6];
extern int BRIGHTNESS;



struct package
{
  int red;
  int green;
  int blue;
  int buttonState[8];
  int counter;
  int switchPinValue;
  int blinkCounterValue;
  int firstMode;
  int thirdMode;
  int NUM_LEDS;
  int BRIGHTNESS;
};

typedef struct package Package;




#endif
