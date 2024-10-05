/**
 **************************************************
 *
 * @file        CheckingNewSMS.ino
 * @brief       This is a bare bone library for communicating with SIM800
 *    It's barebone in that - it only provides basic functionalities while still
 *    maintaining strong performance and being memory friendly.
 *    It currently supports GSM(sending and reading SMS),
 *    GPRS connectivity(sending and receiving TCP) with Time and Location
 *
 *    This library is written by Ayo Ayibiowu.
 *    charlesayibiowu@hotmail.com
 *    Designed to work with the GSM Sim800l module
 *
 *    To Enable Debugging - Go to <BareBoneSim800.h file and change the
 *    #define DEBUG 0 to #define DEBUG 1
 *
 *    PINOUT:
 *        _____________________________
 *       |  Dasduino  >>>   SIM800L   |
 *        -----------------------------
 *           GND      >>>   GND
 *        RX  8       >>>   TX
 *        TX  9       >>>   RX
 *           VCC      >>>   VCC
 *    Also, you have to connect the PWRKEY pin to the VCC or some digital pin if you want to control the module.
 *    You can change RX and TX pins callilng setPins() function before begin(), these are the default ones.
 *
 *
 * @authors     Created on: Oct 24, 2017
 *              Author: Ayo Ayibiowu
 *              Email: charlesayibiowu@hotmail.com
 *              Version: v1.0
 * 
 *   Modified by: soldered.com, 21 March 2023
 *   See more at https://www.solde.red/333071
 ***************************************************/




/**
 * DHT11 Sensor Reader
 * This sketch reads temperature and humidity data from the DHT11 sensor and prints the values to the serial port.
 * It also handles potential error states that might occur during reading.
 *
 * Author: Dhruba Saha
 * Version: 2.1.0
 * License: MIT
 */

// Include the DHT11 library for interfacing with the sensor.
#include <DHT11.h>

// Create an instance of the DHT11 class.
// - For Arduino: Connect the sensor to Digital I/O Pin 2.
// - For ESP32: Connect the sensor to pin GPIO2 or P2.
// - For ESP8266: Connect the sensor to GPIO2 or D4.
DHT11 dht11(4);



// Include soldered library sof SIM800L breakout
#include "SIM800L-SOLDERED.h"

// The next line makes that Dasduino's pin 8 becomes RX and you have to connect it to the TX on the breakout,
// 9 pin also ...
SIM800L sim800(8, 9); // So connect D8 to the TX, D9 to the RX
// The same as SIM800L sim800(); because it's default pins
// If you use Dasduino Lite, the pins are in the format "PAx", e.g. PA2, PA3

// SIM800L sim800("your APN");  // Needed for gprs funtionality
// When using constructors without pins, call setPins() with your pins.
// Use setPins() before begin() function

int previousSMSIndex = 0;
int currentSMSIndex = 0;
String message = "";

void setup()
{
    Serial.begin(9600); // Start serial communication with PC using 115200 baudrate
    sim800.setPins(3, 2); // Set any other TX and RX pins
    sim800.begin(); // Initialize sim800 module
    while (!Serial) // Wait until serial is available
        ;

    Serial.println("Testing GSM module For New SMS Checking");
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

    // Delete all sms in memory
    sim800.dellAllSMS();

    // Save the last sms
    currentSMSIndex = sim800.currentMessageIndex; // Reads the last saved sms index
    previousSMSIndex = currentSMSIndex;

    //Serial.println("Ready, Send your new SMS");

    Serial.println("is sending  new SMS");
    // sim800.sendSMS("+989151575793", "salam");
}

void loop()
{
    #pragma region Temperature
      int temperature = 0;
      int humidity = 0;


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
  #pragma endregion


    bool checkSMS = sim800.checkNewSMS(); // Check if tehere is incoming message
    if (checkSMS)
    {
        Serial.println("New SMS receieved");
        currentSMSIndex = sim800.currentMessageIndex; // Reads the last saved sms index

        // Lets read the sms
        message = sim800.readSMS(currentSMSIndex); // Read sms and save it in message variable
        Serial.print("Received message is: ");
        Serial.println(message);
        previousSMSIndex = currentSMSIndex; // Update your sms index
        

        bool checkSend = sim800.sendSMS("+989151575793", "salam");
        if(checkSend){
          Serial.print("message is Sended");
        }
        else
        {
          Serial.print("Sending Failed");

        }
    }

    delay(5000);
}

