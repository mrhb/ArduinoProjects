/**
 **************************************************
 *
 * @file        SendSMS.ino
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
 *    To Enable Debugging - Go to BareBoneSim800.h file and change the
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


void setup()
{
    Serial.begin(115200); // Start serial communication with PC using 115200 baudrate
    // sim800.setPins(8, 9); // Set any other TX and RX pins
    sim800.begin(); // Initialize sim800 module
    while (!Serial) // Wait until serial is available
        ;

    Serial.println("Testing GSM module For SMS Sending");
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

    // Testing sending SMS aspect

    // +385 is the country code for Croatia, change it to yours and replace xxxx with your number
    const char *number = "+385xxxxxxxxx";
    char *message = "Hello, This is a text message";

    delay(1000);

    bool messageSent = sim800.sendSMS(number, message); // Send message to number "number" with content
                                                        // stored in message variable
    if (messageSent)
        Serial.println("Message Sent");
    else
        Serial.println("Not Sent, Something happened");
}

void loop()
{
    // zZZzz
}
