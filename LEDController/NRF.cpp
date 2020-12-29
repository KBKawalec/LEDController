#include "NRF.h"
#include "LEDController.h"
#include "RF24.h"


//NRF
RF24 myRadio (10, A1);
byte addresses[6] = {'j', 'j', 'k', 'a', 'b', 'e'};
Package data;

void NRFinitialization(){
    //NRF Initialization
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
  myRadio.openWritingPipe( addresses);
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
  data.firstMode = firstMode;
  data.thirdMode = thirdMode;
  memcpy(data.buttonState, touchkeys.buttonstate, sizeof(data.buttonState));

}
