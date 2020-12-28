#include "NRFReceiver.h"
#include "Fire.h"


CRGB leds[NUM_LEDS];

RF24 myRadio (7, 8);
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
};

byte addresses[6] = {'j', 'j', 'k', 'a', 'b', 'e'};

int LEDstripLength = NUM_LEDS;
int segment = LEDstripLength / 8;
int previousRed;
int previousGreen;
int previousBlue;
int previousButtonState[8];

typedef struct package Package;
Package data;
void setup()
{
  Serial.begin(9600);
  delay(1000);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS) ;
  myRadio.maskIRQ(1, 1, 0);
  myRadio.openReadingPipe(1, addresses);
  myRadio.startListening();
  attachInterrupt(digitalPinToInterrupt(3), interuptFunc, FALLING);
}
int count;
int stripPattern[8];
int stripColor[8][3];
void interuptFunc() {
  if (myRadio.available()) {
    while (myRadio.available())
    { //Serial.println(data.switchPinValue);
      myRadio.read( &data, sizeof(data) );
    }

    //(previousRed !=   data.red || previousGreen !=   data.green || previousBlue !=   data.blue)
    if (  data.blue >= 0 && data.blue < 8 &&  data.red >= 0 && data.red < 8 && data.green >= 0 && data.green < 8) {
      for (int i = 0; i < sizeof(data.buttonState) / sizeof(data.buttonState[0]); i++) {
        if ( data.buttonState[i] == 1) {
          stripPattern[i] = data.counter;
          stripColor[i][0] = data.red;
          stripColor[i][1] = data.green;
          stripColor[i][2] = data.blue;

        }

      }
      //      else {
      //        previousRed = data.red;
      //        previousGreen = data.green;
      //        previousBlue = data.blue;
      //      }

      //
      // Serial.println("packet recieved");
      //      Serial.println(previousGreen);
      //      Serial.println(previousBlue);
      previousRed = data.red;
      previousGreen = data.green;
      previousBlue = data.blue;
      //      Serial.println(previousRed);
      //      Serial.println(previousGreen);
      //      Serial.println(previousBlue);
      //      Serial.println("----------------------");







    }
  }


  count++;
}

void loop()
{


  int ledStripLengthBufferBeg =  0;


  for (int i = 0; i < sizeof(data.buttonState) / sizeof(data.buttonState[0]); i++) {

    //  Serial.println(stripPattern[i]);
    switch (stripPattern[i]) {

      case 1 :
        staticLEDS(i);

        break;
      case 2 :
        dynamicLEDS(i);
        break;
      case 3 :
        blinkLEDS(i);
        break;
      case 4 :
        if (i != sizeof(data.buttonState) / sizeof(data.buttonState[0]) - 1 && stripPattern[i + 1] == 4 && stripColor[i + 1][0] == stripColor[i][0] && stripColor[i + 1][1] == stripColor[i][1] && stripColor[i + 1][2] == stripColor[i][2]) {

          continue; // if this is a chain of connected segments then continue to find the whole chain


        }
        else {

          for (int j = i; j >= 0 ; j--) {
            if ( j == 0 ) {
              ledStripLengthBufferBeg =  0; // If the chain is broken we find the beginning of the chain
            }
            else if (stripPattern[j - 1] == 4 && stripColor[j - 1][0] == stripColor[j][0] && stripColor[j - 1][1] == stripColor[j][1] && stripColor[j - 1][2] == stripColor[j][2]) {
              continue;

            }
            else {
              ledStripLengthBufferBeg =  LEDstripLength * j / 8;// If the chain is broken we find the beginning of the chain
              break;
            }
          } // For each time the chain is broken we call the function so that there can be unconnected segments
          doFire(stripColor[i][0] * 255 / 7 , stripColor[i][1] * 255 / 7, stripColor[i][2] * 255 / 7, ledStripLengthBufferBeg, LEDstripLength  * (i + 1) / 8);


        }

        break;





    }
  }

  for (int i = 0; i < sizeof(data.buttonState) / sizeof(data.buttonState[0]); i++) {
    if (stripPattern[i] == 3 || stripPattern[i] == 4) {
      delay(data.blinkCounterValue);
      break;
    }
  }

  for (int i = 0; i < sizeof(data.buttonState) / sizeof(data.buttonState[0]); i++) {
    for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
      if ( stripPattern[i] == 3)
        leds[j] = CRGB (0, 0, 0);

    }


  }
  FastLED.show();
  for (int i = 0; i < sizeof(data.buttonState) / sizeof(data.buttonState[0]); i++) {
    if (stripPattern[i] == 3) {
      delay(data.blinkCounterValue);
      break;
    }
  }
}

void staticLEDS(int i) {
  for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
    leds[j] = CRGB (stripColor[i][0] * 255 / 7 , stripColor[i][1] * 255 / 7, stripColor[i][2] * 255 / 7);
  }
  FastLED.show();
}

void dynamicLEDS(int i) {
  for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
    leds[j] = CRGB (stripColor[i][0] * 255 / 7 , stripColor[i][1] * 255 / 7, stripColor[i][2] * 255 / 7);


  }
  FastLED.show();

}
void blinkLEDS(int i) {
  for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
    leds[j] = CRGB (stripColor[i][0] * 255 / 7 , stripColor[i][1] * 255 / 7, stripColor[i][2] * 255 / 7);

  }
  FastLED.show();


}
