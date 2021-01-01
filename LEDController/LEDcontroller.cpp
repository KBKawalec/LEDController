
#include "LEDController.h"
#include "NRF.h"
#include "OLED.h"
#include "rotaryEncoderInterrupt.h"


CRGB leds[NUM_LEDS];
byte button1 = 8; // So that it starts out off
byte button2;
byte button3 = 7;
byte button4;
unsigned int keys;
unsigned int keys2;

byte LEDvalues[8] = {4, 5, 6, 7, 0, 1,2, 3};
Touchkeys touchkeys;


void initialize() {


  pinMode(KEYS, INPUT);
  delayMicroseconds(10);
  pinMode(CLOCK, OUTPUT);
  delayMicroseconds(10);
  pinMode(KEYS2, INPUT);
  delayMicroseconds(10);
  pinMode(CLOCK2, OUTPUT);
  delayMicroseconds(10);
  pinMode(LED_PIN, OUTPUT);

  OLEDInitialize(); // initialization of parameters for OLED
  initializeRotaryParamaters();


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

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  for (int i = 0; i < 8; i++) {

    touchkeys.LEDNum[i] = LEDvalues[i];
    leds[i] = CRGB(0, 255, 200);

    touchkeys.buttonstate[i] = 1;

    delay(50);
    FastLED.show();
  }


  //Initialzing the NRF
  NRFinitialization();

  attachInterrupt(digitalPinToInterrupt(outputB), updatea, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switchPin), updateSwitch, FALLING);

  delay(100);
}




unsigned int GetKeys2()
{
  keys = 0;
  byte currbit = LOW;
  delay(3); //after 2ms without clock, the TTP229 resets,
  //so, this ensures to get the 16 keys in the expected order

  for (byte i = 0; i < 16; i++)
  {
    digitalWrite(CLOCK, HIGH);
    delayMicroseconds(10);
    digitalWrite(CLOCK, LOW);
    delayMicroseconds(10);
    currbit = digitalRead(KEYS);
    if (currbit == HIGH) { //= If active High is set. Otherwise, invert the test
      keys = keys * 2 + 1;
    }
    else {
      keys = keys * 2;
    }
  }
  return keys;
}
unsigned int GetKeys1()
{
  keys2 = 0;
  byte currbit = LOW;
  delay(3); //after 2ms without clock, the TTP229 resets,
  //so, this ensures to get the 16 keys in the expected order

  for (byte i = 0; i < 16; i++)
  {
    digitalWrite(CLOCK2, HIGH);
    delayMicroseconds(10);
    digitalWrite(CLOCK2, LOW);
    delayMicroseconds(10);
    currbit = digitalRead(KEYS2);
    if (currbit == HIGH) { //= If active High is set. Otherwise, invert the test
      keys2 = keys2 * 2 + 1;
    }
    else {
      keys2 = keys2 * 2;
    }
  }
  return keys2;
}

int capactiveValues[8] = {65279, 65407, 65471, 65503,65533,65531,65527, 65519};
unsigned int touchpadkey;
void getTouchpad() {
  touchpadkey = GetKeys1();
  for (int i = 0; i < 8; i++) {
    if (touchpadkey == capactiveValues[i]) {
      if (millis() - touchkeys.old_time[i] > 500) {
        touchkeys.buttonstate[i] += 1;
        touchkeys.buttonstate[i] = touchkeys.buttonstate[i] % 2;

        touchkeys.old_time[i] = millis();
      }
      else {

      }
    }
  }

}
unsigned long new_time3 = millis();
int capactiveValues3[8] = {65279, 65407, 65471, 65503,65519,65527,65531};
unsigned int slider3;
void getSlider3() {
  slider3 = GetKeys2();
  for (int i = 6; i >= 0 ; i--) {
    if (slider3 == capactiveValues3[i]) {
      if (  i == 0 ) {

        if ( (millis() - new_time3 > 400 ) ) {
          if (button3 !=  8) {
            button3 = i + 8;
          }
          else {
            button3--;
          }
          new_time3 = millis();

        }

      }
      else {

        button3 = i + 8;
      }
      break;


    }
  }
}
int capactiveValues2[7] = {32767, 49151, 57343, 61439, 63487, 64511, 65023};
unsigned long new_time1 = millis();
unsigned int slider2;
void getSlider2() {
  slider2 = GetKeys2();
  for (int i = 6; i >= 0 ; i--) {
    if (slider2 == capactiveValues2[i]) {

      if (  i == 6 ) {

        if ((millis() - new_time1 > 400 )  ) {
          if (button1 != 7) {
            button1 = i + 1;
          }
          else {
            button1++;

          }
          new_time1 = millis();

        }

      }
      else {

        button1 = i + 1;
      }
      break;

    }
  }
}

unsigned long new_time2 = millis();
unsigned int slider1;
void getSlider1() {
  slider1 = GetKeys1();
  for (int i = 6; i >= 0 ; i--) {
    if (slider1 == capactiveValues2[i]) {
      if (  i == 0 ) {

        if ((millis() - new_time2 > 400 )) {
          if ( button2 != 1) {
            button2 = i + 1;
          }
          else {
            button2--;

          }
          new_time2 = millis();

        }

      }
      else {

        button2 = i + 1;
      }
      break;
    }
  }
}

void resetLEDS() {
  for (int i = 8; i < NUM_LEDS; i++) {

    leds[i] = CRGB ( 0, 0, 0);
  }
}

void setSlider1() {
  for (int j = 21; j > button1 + 13; j--) {

    leds[j] = CRGB ( 0, 100 , 0);

  }
}

void setSlider2() {

  for (int i = 8; i < button2 + 8; i++) {

    leds[i] = CRGB ( 0, 0, 100);


  }
}

void setSlider3() {
  for (int i = 22; i < button3 + 15; i++) {

    leds[i] = CRGB ( 100, 0 , 0);

  }

}
