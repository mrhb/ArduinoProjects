#include <TimeLib.h>
#include "JDateLib.h"
// Offset hours from gps time (UTC)
const unsigned long offset = 3.5;   // Tehran Time Zone

void setup(){
  Serial.begin(9600);
  while (!Serial) // Wait until serial is available
      ;
  Serial.println("Serial Configed");

  int Year;
  byte Month, Day, Hour, Minute, Second;
  Year=2024;Month=10;Day=23;
  Hour=11;Minute=11;Second=30;
  // set the Time to the latest GPS reading
  setTime(Hour, Minute, Second, Day, Month, Year);
  adjustTime(4 * SECS_PER_HOUR);
}

void  loop(){
  Serial.println(digitalClockString());
  delay(1000);
}

String digitalClockString(){
  Date now(year(),month(),day());
  String jdate = now.JDate();
  char buffer[]="2014/10/23 11:7:23 ";
  sprintf(buffer, "%s %02d:%02d:%02d ",jdate.c_str(),hour(),minute(),second());
  return String(buffer);
}