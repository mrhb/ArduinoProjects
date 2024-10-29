volatile int ledState = LOW;

void setup() {
  Serial.begin(9600 );
  pinMode(2, INPUT_PULLUP);

  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), toggleLED, FALLING);
}

void loop() {
  digitalWrite(13, ledState);
}
volatile  int n=0;
void toggleLED() {
  n++;
  Serial.println(n);
  ledState = !ledState;
}