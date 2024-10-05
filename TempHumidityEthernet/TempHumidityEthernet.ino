/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 modified 9 Apr 2012
 by David A. Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(172,16,16,94); // Google

IPAddress gateway(172,16,16,16); // آدرس آی پی درگاه
IPAddress subnet(255, 255, 255, 0); // آدرس آی پی ساب نت

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
Serial.println("");
Serial.println("welcome to Ethernet Module");
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }

  Serial.print("IP = ");
  Serial.println(Ethernet.localIP());
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  //Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 5001)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /logo.png");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{

  if (client.connect(server, 5001)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /dist/menu.css");
    while(client.connected()){
      if(client.available()){
        //Read incoming byte from the server and print to the serial monitor
        char c = client.read();
        Serial.print(c);
      }
    }
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  delay(1000);


  // // if there are incoming bytes available 
  // // from the server, read them and print them:
  // if (client.available()) {
  //   char c = client.read();
  //   Serial.print(c);
  // }

  // // if the server's disconnected, stop the client:
  // if (!client.connected()) {
  //   Serial.println();
  //   Serial.println("disconnecting.");
  //   client.stop();

  //   // do nothing forevermore:
  //   for(;;)
  //     ;
  // }
}