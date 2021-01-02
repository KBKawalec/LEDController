#ifndef _OLED_H
#define _OLED_H
#include <Arduino.h>


#include "string.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "LEDController.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

extern void initializeOLED();
extern void displayOLED();




#endif
