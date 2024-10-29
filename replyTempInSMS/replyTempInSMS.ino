#include <DHT11.h>
DHT11 dht11(4); // Create an instance of the DHT11 class.


#include "SIM800L-SOLDERED.h" // Include soldered library sof SIM800L breakout
SIM800L sim800(3,2); // So connect D8 to the TX, D9 to the RX
int previousSMSIndex = 0;
int currentSMSIndex = 0;
String message = "";


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
#define	IMPLEMENTATION	LIFO


typedef struct smsData {
		uint16_t	templateId;
} SMS;
String  templates[3] = {
	{ "turned on!"},
	{ "turnd off!"},
	{ "overheated!"}
};
cppQueue	q(sizeof(SMS), 10, IMPLEMENTATION);	// Instantiate queue

void setup()
{

  currentTime = millis();
  tempTime=currentTime;

  Serial.begin(9600); // Start serial communication with PC using 115200 baudrate
  sim800.begin(); // Initialize sim800 module
  while (!Serial) // Wait until serial is available
      ;

  Serial.println("Module Is Starting.");
  delay(8000); // This delay is necessary, it helps the device to be ready and connect to a network

  Serial.println("Should be ready by now");

  bool deviceAttached=false;
  while (!deviceAttached)// Check if sim800 is connected
  {
    deviceAttached = sim800.isAttached();
    if (deviceAttached)
    {
        Serial.println("Device is Attached");
        break;
    }
    else
    {
        Serial.println("Not Attached");
    }
  }

  // Delete all sms in memory
  sim800.dellAllSMS();

  // Save the last sms
  currentSMSIndex = sim800.currentMessageIndex; // Reads the last saved sms index
  previousSMSIndex = currentSMSIndex;

  pinMode(LED_BUILTIN, OUTPUT);
  ITimer1.init();
  updateState();
}
void loop()
{
  currentTime = millis();

  String dateTime= sim800.getTime();
	Serial.println(dateTime);

  
  tempreture();
  power();
  checkSMS();
  updateState();

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
      //     Serial.print("Received message is: ");
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
  }
#pragma endregion

#pragma region Power    
  #define sensorPin A0   // select the input pin for the potentiometer
  int  sensorValue = 0;  // variable to store the value coming from the sensor
  float  currentVoltage = 220;  // variable to store the value coming from the sensor
  float  previousVoltage = 220;  // variable to store the value coming from the sensor
  void power()
  {
    sensorValue = analogRead(sensorPin);
    // turn the ledPin on
    // stop the program for <sensorValue> milliseconds:
    Serial.print("sensor:");
    Serial.println(sensorValue);
    Serial.print("Voltage:");
    currentVoltage =sensorValue;
    currentVoltage=(currentVoltage /1023)*220;
    Serial.println(currentVoltage);
    if(currentVoltage>200 && (currentVoltage - previousVoltage)>50){
      previousVoltage=currentVoltage;
      Serial.println("Voltage Connected!");
      SMS sms = {0};
      q.push(&sms);
    
    }
    else if(currentVoltage<180 && (previousVoltage - currentVoltage)> float(50)){
      Serial.println("Voltage DisConnected!");
      previousVoltage=currentVoltage;
      SMS sms = {1};
      q.push(&sms);
      //bool checkDisConnectedSend = sim800.sendSMS("+989151575793", "Voltage DisConnected!");
    }

  }
#pragma endregion


SMS tab[3] = {
	{ 0 },
	{ 1 },
	{ 2 }
};


void updateState()
{
  unsigned int i;

  int count =q.getCount();
  Serial.print("count: ");
  Serial.println(count);
  if(count>0)
  {
    indicator(1);
    for (i = 0 ; i < count ; i++)
    {
      SMS sms;
      q.peek(&sms);
      Serial.print("cnt");Serial.print(i);Serial.print(": ");
      Serial.println(templates[sms.templateId]);
      const char* message = templates[sms.templateId].c_str();
      bool IsSend = sim800.sendSMS("+989151575793",message);
      // delay(2000); bool IsSend=true;
      if(IsSend)
      {
        q.drop();
      }
    }
     indicator(0);
  }
  else{
    indicator(0);
  }
}

void indicator(uint8_t state)
{
  switch (state) {
    case 0:
      ITimer1.detachInterrupt();
      digitalWrite(LED_BUILTIN, false);
      break;
    case 1:
      ITimer1.attachInterruptInterval(50, TimerHandler1);
      break;
    case 2:
      ITimer1.attachInterruptInterval(100, TimerHandler1);
      break;
    case 3:
      ITimer1.attachInterruptInterval(200, TimerHandler1);
      break;
    case 4:
      ITimer1.attachInterruptInterval(300, TimerHandler1);
      break;
  }
}

// smsData messages[20];  //an array of 2 structs of type timerData