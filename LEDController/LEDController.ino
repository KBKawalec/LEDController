
#include "LEDController.h"
#include "NRF.h"
#include "OLED.h"
#include "rotaryEncoderInterrupt.h"

byte tempCount = counter;
byte tempSwitchCount = switchCounter;
byte tempBlinkCounter = modeCounter[BLINKMODE];
byte BRIGHTNESSFlag = modeCounter[BRIGHTNESSMODE];


void setup() {
  Serial.begin(9600);
  initialize();
  delay(100);


}

void loop() {



  unsigned long newt = millis();
  unsigned int tempKeys2 = GetKeys2();
  unsigned int tempKeys1 = GetKeys1();


  if (tempKeys2 <= 65533) {
    getSlider3();
    getSlider2();
  }
  if (tempKeys1 <= 65533) {
    getSlider1();
    getTouchpad();
  }
  if (tempKeys2 <= 65533 || tempKeys1 <= 65533) {

    resetLEDS();
    setSlider1();
    setSlider3();
    setSlider2();

  }
  if (tempKeys2 <= 65533 || tempKeys1 <= 65533 || tempCount != counter || tempSwitchCount != switchCounter || tempBlinkCounter != modeCounter[BLINKMODE] || BRIGHTNESSFlag != modeCounter[BRIGHTNESSMODE]) {
    setNRF();
    transmitNRF();
  }
  if (tempCount != counter || tempSwitchCount != switchCounter || tempBlinkCounter != modeCounter[BLINKMODE] || BRIGHTNESSFlag != modeCounter[BRIGHTNESSMODE]) {
    displayOLED();
    tempCount = counter;
    tempSwitchCount = switchCounter;
    tempBlinkCounter = modeCounter[BLINKMODE];
    BRIGHTNESSFlag = modeCounter[BRIGHTNESSMODE];
  }
  
  setTouchPad();
  FastLED.show();

  //  Serial.println(millis() - newt);
  //  Serial.println("----------------------------------");
  //  delay(500);

}
