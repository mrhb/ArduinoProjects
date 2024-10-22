#include "SIM800L-SOLDERED.h" // Include soldered library sof SIM800L breakout
#include "JDateLib.h"
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
}


// the loop function runs over and over again until power down or reset
void loop() {
  String dateTime= sim800.getTime();
	Serial.println(dateTime);
	Date now(dateTime.substring(2,6).toInt(), dateTime.substring(7,9).toInt(),dateTime.substring(10,12).toInt());
	jdate = now.JDate();
	Serial.println(jdate);
  delay(1000);
}
