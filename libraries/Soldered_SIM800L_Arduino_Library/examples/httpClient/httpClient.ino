/**
 **************************************************
 *
 * @file        httpClient.ino
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

SIM800L sim800("internet.ht.hr"); // To declare the library with an APN
// When using constructors without pins, call setPins() with your pins. Default are 8 and 9
// Use setPins() before begin() function!

// Connecting to the Internet and Acting as an HTTP Web Client
// username and password has been set to "" in the Library Code
char resource[] = "postman-echo.com/get";

const int port = 80;

void setup()
{
    Serial.begin(115200); // Start serial communication with PC using 115200 baudrate
    // If you use Dasduino Lite, the pins are in the format "PAx", e.g. PA2, PA3
    sim800.setPins(8, 9); // Set any other TX and RX pins
    sim800.begin();       // Initialize sim800 module
    while (!Serial)       // Wait until serial is available
        ;

    Serial.println("Testing GSM module For GPRS Connectivity");
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

    // Connecting the the GPRS APN Network
    Serial.println(" Connecting to APN");
    bool netConnect = sim800.gprsConnect(); // Check if sim800 is connected to network
    if (netConnect)
        Serial.println("Connected to Network");
    else
        Serial.println("An Error Occured");

    if (netConnect)
    {
        Serial.println("Making HTTP Get Request");
        String result = sim800.sendHTTPData(resource); // Send command
        Serial.println("Received Info: ");
        Serial.println(result);
    }

    sim800.closeHTTP();      // disconnect from server
    sim800.gprsDisconnect(); // Close connection with network
}

void loop()
{
    // zZZzz
}
