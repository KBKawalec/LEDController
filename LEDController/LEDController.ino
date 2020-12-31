
#include "LEDController.h"
#include "NRF.h"
#include "OLED.h"



void setup() {
  Serial.begin(9600);
  initialize();
  delay(1000);


}
byte tempCount = counter;
byte tempSwitchCount = switchCounter;
byte tempBlinkCounter = blinkCounter;
byte BRIGHTNESSFlag = BRIGHTNESS;
void loop() {
  
  
  unsigned long newt = millis();
  unsigned int tempKeys2 = GetKeys2();
  unsigned int tempKeys1 = GetKeys1();
  //  Serial.print(counter);
  //  Serial.print(blinkCounter);




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
  if (tempKeys2 <= 65533 || tempKeys1 <= 65533 || tempCount != counter || tempSwitchCount != switchCounter || tempBlinkCounter != blinkCounter || BRIGHTNESSFlag != BRIGHTNESS) {
    setNRF();
    NRFTransmit();
  }
  if (tempCount != counter || tempSwitchCount != switchCounter || tempBlinkCounter != blinkCounter || BRIGHTNESSFlag != BRIGHTNESS) {
    displayOLED();
    tempCount = counter;
    tempSwitchCount = switchCounter;
    tempBlinkCounter = blinkCounter;
    BRIGHTNESSFlag = BRIGHTNESS;
  }
  // Serial.println(millis() - newt);
  for (int i = 0; i < 8; i++) {
    if (touchkeys.buttonstate[i] == 1) {

      leds[touchkeys.LEDNum[i]] = CRGB(0, 255, 200);
    }
    else {
      leds[touchkeys.LEDNum[i]] = CRGB(255, 200, 0);
    }
  }
  FastLED.show();

  //  Serial.println(millis() - newt);
  //  Serial.println("----------------------------------");
  //  delay(500);

}
