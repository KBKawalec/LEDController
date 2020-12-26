
#ifndef _LEDCONTROLLER_H
#define _LEDCONTROLLER_H
#include <Arduino.h>
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>

#include "string.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

#include "RF24.h"






//Rotary encoder pins
#define outputA 7
#define outputB 3

#define switchPin 2


#define KEYS 4
#define CLOCK 5
#define KEYS2 6
#define CLOCK2 8
#define LED_PIN     9
#define NUM_LEDS    29
#define DELAY_TIME 100
#define MAXBRIGHT 100
#define speed 1

//NRF

extern void NRFTransmit();
extern void setNRF();

struct package
{
  int red;
  int green;
  int blue;
  int buttonState[8];
  int counter;
  int switchPinValue;
};
typedef struct package Package;
extern Package data;

// initializs pins and values
extern void initialize();

//Gets values from the capactive touchpads
extern unsigned int GetKeys1();
extern unsigned int GetKeys2();
//Converts value to number for LED
extern void getSlider3();
extern void getSlider2();
extern void getSlider1();
extern void getTouchpad();

//Used to show current option on LCD
extern void showDisplay();
extern void displayOLED();

//Triggers a rotary encoder movement
extern void triggerRotary();
extern void setRotary();

// Resets to black and then sets the value of the slider to the LED light
extern void resetLEDS();
extern void setSlider1();
extern void setSlider2();
extern void setSlider3();

//Global value to hold current value for each slider and for touchpad
extern int button1;
extern int button2;
extern int button3;
extern int button4;
// used for calculating capactive touchpad
extern unsigned int keys;
extern unsigned int keys2;

// LED Strip variable
extern CRGB leds[NUM_LEDS];


// Tracks state of the touchpad to know when off or when on
struct Touchkeys {
  int buttonstate[8];
  int ledState[8];
  int LEDNum[8];
  unsigned long old_time[8];
  
};
extern Touchkeys touchkeys;


//Used for the rotary encoder and to keep track of the current value (counter) 
extern int counter;
extern int switchCounter;


#endif
