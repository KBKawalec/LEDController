#include "rotaryEncoderInterrupt.h"
#include "NRF.h"
#include "LEDController.h"



volatile byte whichMode[UPPPER_LIMIT_OF_OPTIONS];
volatile byte modeCounter[UPPPER_LIMIT_OF_OPTIONS];

volatile char counter = 0;
volatile byte buttonCounter;


byte pinAstateCurrent = LOW;                // Current state of Pin A
byte pinAStateLast = pinAstateCurrent;
unsigned long int lastButtonEvent;
byte previousStateRotary; // Used to determine which direction the rotary encoder was moving before as it is neccessary to detect a change in direction
unsigned long int lastRotaryEvent;


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

  buttonCounter = 0;

  attachInterrupt(digitalPinToInterrupt(outputB), rotaryInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switchPin), buttonInterrupt, FALLING);

}

void rotaryInterrupt() {
  if (millis() - lastRotaryEvent > 0) {

    int tempCounters = 0;


    // ROTATION DIRECTION
    delayMicroseconds(10);
    pinAstateCurrent = digitalRead(outputA);    // Read the current state of Pin A
    delayMicroseconds(10);
    if ((previousStateRotary == 1 && pinAStateLast == HIGH && pinAstateCurrent == LOW
         && digitalRead(outputB) == HIGH)) {

      if ( counter == BRIGHTNESS_MODE && whichMode[BRIGHTNESS_MODE]) tempCounters = tempCounters - BRIGHTNESS_AMOUNT;
      else tempCounters--;
      previousStateRotary = 0;

    }

    // If there is a minimal movement of 1 step
    else if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {

      if (digitalRead(outputB) == HIGH) {      // If Pin B is HIGH
        if ( counter == BRIGHTNESS_MODE && whichMode[BRIGHTNESS_MODE]) tempCounters = tempCounters + BRIGHTNESS_AMOUNT;
        else tempCounters++;
        previousStateRotary = 1;
      }
      else {
        if ( counter == BRIGHTNESS_MODE && whichMode[BRIGHTNESS_MODE]) tempCounters = tempCounters - BRIGHTNESS_AMOUNT;
        else tempCounters--;
        previousStateRotary = 0;
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
        break;

      }

    }

    if (counter < 0) counter = UPPPER_LIMIT_OF_OPTIONS - 1;

    else if (counter > UPPPER_LIMIT_OF_OPTIONS - 1)  counter = 0;
    
    lastRotaryEvent = millis();
  }

}


void buttonInterrupt() {
  if (millis() - lastButtonEvent > 200) {
    buttonCounter = ( buttonCounter + 1 ) % 2;
    // firstMode = 0;
    for (int i = 0; i < UPPPER_LIMIT_OF_OPTIONS; i++) {
      if (counter == i) {
        whichMode[i] = (whichMode[i] + 1) % 2;
      }
      else {
        whichMode[i] = 0;
      }
    }
    lastButtonEvent = millis();
  }
}
