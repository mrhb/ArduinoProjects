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





unsigned long currentTime;
unsigned long tempTime;


#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
#define USE_TIMER_1     true 
#include "TimerInterrupt.h"

void TimerHandler1(void)
{
  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}


void setup() {
    // Initialize serial communication to allow debugging and data readout.
    // Using a baud rate of 9600 bps.
    Serial.begin(9600);
    Serial.println("Starting dht11 Project.");
    ITimer1.init();

    pinMode(LED_BUILTIN, OUTPUT);
    indicator(0);
    // Uncomment the line below to set a custom delay between sensor readings (in milliseconds).
     dht11.setDelay(500); // Set this to the desired delay. Default is 500ms.
    //indicator(0);
}

void loop() {
    int temperature = 0;
    int humidity = 0;


    // Attempt to read the temperature and humidity values from the DHT11 sensor.
    indicator(0);
    int result = dht11.readTemperatureHumidity(temperature, humidity);
    //int result=0;
    indicator(1);
    //ITimer1.reattachInterrupt(200);
    //interrupts();

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
    delay(2000);
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