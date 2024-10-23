#include <TimeLib.h>
// Offset hours from gps time (UTC)
const unsigned long offset = 3.5;   // Tehran Time Zone

void setup(){
  Serial.begin(9600);
  while (!Serial) // Wait until serial is available
      ;
  Serial.println("Serial Configed");

  int Year;
  byte Month, Day, Hour, Minute, Second;
  Year=2024;Month=2;Day=23;
  Hour=3;Minute=30;Second=59;
  // set the Time to the latest GPS reading
  setTime(Hour, Minute, Second, Day, Month, Year);
  adjustTime(3.5 * SECS_PER_HOUR);
}

void  loop(){
  Serial.println(digitalClockString());
  delay(1000);
}

String digitalClockString(){
  char buffer[]="2014/10/23 11:7:23 ";
  sprintf(buffer, "%4d/%02d/%02d %02d:%02d:%02d ", year(),month(),day(),hour(),minute(),second());
  return String(buffer);
}