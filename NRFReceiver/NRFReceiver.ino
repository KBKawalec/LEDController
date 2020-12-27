#include <SPI.h>
#include "RF24.h"
#include <FastLED.h>
#define NUM_LEDS    30
CRGB leds[NUM_LEDS];
#define LED_PIN     2
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
void interuptFunc() {
  if (myRadio.available()) {
    while (myRadio.available())
    { Serial.println(data.switchPinValue);
      myRadio.read( &data, sizeof(data) );
    }

    //(previousRed !=   data.red || previousGreen !=   data.green || previousBlue !=   data.blue)
    if (  data.blue >= 0 && data.blue < 8 &&  data.red >= 0 && data.red < 8 && data.green >= 0 && data.green < 8) {
      for (int i = 0; i < sizeof(data.buttonState) / sizeof(data.buttonState[0]); i++) {
        if ( data.buttonState[i] == 1) {
          stripPattern[i] = data.counter;

        }
      }

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

        break;





    }
  }
  delay(data.blinkCounterValue);
  for (int i = 0; i < sizeof(data.buttonState) / sizeof(data.buttonState[0]); i++) {
    for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
      if ( stripPattern[i] == 3)
        leds[j] = CRGB (0, 0, 0);

    }


  }
  FastLED.show();
  delay(data.blinkCounterValue);
}

void staticLEDS(int i) {
  for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
    leds[j] = CRGB (previousRed * 255 / 7 , previousGreen * 255 / 7, previousBlue * 255 / 7);
  }
  FastLED.show();
}

void dynamicLEDS(int i) {
  for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
    leds[j] = CRGB (previousRed * 255 / 7 , previousGreen * 255 / 7, previousBlue * 255 / 7);
    FastLED.show();

  }

}
void blinkLEDS(int i) {
  for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
    leds[j] = CRGB (previousRed * 255 / 7 , previousGreen * 255 / 7, previousBlue * 255 / 7);

  }
  FastLED.show();


}
