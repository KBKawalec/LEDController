
#include "LEDController.h"


CRGB leds[NUM_LEDS];
int button1 = 8; // So that it starts out off
int button2;
int button3;
int button4;
unsigned int keys;
unsigned int keys2;


//NRF
RF24 myRadio (10, A1);
byte addresses[6] = {'j', 'j', 'k', 'a', 'b', 'e'};
Package data;

//Rotary encoder

//int aState;
//int aLastState;
//int flag = 1;


int LEDvalues[8] = {4, 5, 6, 7, 3, 2, 1, 0};
Touchkeys touchkeys;

SSD1306AsciiWire oled;

int pinAstateCurrent = LOW;                // Current state of Pin A
int pinAStateLast = pinAstateCurrent;
int counter = 1;
int previousState;

int blinkCounter = 50;

int switchCounter;
unsigned long int lastSwitch;
void updateSwitch() {
  if (millis() - lastSwitch > 200) {
    switchCounter = ( switchCounter + 1 ) % 2;
    lastSwitch = millis();
  }
}


void updatea() {

  /* WARNING: For this example I've used Serial.println within the interrupt callback. The Serial
     library already uses interrupts which could cause errors. Therefore do not use functions
     of the Serial libray in your interrupt callback.
  */
  int tempCounters;
  switch (counter) {
    case 1 :
      tempCounters = counter;
      break;
    case 2 :
      tempCounters = counter;
      break;
    case 3 :
      if (switchCounter == 0) {
        tempCounters = counter;
      }
      else {
        tempCounters = blinkCounter;
      }

      break;
    case 4 :
      tempCounters = counter;

      break;

  }

  // ROTATION DIRECTION
  pinAstateCurrent = digitalRead(outputA);    // Read the current state of Pin A
  if ((previousState == 1 && pinAStateLast == 1 && pinAstateCurrent == 0
       && digitalRead(outputB) == 1)) {

    tempCounters--;
    previousState = 0;

  }


  // If there is a minimal movement of 1 step
  if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {

    if (digitalRead(outputB) == HIGH) {      // If Pin B is HIGH
      tempCounters++;
      previousState = 1;
    }
    else {
      tempCounters--;
      previousState = 0;
    }

  }

  pinAStateLast = pinAstateCurrent;        // Store the latest read value in the currect state variable
  switch (counter) {
    case 1 :
      counter = tempCounters;
      break;
    case 2 :
      counter = tempCounters;
      break;
    case 3 :
      if (switchCounter == 0) {
        counter = tempCounters;
      }
      else {
        blinkCounter = tempCounters;
      }

      break;
    case 4 :
      counter = tempCounters;

      break;

  }
  if (counter < 1) counter = 4;

  if (counter > 4)  counter = 1;

}

void displayOLED() {
  oled.clear();
  oled.setCursor(0, 4);
  oled.set2X();

  switch (counter) {
    case 1 :
      oled.println(F("Static"));
      break;
    case 2 :
      oled.println(F("Dynamic"));
      break;
    case 3 :
      if (switchCounter == 0) {
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

  }
}

void initialize() {


  pinMode(KEYS, INPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(KEYS2, INPUT);
  pinMode(CLOCK2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Wire.begin();
  Wire.setClock(400000L);

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  oled.setFont(Adafruit5x7);




  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);
  digitalWrite (outputA, HIGH);
  digitalWrite (outputB, HIGH);

  digitalWrite (switchPin, HIGH);
  switchCounter = 1;






  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  for (int i = 0; i < 8; i++) {

    touchkeys.LEDNum[i] = LEDvalues[i];
    leds[i] = CRGB(0, 255, 200);

    touchkeys.buttonstate[i] = 1;

    delay(50);
    FastLED.show();
  }


  //NRF Initialization
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
  myRadio.openWritingPipe( addresses);

  attachInterrupt(digitalPinToInterrupt(outputB), updatea, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switchPin), updateSwitch, FALLING);

  delay(100);
}



void NRFTransmit() {
  myRadio.write(&data, sizeof(data));
}

void setNRF() {
  data.red = button3 - 7;
  data.green = 8 - button1;
  data.blue = button2;
  data.counter = counter;
  data.switchPinValue = switchCounter;
  data.blinkCounterValue = blinkCounter;
  memcpy(data.buttonState, touchkeys.buttonstate, sizeof(data.buttonState));

}
unsigned int GetKeys2()
{
  keys = 0;
  byte currbit = LOW;
  delay(3); //after 2ms without clock, the TTP229 resets,
  //so, this ensures to get the 16 keys in the expected order

  for (byte i = 0; i < 16; i++)
  {
    digitalWrite(CLOCK, HIGH);
    delayMicroseconds(10);
    digitalWrite(CLOCK, LOW);
    delayMicroseconds(10);
    currbit = digitalRead(KEYS);
    if (currbit == HIGH) { //= If active High is set. Otherwise, invert the test
      keys = keys * 2 + 1;
    }
    else {
      keys = keys * 2;
    }
  }
  return keys;
}
unsigned int GetKeys1()
{
  keys2 = 0;
  byte currbit = LOW;
  delay(3); //after 2ms without clock, the TTP229 resets,
  //so, this ensures to get the 16 keys in the expected order

  for (byte i = 0; i < 16; i++)
  {
    digitalWrite(CLOCK2, HIGH);
    delayMicroseconds(10);
    digitalWrite(CLOCK2, LOW);
    delayMicroseconds(10);
    currbit = digitalRead(KEYS2);
    if (currbit == HIGH) { //= If active High is set. Otherwise, invert the test
      keys2 = keys2 * 2 + 1;
    }
    else {
      keys2 = keys2 * 2;
    }
  }
  return keys2;
}

int capactiveValues[8] = {65279, 65407, 65471, 65503, 65519, 65527, 65531, 65533};
unsigned int touchpadkey;
void getTouchpad() {
  touchpadkey = GetKeys1();
  for (int i = 0; i < 8; i++) {
    if (touchpadkey == capactiveValues[i]) {
      if (millis() - touchkeys.old_time[i] > 500) {
        touchkeys.buttonstate[i] += 1;
        touchkeys.buttonstate[i] = touchkeys.buttonstate[i] % 2;

        touchkeys.old_time[i] = millis();
      }
      else {

      }
    }
  }

}
unsigned long new_time3 = millis();
unsigned int slider3;
bool pressed3 = false;
void getSlider3() {
  slider3 = GetKeys2();
  for (int i = 6; i >= 0 ; i--) {
    if (slider3 == capactiveValues[i]) {
      if (  i == 0 ) {

        if (pressed3 == false && (millis() - new_time3 > 400 )) {
          button3 = i + 8;
          new_time3 = millis();
          pressed3 = true;
        }
        else {
          if ( (millis() - new_time3 > 400 )) {
            button3--;
            new_time3 = millis();
            pressed3 = false;
          }
        }

      }
      else {

        button3 = i + 8;
      }
      break;


    }
  }
}
int capactiveValues2[7] = {32767, 49151, 57343, 61439, 63487, 64511, 65023};
unsigned long new_time1 = millis();
unsigned int slider2;
bool pressed = false;
void getSlider2() {
  slider2 = GetKeys2();
  for (int i = 6; i >= 0 ; i--) {
    if (slider2 == capactiveValues2[i]) {

      if (  i == 6 ) {

        if (pressed == false && (millis() - new_time1 > 400 )) {
          button1 = i + 1;
          new_time1 = millis();
          pressed = true;
        }
        else {
          if ( (millis() - new_time1 > 400 )) {
            button1++;
            new_time1 = millis();
            pressed = false;
          }
        }

      }
      else {

        button1 = i + 1;
      }
      break;


    }




  }
}

unsigned long new_time2 = millis();
unsigned int slider1;
bool pressed2 = false;
void getSlider1() {
  slider1 = GetKeys1();
  for (int i = 6; i >= 0 ; i--) {
    if (slider1 == capactiveValues2[i]) {
      if (  i == 0 ) {

        if (pressed2 == false && (millis() - new_time2 > 400 )) {
          button2 = i + 1;
          new_time2 = millis();
          pressed2 = true;
        }
        else {
          if ( (millis() - new_time2 > 400 )) {
            button2--;
            new_time2 = millis();
            pressed2 = false;
          }
        }

      }
      else {

        button2 = i + 1;
      }
      break;


    }
  }
}

void resetLEDS() {
  for (int i = 8; i < NUM_LEDS; i++) {

    leds[i] = CRGB ( 0, 0, 0);
  }
}

void setSlider1() {
  for (int j = 21; j > button1 + 13; j--) {

    leds[j] = CRGB ( 0, 100 , 0);

  }
}

void setSlider2() {

  for (int i = 8; i < button2 + 8; i++) {

    leds[i] = CRGB ( 0, 0, 100);


  }
}

void setSlider3() {
  for (int i = 22; i < button3 + 15; i++) {

    leds[i] = CRGB ( 100, 0 , 0);

  }

}
