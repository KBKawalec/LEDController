#ifndef _NRF_H
#define _NRF_H
#include <Arduino.h>



//Package data;


//NRF

extern void NRFTransmit();
extern void setNRF();
extern void NRFinitialization();

struct package
{
  byte red;
  byte green;
  byte blue;
  byte buttonState[8];
  byte counter;
  byte blinkCounterValue = 50;
  byte firstMode;
  //int numberOfLEDS;
  byte BRIGHTNESS;

};
typedef struct package Package;
extern Package data;




#endif
