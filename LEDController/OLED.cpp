#include "OLED.h"
#include "LEDController.h"
#include "rotaryEncoderInterrupt.h"
SSD1306AsciiWire oled;



void OLEDInitialize() {
  Wire.begin();
  Wire.setClock(400000L);

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  oled.setFont(Adafruit5x7);
}


void displayOLED() {
  oled.clear();
  oled.setCursor(0, 4);
  oled.set2X();

  switch (counter) {
    case 1 :
      if (firstMode == 0) {
        oled.println(F("Gradient"));
      }
      else {
        oled.println(F("Rainbow"));

      }


      break;
    case 2 :
      oled.println(F("Dynamic"));
      break;
    case 3 :
      if (thirdMode == 0) {
        oled.println(F("Blink"));
      }
      else {
        oled.println(blinkCounter);

      }
      break;
    case 4 :
      oled.println(F("Candle"));
      oled.println(F("light"));
      break;
    case 5 :
      if (fifthMode == 0) {
        oled.println(F("Brightness"));
      }
      else {
        oled.println(BRIGHTNESS);

      }


      break;

  }
}
