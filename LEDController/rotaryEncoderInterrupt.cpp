#include "rotaryEncoderInterrupt.h"
#include "NRF.h"
#include "LEDController.h"

char counter = -1;

byte whichMode[UPPPER_LIMIT_OF_OPTIONS];
byte modeCounter[UPPPER_LIMIT_OF_OPTIONS];


byte switchCounter;
unsigned long int lastSwitch;

byte pinAstateCurrent = LOW;                // Current state of Pin A
byte pinAStateLast = pinAstateCurrent;

byte previousState;

void initializeRotaryParamaters() {
  modeCounter[BLINK_MODE] = BLINK_START;
  modeCounter[BRIGHTNESS_MODE] = BRIGHTNESS_START;

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

  int tempCounters = 0;
  //  for (int i = 0; i < UPPPER_LIMIT_OF_OPTIONS; i++) {
  //    if (counter == i) {
  //      if (whichMode[i] == 0) {
  //        tempCounters = counter;
  //      }
  //      else if (i == BLINK_MODE || i == BRIGHTNESS_MODE) {
  //        tempCounters = modeCounter[i];
  //      }
  //
  //    }
  //
  //  }

  // ROTATION DIRECTION
  delayMicroseconds(10);
  pinAstateCurrent = digitalRead(outputA);    // Read the current state of Pin A
  delayMicroseconds(10);
  if ((previousState == 1 && pinAStateLast == 1 && pinAstateCurrent == 0
       && digitalRead(outputB) == 1)) {

    if ( counter == BRIGHTNESS_MODE && whichMode[BRIGHTNESS_MODE]) tempCounters = tempCounters - BRIGHTNESS_AMOUNT;
    else tempCounters--;
    previousState = 0;

  }

  delayMicroseconds(10);
  // If there is a minimal movement of 1 step
  if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {

    if (digitalRead(outputB) == HIGH) {      // If Pin B is HIGH
      if ( counter == BRIGHTNESS_MODE && whichMode[BRIGHTNESS_MODE]) tempCounters = tempCounters + BRIGHTNESS_AMOUNT;
      else tempCounters++;
      previousState = 1;
    }
    else {
      if ( counter == BRIGHTNESS_MODE && whichMode[BRIGHTNESS_MODE]) tempCounters = tempCounters - BRIGHTNESS_AMOUNT;
      else tempCounters--;
      previousState = 0;
    }

  }

  pinAStateLast = pinAstateCurrent;        // Store the latest read value in the currect state variable

  for (int i = 0; i < UPPPER_LIMIT_OF_OPTIONS; i++) {
    if (counter == i) {
      if (whichMode[i] == 0) {
        counter = counter + tempCounters;
      }
      else if (i == BLINK_MODE || i == BRIGHTNESS_MODE) {
        modeCounter[i] = modeCounter[i] + tempCounters;
        if (modeCounter[i] > 255 )  modeCounter[i] = 255;
        else if (modeCounter[i] < 0) modeCounter[i] = 0;

      }

    }

  }

  if (counter < 0) counter = UPPPER_LIMIT_OF_OPTIONS - 1;

  else if(counter > UPPPER_LIMIT_OF_OPTIONS - 1)  counter = 0;

}


void buttonInterrupt() {
  if (millis() - lastSwitch > 200) {
    switchCounter = ( switchCounter + 1 ) % 2;
    // firstMode = 0;
    for (int i = 0; i < UPPPER_LIMIT_OF_OPTIONS; i++) {
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
