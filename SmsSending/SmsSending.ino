#include <SoftwareSerial.h>
 #define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
#define USE_TIMER_1     true 
#include "TimerInterrupt.h"

void TimerHandler1(void)
{
  static bool toggle1 = false;

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle1);
  toggle1 = !toggle1;
}


//Create software serial object to communicate with SIM800L
SoftwareSerial gsmSerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
 
void setup()
{
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(5, OUTPUT);digitalWrite(5, true);
  // ITimer1.init();

  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600 );
  
  //Begin serial communication with Arduino and SIM800L
  gsmSerial.begin(9600);
 
  Serial.println("Initializing..."); 
  indicator(3);
  delay(1000);
 
  wiatuntilConnectToGSM();
 
 sendSMS("salam");
  
}


void wiatuntilConnectToGSM(){
  // digitalWrite(5, false);
  // delay(200);
  // digitalWrite(5, true);
  // delay(10000);
   bool deviceAttached=false;
  

  while (!deviceAttached)// Check if sim800 is connected
  {
    gsmSerial.println("AT"); //Once the handshake test is successful, it will back to OK

    String tempData = gsmSerial.readString(); // reads the response
    char *temp = strdup(tempData.c_str());

    if ((strstr(temp,"OK")) != NULL)
    {
        Serial.println("Device is Attached");





        //enable debugging
        gsmSerial.println("AT+CMEE=1");
        delay(200);
        while(gsmSerial.available()) 
        {
          Serial.write(gsmSerial.read());//Forward what Software Serial received to Serial Port
        }
        




        break;
    }
    else
    {
        Serial.println("Not Attached");
        delay(100);
    }
   
  }
} 

int n=1;
char buf[10];
void loop()
{
  sprintf(buf, "Hello!%d", n);
  indicator(0);
  bool IsSend = sendSMS(buf);
  // delay(2000); bool IsSend=true;
  if(IsSend)
  {
    Serial.println("sms is send: ");
    Serial.write(buf);
    indicator(4); //blink fast
    n++;
    delay(10000);
  }
  else{
    Serial.println("sending sms failed: ");
    Serial.write(buf);
    indicator(1); //blink fast
    delay(500);
    indicator(0);



   // wiatuntilConnectToGSM();
    gsmSerial.println("AT"); //Once the handshake test is successful, it will back to OK
    updateSerial();

  }
 
}

bool sendSMS(const char message[])
{
  Serial.println("Is sending Sms");
  Serial.write(message);
  gsmSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  
  delay(500);//updateSerial();
  gsmSerial.println("AT+CSMP=17,167,0,0"); // برای ارسال SMS انگلیسی مناسب است
  // gsmSerial.println("AT+CSMP=17,167,0,8"); // برای ارسال SMS UNICODE مناسب است
  // gsmSerial.println("AT+CSMP=17,167,0,16"); // _دیالوگ ذخره کردن ظاهر میشود_برای ارسال یک پیام با مدت اعتبار ۱ روز
  delay(500);//updateSerial(); 
  gsmSerial.println("AT+CMGS=\"+989151575793\"\r");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  
  delay(500);//updateSerial(); 
  gsmSerial.println(message); //text content
  delay(100);
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
      return true;
  }
  else
  {
      return false;
  }
}


void indicator(uint8_t state)
{
  // switch (state) {
  //   case 0:
  //     ITimer1.detachInterrupt();
  //     digitalWrite(LED_BUILTIN, false);
  //     break;
  //   case 1:
  //     ITimer1.attachInterruptInterval(50, TimerHandler1);
  //     break;
  //   case 2:
  //     ITimer1.attachInterruptInterval(100, TimerHandler1);
  //     break;
  //   case 3:
  //     ITimer1.attachInterruptInterval(200, TimerHandler1);
  //     break;
  //   case 4:
  //     ITimer1.attachInterruptInterval(300, TimerHandler1);
  //     break;
  // }
}