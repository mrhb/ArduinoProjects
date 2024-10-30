#include <SoftwareSerial.h>
 
//Create software serial object to communicate with SIM800L
SoftwareSerial gsmSerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
 
void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600 );
  
  //Begin serial communication with Arduino and SIM800L
  gsmSerial.begin(9600);
 
  Serial.println("Initializing..."); 
  delay(1000);
 
  gsmSerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
 
 sendSMS("salam");
  
}


 

int n=1;
char buf[10];
void loop()
{
  sprintf(buf, "Hello!%d", n);
  sendSMS(buf);
  n++;
  delay(10000);
}

bool sendSMS(const char message[])
{
  gsmSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  
  delay(500);//updateSerial();
  gsmSerial.println("AT+CSMP=17,167,0,0"); // برای ارسال SMS انگلیسی مناسب است
  // gsmSerial.println("AT+CSMP=17,167,0,8"); // برای ارسال SMS UNICODE مناسب است
  // gsmSerial.println("AT+CSMP=17,167,0,16"); // _دیالوگ ذخره کردن ظاهر میشود_برای ارسال یک پیام با مدت اعتبار ۱ روز
  delay(500);//updateSerial(); 
  gsmSerial.println("AT+CMGS=\"+989151575793\"\r");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  
  delay(500);//updateSerial(); 
  gsmSerial.println(message); //text content
  gsmSerial.write(0x1A);
  return checkResponse(); 
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    gsmSerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(gsmSerial.available()) 
  {
    Serial.write(gsmSerial.read());//Forward what Software Serial received to Serial Port
  }
}
bool checkResponse()
{
 delay(500);
  // while (Serial.available()) 
  // {
  //   gsmSerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  // }
  // while(gsmSerial.available()) 
  // {
  //   Serial.write(gsmSerial.read());//Forward what Software Serial received to Serial Port
  // }

  String tempData = gsmSerial.readString(); // reads the response
  Serial.println(tempData);
  char *temp = strdup(tempData.c_str());

  if ((strstr(temp,"+CMGS:")) != NULL)
  {
      Serial.println("Send: ");
      return true;
  }
  else
  {
      Serial.write("failed: ");
      return false;
  }
}