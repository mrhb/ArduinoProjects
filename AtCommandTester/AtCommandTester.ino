#include <SoftwareSerial.h>
SoftwareSerial Sim800Serial(3, 2); // RX, TX
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Sim800Serial.begin(9600);
}
void loop() {
  if (Sim800Serial.available()) {
    Serial.write(Sim800Serial.read());
  }
  if (Serial.available()) {
    Sim800Serial.write(Serial.read());
  }
}



// #include "AsyncSMS.h"
// #include <SoftwareSerial.h>

// #define RX 3
// #define TX 2

// SoftwareSerial SerialGSM(RX, TX);
// AsyncSMS smsHelper(&SerialGSM);
// void setup() {
//   Serial.begin(9600);
//   SerialGSM.begin(9600);
//   smsHelper.init();
//   smsHelper.send("+989361855278", "This is my Arduino driven SMS module working", 45);
// }

// void loop() {
//   smsHelper.process();

//    if (SerialGSM.available()) {
//     Serial.write(SerialGSM.read());
//   }
// }