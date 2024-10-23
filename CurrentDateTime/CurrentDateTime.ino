#include <TimeLib.h>
#include "SIM800L-SOLDERED.h" // Include soldered library sof SIM800L breakout
#include "JDateLib.h"

// Offset hours from gps time (UTC)
const unsigned long offset = 3.5;   // Tehran Time Zone
String jdate;            // Jalali Date String
int JY,JM,JD;            // Jalali Year & Month & Day Intiger

SIM800L sim800(3,2); // So connect D8 to the TX, D9 to the RX

void setup()
{  
  Serial.begin(9600); // Start serial communication with PC using 115200 baudrate
  sim800.begin(); // Initialize sim800 module
  while (!Serial) // Wait until serial is available
      ;

  Serial.println("Initializing GSM module");
  delay(8000); // This delay is necessary, it helps the device to be ready and connect to a network

  Serial.println("Should be ready by now");
  bool deviceAttached = sim800.isAttached(); // Check if sim800 is connected
  if (deviceAttached)
  {
      Serial.println("Device is Attached");
  }
  else
  {
      Serial.println("Not Attached");
      while (1)
          ;
  }

  setNetworkTime();
}


// the loop function runs over and over again until power down or reset
void loop() {
  Serial.println(digitalClockString());
  delay(5000);
}

void setNetworkTime(){

  int Year;
  byte Month, Day, Hour, Minute, Second;

  String dateTime= sim800.getTime();
  Serial.println(dateTime);
  if(dateTime.length()<2) return;

  Year=dateTime.substring(2,6).toInt();
  Month= dateTime.substring(7,9).toInt();
  Day=dateTime.substring(10,12).toInt();
  Hour=dateTime.substring(13,15).toInt();
  Minute=dateTime.substring(16,18).toInt();
  Second=dateTime.substring(19,21).toInt();
	
  Date now(Year,Month,Day);

  // set the Time to the latest GPS reading
  setTime(Hour, Minute, Second, Day, Month, Year);
  adjustTime(3.5 * SECS_PER_HOUR);

}


String digitalClockString(){
  Date now(year(),month(),day());
  String jdate = now.JDate();
  char buffer[]="2014/10/23 11:7:23 ";
  sprintf(buffer, "%s %02d:%02d:%02d ",jdate.c_str(),hour(),minute(),second());
  return String(buffer);
}