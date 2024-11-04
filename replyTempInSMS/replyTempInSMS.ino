#include <DHT11.h>
DHT11 dht11(4); // Create an instance of the DHT11 class.


#include <TimeLib.h>
// #include "SIM800L-SOLDERED.h" // Include soldered library sof SIM800L breakout
#include <SoftwareSerial.h>
#include "JDateLib.h"

// Offset hours from gps time (UTC)
const unsigned long offset = 3.5;   // Tehran Time Zone

//SIM800L sim800(3,2); // So connect D8 to the TX, D9 to the RX
SoftwareSerial GsmSerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
int previousSMSIndex = 0;
int currentSMSIndex = 0;


unsigned long currentTime;
unsigned long tempTime;


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





#include <cppQueue.h>
#define	IMPLEMENTATION	FIFO


typedef struct smsData {
		uint16_t	      templateId;
    unsigned long   dateTime;
} SMS;

char templates[4][20] = {
	 "turned on!\r\n",
	 "turned off!\r\n",
	 "overheated!\r\n",
	 "Monitoring Started.\r\n"
};
cppQueue	q(sizeof(SMS), 10, IMPLEMENTATION);	// Instantiate queue

const int powerIndicator = 17;
const int tempretureIndicator = 18;
const int smsIndicator = 19;

void setup()
{

  pinMode(powerIndicator, INPUT_PULLUP);//right yellow 
  pinMode(tempretureIndicator, INPUT_PULLUP);//red
  pinMode(smsIndicator, INPUT_PULLUP);

  digitalWrite(powerIndicator,HIGH);
  digitalWrite(tempretureIndicator,HIGH);
  digitalWrite(smsIndicator,HIGH);

  currentTime = millis();
  tempTime=currentTime;
  GsmSerial.begin(9600);
  Serial.begin(9600); // Start serial communication with PC using 115200 baudrate
  //sim800.begin(); // Initialize sim800 module
  while (!Serial) // Wait until serial is available
      ;

  Serial.println("Module Is Starting.");
  delay(8000); // This delay is necessary, it helps the device to be ready and connect to a network

  Serial.println("Should be ready by now");

  // bool deviceAttached=false;
  // while (!deviceAttached)// Check if sim800 is connected
  // {
  //   deviceAttached = sim800.isAttached();
  //   if (deviceAttached)
  //   {
  //       Serial.println("Device is Attached");
  //       break;
  //   }
  //   else
  //   {
  //       Serial.println("Not Attached");
  //       delay(100);
  //   }
  // }


  wiatuntilConnectToGSM();


  // Delete all sms in memory
  //sim800.dellAllSMS();
  setNetworkTime();
  // Save the last sms
  // currentSMSIndex = sim800.currentMessageIndex; // Reads the last saved sms index
  // previousSMSIndex = currentSMSIndex;

  pinMode(LED_BUILTIN, OUTPUT);
  // ITimer1.init();

  // alert Start
  SMS sms = {};
  sms.templateId=3;
  sms.dateTime=now();
  q.push(&sms);

  digitalWrite(powerIndicator,LOW);
  digitalWrite(tempretureIndicator,LOW);
  digitalWrite(smsIndicator,LOW);
}
void loop()
{
  Serial.println("main loop started!");
  currentTime = millis();
  //String dateTime= sim800.getTime();
	//Serial.println(dateTime);
  
  power();
  tempreture();
  checkSMS();
  Serial.println("main loop completed!1");
  updateState();
  Serial.println("main loop completed!2");
}


#pragma region checkNewSMS
  void checkSMS()
  {
      // bool checkSMS = sim800.checkNewSMS(); // Check if tehere is incoming message
      // if (checkSMS)
      // {
      //     Serial.println("New SMS receieved");
      //     // currentSMSIndex = sim800.currentMessageIndex; // Reads the last saved sms index

      //     // Lets read the sms
      //     message = sim800.readSMS(sim800.currentMessageIndex); // Read sms and save it in message variable
      //     //Serial.println("Received message is: ");
      //     Serial.println(message);
      //     // previousSMSIndex = currentSMSIndex; // Update your sms index
          
      //     char msg[]="";
      //     char mobilenumber[14];
      //     sim800.lastMobileNumber.toCharArray(mobilenumber, 14);
      //     sprintf(msg,"Hi!Temperature: %ddeg and Humidity: %d%",temperature,humidity);
      //     bool checkSend = sim800.sendSMS(mobilenumber, msg);
      //     if(checkSend){
      //       Serial.println("message is Sended");
      //     }
      //     else
      //     {
      //       Serial.println("Sending Failed");

      //     }
      // }
  }
#pragma endregion

#pragma region Temperature
  #define tempPeriod 1000
  int temperature = 0;
  int humidity = 0;
  void tempreture()
  {
    digitalWrite(tempretureIndicator,HIGH);

    Serial.println("read tempreture started!");
    if ( currentTime - tempTime < tempPeriod )  //test whether the period has elapsed
    {
      return;
    }
    else {
      tempTime = currentTime;  //IMPORTANT to save the start time of the current LED state.
    }

      // Attempt to read the temperature and humidity values from the DHT11 sensor.
      int result = dht11.readTemperatureHumidity(temperature, humidity);

      // Check the results of the readings.
      // If the reading is successful, print the temperature and humidity values.
      // If there are errors, print the appropriate error messages.
      if (result == 0) {
          Serial.print("Temperature: ");
          Serial.print(temperature);
          Serial.print(" °C\tHumidity: ");
          Serial.print(humidity);
          Serial.println(" %");
      } else {
          // Print error message based on the error code.
          Serial.println(DHT11::getErrorString(result));
      }
    Serial.println("read tempreture completed!");
    delay(1000);
    digitalWrite(tempretureIndicator,LOW);
  }
#pragma endregion //Temperature

#pragma region Power    
  #define sensorPin A0   // select the input pin for the potentiometer
  int  sensorValue = 0;  // variable to store the value coming from the sensor
  float  currentVoltage = 220;  // variable to store the value coming from the sensor
  float  previousVoltage = 220;  // variable to store the value coming from the sensor
  void power()
  {
     digitalWrite(powerIndicator,HIGH);
     delay(500);
     digitalWrite(powerIndicator,LOW);
     delay(500);
     digitalWrite(powerIndicator,HIGH);
    Serial.println("read power started!");
    sensorValue = analogRead(sensorPin);
    // turn the ledPin on
    // stop the program for <sensorValue> milliseconds:
    //Serial.println("sensor:");
    Serial.println(sensorValue);
    //Serial.println("Voltage:");
    currentVoltage =sensorValue;
    currentVoltage=(currentVoltage /1023)*220;
    Serial.println(currentVoltage);
    if(currentVoltage>200 && (currentVoltage - previousVoltage)>50){
      previousVoltage=currentVoltage;
      Serial.println("Voltage Connected!");
      SMS sms = {};
      sms.templateId=0;
      sms.dateTime=now();
      q.push(&sms);
    
    }
    else if(currentVoltage<180 && (previousVoltage - currentVoltage)> float(50)){
      Serial.println("Voltage DisConnected!");
      previousVoltage=currentVoltage;
      SMS sms = {};
      sms.templateId=1;
      sms.dateTime=now();
      q.push(&sms);
      //bool checkDisConnectedSend = sim800.sendSMS("+989151575793", "Voltage DisConnected!");
    }

    
    digitalWrite(powerIndicator,LOW);
    Serial.println("read power completed!");
  }
#pragma endregion


SMS tab[3] = {
	{ 0 },
	{ 1 },
	{ 2 }
};


void updateState()
{
  digitalWrite(smsIndicator,HIGH);
  Serial.println("updateState started!");
  unsigned int i=0;

  int count =q.getCount();
  //Serial.println("count: ");
  Serial.println(count);
  if(count>0)
  {
    indicator(4); //blink slow
    for (i = 0 ; i < count ; i++)
    {
      Serial.println("updateState complete! 001");

      SMS sms;
      q.peek(&sms);
      //Serial.println("cnt");//Serial.println(i);//Serial.println(": ");
      // Serial.println(templates[sms.templateId]);
      // Serial.println(sms.dateTime);

      char message[200];
      strcpy (message,templates[sms.templateId]);
      strcat (message,digitalClockString(sms.dateTime));
      Serial.println(message);
      // bool IsSend = sim800.sendSMS("+989151575793",message);
      // delay(2000); bool IsSend=true;
      bool IsSend = sendSMS(message);
      if(IsSend)
      {
        q.drop();
        Serial.println("sms is send");
      }
      else{
        Serial.println("sending sms failed!");
        indicator(1); //blink fast
        delay(500);
      }
    }
     indicator(0);
  //  Serial.println("updateState complete! 00");
     
  }
  else{
   indicator(0);
  }
   Serial.println("updateState complete!");
    delay(500);
    digitalWrite(smsIndicator,LOW);
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



void setNetworkTime(){

  int Year;
  byte Month, Day, Hour, Minute, Second;

  // String dateTime= sim800.getTime();
  // Serial.println(dateTime);
  // if(dateTime.length()<2) return;

  // Year=dateTime.substring(2,6).toInt();
  // Month= dateTime.substring(7,9).toInt();
  // Day=dateTime.substring(10,12).toInt();
  // Hour=dateTime.substring(13,15).toInt();
  // Minute=dateTime.substring(16,18).toInt();
  // Second=dateTime.substring(19,21).toInt();

  Year=2024;
  Month= 11;
  Day=4;
  Hour=11;
  Minute=4;
  Second=1;
	
  Date now(Year,Month,Day);

  // set the Time to the latest GPS reading
  setTime(Hour, Minute, Second, Day, Month, Year);
  adjustTime(3.5 * SECS_PER_HOUR);

}


char* digitalClockString(unsigned long time){
  Date now(year(time),month(time),day(time));
  String jdate = now.JDate();
  char* buffer="2014/10/23 11:7:23";
  sprintf(buffer, "%s-%02d:%02d:%02d",jdate.c_str(),hour(time),minute(time),second(time));
  return buffer;
}
// smsData messages[20];  //an array of 2 structs of type timerData





bool sendSMS(const char message[])
{
  Serial.println("Is sending Sms");
  Serial.write(message);
  GsmSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  
  delay(500);//updateSerial();
  GsmSerial.println("AT+CSMP=17,167,0,0"); // برای ارسال SMS انگلیسی مناسب است
  // GsmSerial.println("AT+CSMP=17,167,0,8"); // برای ارسال SMS UNICODE مناسب است
  // GsmSerial.println("AT+CSMP=17,167,0,16"); // _دیالوگ ذخره کردن ظاهر میشود_برای ارسال یک پیام با مدت اعتبار ۱ روز
  delay(500);//updateSerial(); 
  GsmSerial.println("AT+CMGS=\"+989151575793\"\r");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  
  delay(500);//updateSerial(); 
  GsmSerial.println(message); //text content
  delay(100);
  GsmSerial.write(0x1A);
  return checkResponse(); 
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    GsmSerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(GsmSerial.available()) 
  {
    Serial.write(GsmSerial.read());//Forward what Software Serial received to Serial Port
  }
}
bool checkResponse()
{
 delay(500);
  // while (Serial.available()) 
  // {
  //   GsmSerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  // }
  // while(GsmSerial.available()) 
  // {
  //   Serial.write(GsmSerial.read());//Forward what Software Serial received to Serial Port
  // }

  String tempData = GsmSerial.readString(); // reads the response
  Serial.println(tempData);
  delay(20000);
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
void wiatuntilConnectToGSM(){
  // digitalWrite(5, false);
  // delay(200);
  // digitalWrite(5, true);
  // delay(10000);
   bool deviceAttached=false;
  

  while (!deviceAttached)// Check if sim800 is connected
  {
    GsmSerial.println("AT"); //Once the handshake test is successful, it will back to OK

    String tempData = GsmSerial.readString(); // reads the response
    char *temp = strdup(tempData.c_str());

    if ((strstr(temp,"OK")) != NULL)
    {
        Serial.println("Device is Attached");





        //enable debugging
        GsmSerial.println("AT+CMEE=1");
        delay(200);
        while(GsmSerial.available()) 
        {
          Serial.write(GsmSerial.read());//Forward what Software Serial received to Serial Port
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