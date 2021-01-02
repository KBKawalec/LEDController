#ifndef _NRF_H
#define _NRF_H
#include <Arduino.h>

#include "rotaryEncoderInterrupt.h"

//NRF

extern void transmitNRF();
extern void setNRF();
extern void initializeNRF();

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
extern Package data;




#endif
