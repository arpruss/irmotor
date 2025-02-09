
#include <IRremote.hpp>

const uint32_t in1 = PA1;
const uint32_t in2 = PA2;

// scrounged from ir heli: left=signal, center (red wire!!)=ground, right=vcc

const uint32_t irPin = PA4; 
const uint32_t LED = PB12;

const uint32_t irProtocol = RC6;
const uint32_t irAddress = 0x46;
const uint32_t irUpCommand = 0x6D;
const uint32_t irDownCommand = 0x70;
const uint32_t irTimeSpacing = 120; // maximum milliseconds between repeating button presses

const int dutyCycle = 255;

uint32_t irTime = 0;
int mode = 0;

void setMode(int m) {
  mode = m;
  motor(mode * dutyCycle);
  digitalWrite(LED, m == 0);
}

int motor(int value) {
  if (value == 0) {
    analogWrite(in1, 0);
    analogWrite(in2, 0);
//    digitalWrite(in1, 0);
//    digitalWrite(in2, 0);
  }
  else if (value > 0) {
    analogWrite(in1, value);
    analogWrite(in2, 0);
//    digitalWrite(in1, 1);
//    digitalWrite(in2, 0);
  }
  else {
    analogWrite(in1, 0);
    analogWrite(in2, -value);
//    digitalWrite(in1, 0);
//    digitalWrite(in2, 1);
  }
}

void setup() {
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(irPin, INPUT);

//    Serial.begin();
  IrReceiver.begin(irPin);

  setMode(0);
}

void loop() {

/*   if (IrReceiver.decode()) {
      Serial.println(millis());
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print "old" raw data
      IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
      IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data
      IrReceiver.resume(); // Enable receiving of the next value
  } */

  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == irProtocol && IrReceiver.decodedIRData.address == irAddress) {
      if (IrReceiver.decodedIRData.command == irDownCommand) {
        setMode(-1);
        irTime = millis();
      }
      else if (IrReceiver.decodedIRData.command == irUpCommand) {
        setMode(1);
        irTime = millis();
      }
    }
    IrReceiver.resume();
  }
  if (mode != 0 && millis() - irTime > irTimeSpacing) {
    setMode(0);
  }
}
