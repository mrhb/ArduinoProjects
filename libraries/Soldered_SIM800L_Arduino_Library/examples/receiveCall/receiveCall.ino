/**
 **************************************************
 *
 * @file        makeCall.ino
 * @brief       Example for receiving a phone call.
 *
 *              This example will show you how you can receive a phone call.
 *              Open Serial Monitor at 115200 baud to see what's happening.
 *
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
 * @author     Karlo Leksic for soldered.com
 *
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

    Serial.println("Receiving a call example");
    delay(8000); // This delay is necessary, it helps the device to be ready and connect to a network

    Serial.println("Should be ready by now");

    // Check if sim800 is connected
    if (sim800.isAttached())
    {
        Serial.println("Device is Attached");
        Serial.println("Now you can call me");
    }
    else
    {
        Serial.println("Not Attached");

        while (1)
            ;
    }
}

void loop()
{
    // Check if there is an incoming call
    if (sim800.incomingCall())
    {
        // You can answer the call or hang up

        // Hang up
        // sim800.hangUpCall();

        // Answer
        // sim800.answerCall();
    }
}
