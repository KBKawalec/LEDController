
#include "LEDController.h"
#include "NRF.h"
#include "OLED.h"
#include "rotaryEncoderInterrupt.h"

byte tempCount = counter;
byte tempButtonCounter = buttonCounter;
byte tempBlinkCounter = modeCounter[BLINK_MODE];
byte BRIGHTNESSFlag = modeCounter[BRIGHTNESS_MODE];


void setup() {
  Serial.begin(115200);
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
  if (tempKeys2 <= 65533 || tempKeys1 <= 65533 || tempCount != counter || tempButtonCounter != buttonCounter || tempBlinkCounter != modeCounter[BLINK_MODE] || BRIGHTNESSFlag != modeCounter[BRIGHTNESS_MODE]) {
    setNRF();
    transmitNRF();
  }
  if (tempCount != counter || tempButtonCounter != buttonCounter || tempBlinkCounter != modeCounter[BLINK_MODE] || BRIGHTNESSFlag != modeCounter[BRIGHTNESS_MODE]) {
    displayOLED();
    tempCount = counter;
    tempButtonCounter = buttonCounter;
    tempBlinkCounter = modeCounter[BLINK_MODE];
    BRIGHTNESSFlag = modeCounter[BRIGHTNESS_MODE];
  }
  
  setTouchPad();
  FastLED.show();

  //  Serial.println(millis() - newt);
  //  Serial.println("----------------------------------");
  //  delay(500);

}
