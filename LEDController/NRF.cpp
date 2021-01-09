#include "NRF.h"
#include "LEDController.h"
#include "RF24.h"
#include "rotaryEncoderInterrupt.h"



const byte numSlaves = 2;
const byte slaveAddress[numSlaves][6] = {
  // each slave needs a different address
  {'j', 'j', 'k', 'a', 'b', 'e'},
  {'j', 'j', 'k', 'a', 'b', 'F'}
};



//NRF
RF24 myRadio (10, A1);
//byte addresses[6] = ;
Package data;

void initializeNRF() {
  //NRF Initialization
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
  // myRadio.openWritingPipe( addresses);
}

void transmitNRF() {
  for (int i = 0; i < numSlaves; i++) {

    myRadio.openWritingPipe(slaveAddress[i]);
    myRadio.write(&data, sizeof(data));

  }

}

void setNRF() {
  data.red = button3 - 7;
  data.green = 8 - button1;
  data.blue = button2;
  data.counter = counter;
  memcpy(data.modeCounter, modeCounter, sizeof(data.modeCounter));
  memcpy(data.buttonState, touchkeys.buttonstate, sizeof(data.buttonState));

}
