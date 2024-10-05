/**
 **************************************************
 *
 * @file        makeCall.ino
 * @brief       Example for making a phone call.
 *
 *              This example will show you how you can make a phone call.
 *              Just enter your number in the code below.
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

    Serial.println("Making a call example");
    delay(8000); // This delay is necessary, it helps the device to be ready and connect to a network

    Serial.println("Should be ready by now");

    // Check if sim800 is connected
    if (sim800.isAttached())
    {
        Serial.println("Device is Attached");
    }
    else
    {
        Serial.println("Not Attached");

        while (1)
            ;
    }

    // Make a call - here you enter the number you want to call
    Serial.println("Making a call");

    // Function makeCall returns 1 if the call is made successfully
    if (sim800.makeCall("+385996603414"))
    {
        // Calling for 15 seconds
        Serial.print("Calling");
        for (int i = 0; i < 15; i++)
        {
            // Print a dot every 1 second on serial while waiting / calling
            Serial.print(".");
            delay(1000);
        }
        Serial.println();
    }
    else
    {
        Serial.println("Something went wrong");
    }

    // Hang up call
    sim800.hangUpCall();
    Serial.println("Hang up call");
}

void loop()
{
    // Nothing. We want to make only one call
}
