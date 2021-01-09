#ifndef _NRFRECEIVER_H
#define _NRFRECEIVER_H
#include <Arduino.h>


#include <SPI.h>
#include "RF24.h"
#include <FastLED.h>

#define UPPPERLIMITOFOPTIONS  6
#define BLINKMODE 2
#define BRIGHTNESSMODE 5
#define LED_PIN     2
#define MAXLEDS 200


  extern int NUM_LEDS;
extern CRGB leds[MAXLEDS];
extern RF24 myRadio;
extern byte addresses[6];
extern byte BRIGHTNESS;



struct package
{
  byte red;
  byte green;
  byte blue;
  byte buttonState[8];
  byte counter;
  byte modeCounter[UPPPERLIMITOFOPTIONS];
};

typedef struct package Package;




#endif
