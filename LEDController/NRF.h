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
  int red;
  int green;
  int blue;
  int buttonState[8];
  int counter;
 // int switchPinValue;
  int blinkCounterValue = 50;
  int firstMode;
  int thirdMode;
  //int numberOfLEDS;
  int BRIGHTNESS;

};
typedef struct package Package;
extern Package data;




#endif
