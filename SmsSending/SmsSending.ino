#include <SoftwareSerial.h>
 
//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
 
void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600 );
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);
 
  Serial.println("Initializing..."); 
  delay(1000);
 
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
 

 sendSMSWithoutFeedback("salam");
 delay(5000);
 sendSMS("by");
  
}

void sendSMS(const char message[])
{
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial(); 
  mySerial.println("AT+CSMP=17,167,0,0"); // برای ارسال SMS انگلیسی مناسب است
  // mySerial.println("AT+CSMP=17,167,0,8"); // برای ارسال SMS UNICODE مناسب است
  // mySerial.println("AT+CSMP=17,167,0,16"); // _دیالوگ ذخره کردن ظاهر میشود_برای ارسال یک پیام با مدت اعتبار ۱ روز
  updateSerial(); 
  mySerial.println("AT+CMGS=\"+989151575793\"\r");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  
  updateSerial(); 
  mySerial.println(message); //text content
  mySerial.write(0x1A);
  updateSerial(); 
}
 

void sendSMSWithoutFeedback(const char message[])
{
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  ReadSerial();
  //updateSerial(); 
  mySerial.println("AT+CSMP=17,167,0,0"); // برای ارسال SMS انگلیسی مناسب است
  // mySerial.println("AT+CSMP=17,167,0,8"); // برای ارسال SMS UNICODE مناسب است
  // mySerial.println("AT+CSMP=17,167,0,16"); // _دیالوگ ذخره کردن ظاهر میشود_برای ارسال یک پیام با مدت اعتبار ۱ روز
  //updateSerial(); 
  ReadSerial();
  mySerial.println("AT+CMGS=\"+989151575793\"\r");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  
  //updateSerial(); 
  ReadSerial();
  mySerial.println(message); //text content
  mySerial.write(0x1A);
  //updateSerial(); 
  ReadSerial();
}
 
 
int n=1;
char buf[10];
void loop()
{
   sendSMSWithoutFeedback("salam");
 delay(5000);
  sprintf(buf, "Hello!%d", n);
  n++;
 sendSMS(buf);
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}


void ReadSerial()
{
  delay(500);
  //  while(mySerial.available()) 
  // {
  //   mySerial.read();//Forward what Software Serial received to Serial Port
  // }
}