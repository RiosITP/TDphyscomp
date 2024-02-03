#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"

#include <OSCBoards.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <OSCMatch.h>
#include <OSCMessage.h>
#include <OSCTiming.h>
#include <SLIPEncodedSerial.h>

/*
  WiFi UDP Send and Receive String

 This sketch waits for a UDP packet on localPort using the WiFi module.
 When a packet is received an Acknowledge packet is sent to the client on port remotePort

 created 30 December 2012
 by dlf (Metodo2 srl)

 */

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;      // your network SSID (name)
char pass[] = SECRET_PASS;      // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;               // your network key index number (needed only for WEP)
unsigned int localPort = 2345;   // local port to receive / open on
unsigned int sendPort = 9051;   // local port to send to
//school///char outIP[] = "128.122.0.71 "; // insert your own computer's IP Address
char outIP[] = "10.23.11.184";  // insert your own computer's IP Address

WiFiUDP Udp;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  // while (!Serial) {
  //   ;  // wait for serial port to connect. Needed for native USB port only
  //      // this will freeze your code until there is a serial connection (Open serial monitor)
  //      // comment out or remove this while loop if you do not need serial connection / serial monitor
  // }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }
  pinMode(2, INPUT_PULLUP);

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();  // function defined at end of sketch

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

void loop() {
  OSCBundle oscOut;

  int sensor1 = analogRead(A0);  // sensor 1 data
  int sensor2 = analogRead(A1);  // sensor 2 data
  int sensor3 = digitalRead(2);  // sensor 3 data

  oscOut.add("/sensor1").add(sensor1);
  oscOut.add("/sensor2").add(sensor2);
  oscOut.add("/sensor3").add(sensor3);
  //format and sent udp
  Udp.beginPacket(outIP, sendPort);
  oscOut.send(Udp);
  Udp.endPacket();
  oscOut.empty();
  delay(20);  // if you are not using call and response, do not send more information than you need
}



void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
