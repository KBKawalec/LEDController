#include "rotaryEncoderInterrupt.h"
#include "NRF.h"
#include "LEDController.h"
#include "RF24.h"

#define UPPPERLIMITOFOPTIONS  5
#define BRIGHTNESSAMOUNT 10

byte counter = 1;

//byte whichMode[UPPPERLIMITOFOPTIONS];

byte fifthMode;
byte thirdMode;
byte firstMode;

byte pinAstateCurrent = LOW;                // Current state of Pin A
byte pinAStateLast = pinAstateCurrent;

byte previousState;

void updatea() {

  int tempCounters;
  switch (counter) {
    case 1 :
      tempCounters = counter;
      break;
    case 2 :
      tempCounters = counter;
      break;
    case 3 :
      if (thirdMode == 0) {
        tempCounters = counter;
      }
      else {
        tempCounters = blinkCounter;
      }

      break;
    case 4 :
      tempCounters = counter;

      break;
    case 5 :
      if (fifthMode == 0) {
        tempCounters = counter;
      }
      else {
        tempCounters = BRIGHTNESS;
      }

      break;

  }

  // ROTATION DIRECTION
  pinAstateCurrent = digitalRead(outputA);    // Read the current state of Pin A
  delayMicroseconds(10);
  if ((previousState == 1 && pinAStateLast == 1 && pinAstateCurrent == 0
       && digitalRead(outputB) == 1)) {

    if ( counter == 5 && fifthMode) tempCounters = tempCounters - BRIGHTNESSAMOUNT;
    else tempCounters--;
    previousState = 0;

  }

  delayMicroseconds(10);
  // If there is a minimal movement of 1 step
  if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {

    if (digitalRead(outputB) == HIGH) {      // If Pin B is HIGH
      if ( counter == 5 && fifthMode) tempCounters = tempCounters + BRIGHTNESSAMOUNT;
      else tempCounters++;
      previousState = 1;
    }
    else {
      if ( counter == 5 && fifthMode) tempCounters = tempCounters - BRIGHTNESSAMOUNT;
      else tempCounters--;
      previousState = 0;
    }

  }

  pinAStateLast = pinAstateCurrent;        // Store the latest read value in the currect state variable
  switch (counter) {
    case 1 :
      counter = tempCounters;
      break;
    case 2 :
      counter = tempCounters;
      break;
    case 3 :
      if (thirdMode == 0) {
        counter = tempCounters;
      }
      else {
        blinkCounter = tempCounters;
      }

      break;
    case 4 :
      counter = tempCounters;

      break;
    case 5 :
      if (fifthMode == 0) {
        counter = tempCounters;
      }
      else {
        if (tempCounters <= 255 && tempCounters >= 0)BRIGHTNESS = tempCounters;

      }
      break;

  }
  if (counter < 1) counter = UPPPERLIMITOFOPTIONS;

  if (counter > UPPPERLIMITOFOPTIONS)  counter = 1;

}
