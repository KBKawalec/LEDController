#include "OLED.h"
#include "LEDController.h"
#include "rotaryEncoderInterrupt.h"
SSD1306AsciiWire oled;

//const char OLEDMessages[UPPPER_LIMIT_OF_OPTIONS][2] PROGMEM = {{, "Rainbow"}, {"Dynamic"}, {"Blink"}, {"Candle Light"}, {"Brightness"}};


const char string_1[] PROGMEM = "Rainbow";
const char string_2[] PROGMEM = "Dynamic";
const char string_3[] PROGMEM = "Blink";
const char string_4[] PROGMEM = "Candle Light";
const char string_5[] PROGMEM = "Gradient";
const char string_6[] PROGMEM = "Brightness";
char buffer[30]; // buffer for reading the string to (needs to be large enough to take the longest string

// Initialize Table of Strings
const char* const string_table[] PROGMEM = { string_1, string_2, string_3, string_4, string_5, string_6 };

void initializeOLED() {
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
  for (int i = 0; i < UPPPER_LIMIT_OF_OPTIONS; i++) {
    if (counter == i) {
      if (whichMode[i] == 0) {
        strcpy_P(buffer, (char*)pgm_read_dword(&(string_table[i])));
        oled.println(buffer);
        
      }
      else if (i == BLINK_MODE || i == BRIGHTNESS_MODE) {
        oled.println(modeCounter[i]);
       
      }
      else {
        oled.println("TODO");
        
      }
      break;

    }

  }

}
