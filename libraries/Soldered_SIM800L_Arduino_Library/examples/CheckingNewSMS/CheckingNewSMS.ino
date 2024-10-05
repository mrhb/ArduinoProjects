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
    Serial.begin(115200); // Start serial communication with PC using 115200 baudrate
    // sim800.setPins(8, 9); // Set any other TX and RX pins
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

    Serial.println("Ready, Send your new SMS");
}

void loop()
{
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
    }

    delay(5000);
}
