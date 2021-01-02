#include "rotaryEncoderInterrupt.h"
#include "NRF.h"
#include "LEDController.h"

char counter = -1;

byte whichMode[UPPPERLIMITOFOPTIONS];
byte modeCounter[UPPPERLIMITOFOPTIONS];


byte switchCounter;
unsigned long int lastSwitch;

byte pinAstateCurrent = LOW;                // Current state of Pin A
byte pinAStateLast = pinAstateCurrent;

byte previousState;

void initializeRotaryParamaters() {
  modeCounter[BLINKMODE] = 50;
  modeCounter[BRIGHTNESSMODE] = 255;
  pinMode (outputA, INPUT);
  delayMicroseconds(10);
  pinMode (outputB, INPUT);
  delayMicroseconds(10);
  digitalWrite (outputA, HIGH);
  delayMicroseconds(10);
  digitalWrite (outputB, HIGH);
  delayMicroseconds(10);
  digitalWrite (switchPin, HIGH);
  
  switchCounter = 0;
  
  attachInterrupt(digitalPinToInterrupt(outputB), rotaryInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switchPin), buttonInterrupt, FALLING);

}

void rotaryInterrupt() {

  int tempCounters;
  for (int i = 0; i < UPPPERLIMITOFOPTIONS; i++) {
    if (counter == i) {
      if (whichMode[i] == 0) {
        tempCounters = counter;
      }
      else if (i == BLINKMODE || i == BRIGHTNESSMODE) {
        tempCounters = modeCounter[i];
      }

    }

  }

  // ROTATION DIRECTION
  delayMicroseconds(10);
  pinAstateCurrent = digitalRead(outputA);    // Read the current state of Pin A
  delayMicroseconds(10);
  if ((previousState == 1 && pinAStateLast == 1 && pinAstateCurrent == 0
       && digitalRead(outputB) == 1)) {

    if ( counter == BRIGHTNESSMODE && whichMode[BRIGHTNESSMODE]) tempCounters = tempCounters - BRIGHTNESSAMOUNT;
    else tempCounters--;
    previousState = 0;

  }

  delayMicroseconds(10);
  // If there is a minimal movement of 1 step
  if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {

    if (digitalRead(outputB) == HIGH) {      // If Pin B is HIGH
      if ( counter == BRIGHTNESSMODE && whichMode[BRIGHTNESSMODE]) tempCounters = tempCounters + BRIGHTNESSAMOUNT;
      else tempCounters++;
      previousState = 1;
    }
    else {
      if ( counter == BRIGHTNESSMODE && whichMode[BRIGHTNESSMODE]) tempCounters = tempCounters - BRIGHTNESSAMOUNT;
      else tempCounters--;
      previousState = 0;
    }

  }

  pinAStateLast = pinAstateCurrent;        // Store the latest read value in the currect state variable

  for (int i = 0; i < UPPPERLIMITOFOPTIONS; i++) {
    if (counter == i) {
      if (whichMode[i] == 0) {
        counter = tempCounters;
      }
      else {
        if (modeCounter[i] <= 255 && modeCounter[i] >= 0)modeCounter[i] = tempCounters;

      }

    }

  }

  if (counter < 0) counter = UPPPERLIMITOFOPTIONS - 1;

  else if (counter > UPPPERLIMITOFOPTIONS - 1)  counter = 0;

}




void buttonInterrupt() {
  if (millis() - lastSwitch > 200) {
    switchCounter = ( switchCounter + 1 ) % 2;
    // firstMode = 0;
    for (int i = 0; i < UPPPERLIMITOFOPTIONS; i++) {
      if (counter == i) {
        whichMode[i] = (whichMode[i] + 1) % 2;
      }
      else {
        whichMode[i] = 0;
      }
    }
    lastSwitch = millis();
  }
}
