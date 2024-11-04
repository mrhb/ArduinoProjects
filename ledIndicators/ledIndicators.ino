const int powerIndicator = 17;
const int tempretureIndicator = 18;
const int smsIndicator = 19;
void setup() {

pinMode(powerIndicator, INPUT_PULLUP);
pinMode(tempretureIndicator, INPUT_PULLUP);
pinMode(smsIndicator, INPUT_PULLUP);

digitalWrite(powerIndicator,HIGH);
digitalWrite(tempretureIndicator,HIGH);
digitalWrite(smsIndicator,HIGH);
delay(500);
digitalWrite(powerIndicator,LOW);
digitalWrite(tempretureIndicator,LOW);
digitalWrite(smsIndicator,LOW);

// A0 => 14
// A2 => 15
// A3 => 16
// A4 => 17
// A5 => 18
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(powerIndicator,HIGH);
delay(100);
digitalWrite(tempretureIndicator,HIGH);
delay(100);
digitalWrite(smsIndicator,HIGH);
delay(500);
digitalWrite(powerIndicator,LOW);
delay(500);
digitalWrite(tempretureIndicator,LOW);
delay(500);
digitalWrite(smsIndicator,LOW);
delay(1000);
}
