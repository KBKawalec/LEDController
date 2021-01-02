#include "NRF.h"
#include "LEDController.h"
#include "RF24.h"
#include "rotaryEncoderInterrupt.h"



//NRF
RF24 myRadio (10, A1);
byte addresses[6] = {'j', 'j', 'k', 'a', 'b', 'e'};
Package data;

void initializeNRF() {
  //NRF Initialization
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
  myRadio.openWritingPipe( addresses);
}

void transmitNRF() {
  myRadio.write(&data, sizeof(data));
}
int numberOfLEDS = 30;
void setNRF() {
  data.red = button3 - 7;
  data.green = 8 - button1;
  data.blue = button2;
  data.counter = counter;
  memcpy(data.modeCounter, modeCounter, sizeof(data.modeCounter));
  memcpy(data.buttonState, touchkeys.buttonstate, sizeof(data.buttonState));

}
