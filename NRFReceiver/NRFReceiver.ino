#include "NRFReceiver.h"
#include "Fire.h"





int LEDstripLength = NUM_LEDS;
int segment = LEDstripLength / 8;
int previousRed;
int previousGreen;
int previousBlue;
int previousButtonState[8];

int count;
int stripPattern[8];
int stripColor[8][3];


CRGB leds[MAXLEDS];
Package data;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, MAXLEDS);
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS) ;
  myRadio.maskIRQ(1, 1, 0);
  myRadio.openReadingPipe(1, addresses);
  myRadio.startListening();
  attachInterrupt(digitalPinToInterrupt(3), interuptFunc, FALLING);
}

void interuptFunc() {
  if (myRadio.available()) {
    while (myRadio.available())
    { //Serial.println(data.switchPinValue);
      myRadio.read( &data, sizeof(data) );
    }

    Serial.println(BRIGHTNESS);
    //(previousRed !=   data.red || previousGreen !=   data.green || previousBlue !=   data.blue)
    if (  data.blue >= 0 && data.blue < 8 &&  data.red >= 0 && data.red < 8 && data.green >= 0 && data.green < 8) {
      for (int i = 0; i < sizeof(data.buttonState) / sizeof(data.buttonState[0]); i++) {
        if ( data.buttonState[i] == 1 ) {
          if (data.counter < 5) {
            stripPattern[i] = data.counter;
          }
          stripColor[i][0] = data.red;
          stripColor[i][1] = data.green;
          stripColor[i][2] = data.blue;

        }
        // if (data.NUM_LEDS > 0 && data.NUM_LEDS < 3000) {
        // NUM_LEDS = data.NUM_LEDS;
        // }
        if (data.BRIGHTNESS >= 0 && data.BRIGHTNESS <= 255) {
          BRIGHTNESS = data.BRIGHTNESS;
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
          doFire(stripColor[i][0] * BRIGHTNESS / 7 , stripColor[i][1] * BRIGHTNESS / 7, stripColor[i][2] * BRIGHTNESS / 7, ledStripLengthBufferBeg, LEDstripLength  * (i + 1) / 8);


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
DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,   //black
60,   255,  0,  0,   //red
120,   255,255,  0,   //bright yellow
180,   0,0,0 };
void staticLEDS(int i) {
  for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
       CRGBPalette16 gPal = CRGBPalette16( CRGB (stripColor[i][0] * BRIGHTNESS / 7 , stripColor[i][1] * BRIGHTNESS / 7, stripColor[i][2] * BRIGHTNESS / 7),
       CRGB (stripColor[i][2] * BRIGHTNESS / 7 , stripColor[i][0] * BRIGHTNESS / 7, stripColor[i][1] * BRIGHTNESS / 7),
        CRGB (stripColor[i][1] * BRIGHTNESS / 7 , stripColor[i][2] * BRIGHTNESS / 7, stripColor[i][0] * BRIGHTNESS / 7));
    //    CRGB (stripColor[i][1] * BRIGHTNESS / 7 , stripColor[i][2] * BRIGHTNESS / 7, stripColor[i][0] * BRIGHTNESS / 7),
    //    CRGB (stripColor[i][2] * BRIGHTNESS / 7 , stripColor[i][0] * BRIGHTNESS / 7, stripColor[i][1] * BRIGHTNESS / 7));
    //    CRGBPalette16 gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow);
    //    //byte colorindex = random8(0, 255);
    //    CRGB color = ColorFromPalette( gPal, 256/(j+2));
    //    Serial.println(256/(j+2));
    CRGBPalette16 myPal = heatmap_gp;
    uint8_t heatindex = (j * 255 / 29);
    leds[j] = ColorFromPalette( gPal, heatindex);
    //leds[j] = color;
  }
  FastLED.show();
}

void dynamicLEDS(int i) {
  paintLEDS(i);
  FastLED.show();

}
void blinkLEDS(int i) {
  paintLEDS(i);
  FastLED.show();

}
void paintLEDS(int i) {
  for (int j = LEDstripLength * i / 8; j < LEDstripLength  * (i + 1) / 8; j++) {
    leds[j] = CRGB (stripColor[i][0] * BRIGHTNESS / 7 , stripColor[i][1] * BRIGHTNESS / 7, stripColor[i][2] * BRIGHTNESS / 7);

  }
}
