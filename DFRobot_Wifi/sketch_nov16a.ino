/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 9.

 If the IP address of your WiFi is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * CC3200 WiFi LaunchPad or CC3100 WiFi BoosterPack
   with TM4C or MSP430 LaunchPad

 created 25 Nov 2012
 by Tom Igoe
 modified 6 July 2014
 by Noah Luskey
 */

#include <SPI.h>
#include <WiFi.h>

#include "LCD_SharpBoosterPack_SPI.h"

#define PIN1 27
#define PIN2 28
#define PIN3 29
#define PIN4 30

LCD_SharpBoosterPack_SPI myScreen;
uint8_t k = 0;

// your network name also called SSID
char ssid[] = "energia";
// your network password
char password[] = "energia!";
// your network key Index number (needed only for WEP)
int keyIndex = 0;
// your network name you want
char wifi_name[] = "Energia!";
// your network password you want
char wifi_password[] = "launchpad";
WiFiServer server(80);

String css = "style=\" \
//      background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #ededed), color-stop(1, #dfdfdf) ); \
        background-color:#ededed;\
//  -webkit-border-top-left-radius:0px;\
//  -moz-border-radius-topleft:0px;\
//  border-top-left-radius:0px;\
//  -webkit-border-top-right-radius:0px;\
//  -moz-border-radius-topright:0px;\
//  border-top-right-radius:0px;\
//  -webkit-border-bottom-right-radius:0px;\
//  -moz-border-radius-bottomright:0px;\
//  border-bottom-right-radius:0px;\
//  -webkit-border-bottom-left-radius:0px;\
//  -moz-border-radius-bottomleft:0px;\
//  border-bottom-left-radius:0px;\
//  text-indent:0;\
  border:1px solid #dcdcdc;\
  display:inline-block;\
  color:#777777;\
  font-family:Arial Black;\
  font-size:24px;\
  font-weight:bold;\
  font-style:normal;\
  height:100px;\
  line-height:100px;\
  width:200px;\
  text-decoration:none;\
  text-align:center;\" ";


void setup() {
  Serial.begin(115200);      // initialize serial communication
  pinMode(RED_LED, OUTPUT);      // set the RED LED pin mode
  pinMode(GREEN_LED, OUTPUT);
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);
  
//  #if defined(__MSP430__)
//    SPI.begin();
//    SPI.setClockDivider(SPI_CLOCK_DIV2);
//    SPI.setBitOrder(MSBFIRST);
//    SPI.setDataMode(SPI_MODE0);
//  #elif defined(__LM4F120H5QR__)
//    SPI.Select(2);
//    SPI.begin();
//    SPI.setClockDivider(SPI_CLOCK_DIV128); // for LM4F120H5QR DIV2 = 4 MHz !
//  #elif defined(__CC3200R1M1RGC__)
//    SPI.begin();
//    SPI.setClockDivider(SPI_CLOCK_DIV2); // for __CC3200R1M1RGC__ DIV2 = 4 MHz !
//    SPI.setBitOrder(MSBFIRST);
//    SPI.setDataMode(SPI_MODE0);
//  #endif
    
    //myScreen.begin();
    
//    myScreen.setFont(1);
//    myScreen.text(10, 10, "Hello!");
//    myScreen.flush();  
//    
//    delay(1000);
//    myScreen.clear();
  
  
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(wifi_name); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.beginNetwork(wifi_name, wifi_password);

  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");

  delay(10);
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
//  myScreen.setFont(0);
//  myScreen.text(10, 10, "IP Address   ");
//  myScreen.text(10, 20, "obtained     ");
//  myScreen.flush();
//  delay(10);
  // you're connected now, so print out the status  
  printWifiStatus();
//
//  Serial.println("Starting webserver on port 80");
//  myScreen.text(10, 40, "Start server ");
//  myScreen.text(10, 50, "on port 80   ");
//  myScreen.flush();
//  delay(10);
  server.begin();                           // start the web server on port 80
//  Serial.println("Webserver started!");
//  myScreen.text(10, 10, "Webserver    ");
//  myScreen.text(10, 20, "started!     ");
//  myScreen.flush();
//  myScreen.text(10, 40, "You are      ");
//  myScreen.text(10, 50, "connected to:");
//  myScreen.text(10, 70, WiFi.SSID());
//  //IPAddress ip = WiFi.localIP();
//  //myScreen.text(10, 80, String(ip));
//  myScreen.text(10, 80, "192.168.1.1  ");
//  myScreen.flush();
}

void loop() {
  int i = 0;
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    //myScreen.clearBuffer();
    Serial.println("new client");           // print a message out the serial port
    char buffer[150] = {0};                 // make a buffer to hold incoming data
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (strlen(buffer) == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<html><head><title>Energia CC3100 WiFi Web Server</title></head><body align=center>");
            client.println("<h1 align=center><font color=\"red\">Cherokey LaunchPad Controller</font></h1>");
            client.println("<div align=center>");
            client.println("Motor <br><br><button ");
            client.print("style=\"background-color:#ededed;border:1px solid #dcdcdc;display:inline-block;color:#777777;font-family:Arial Black;font-size:24px;\
                    font-weight:bold;font-style:normal;height:100px;line-height:100px;width:200px;\" ");
            client.print("onclick=\"location.href='/F'\">Forward</button><br>");
            client.println("<button ");
            client.print("style=\"background-color:#ededed;border:1px solid #dcdcdc;display:inline-block;color:#777777;font-family:Arial Black;font-size:24px;\
                    font-weight:bold;font-style:normal;height:100px;line-height:100px;width:200px;\" ");
            client.print(" onclick=\"location.href='/L'\">LEFT</button>");
            client.print(" <button ");
            client.print("style=\"background-color:#ededed;border:1px solid #dcdcdc;display:inline-block;color:#777777;font-family:Arial Black;font-size:24px;\
                    font-weight:bold;font-style:normal;height:100px;line-height:100px;width:200px;\" ");
            client.print("onclick=\"location.href='/S'\">STOP</button>");
            client.print(" <button ");
            client.print("style=\"background-color:#ededed;border:1px solid #dcdcdc;display:inline-block;color:#777777;font-family:Arial Black;font-size:24px;\
                    font-weight:bold;font-style:normal;height:100px;line-height:100px;width:200px;\" ");
            client.print("onclick=\"location.href='/R'\">RIGHT</button><br>");
            client.println(" <button ");
            client.print("style=\"background-color:#ededed;border:1px solid #dcdcdc;display:inline-block;color:#777777;font-family:Arial Black;font-size:24px;\
                    font-weight:bold;font-style:normal;height:100px;line-height:100px;width:200px;\" ");
            client.print("onclick=\"location.href='/B'\">Back</button><br><br>");
            client.println("RED LED <button ");
            client.print("style=\"background-color:#ededed;border:1px solid #dcdcdc;display:inline-block;color:#777777;font-family:Arial Black;font-size:24px;\
                    font-weight:bold;font-style:normal;height:100px;line-height:100px;width:200px;\" ");
            client.print("onclick=\"location.href='/REDHIGH'\">HIGH</button>");
            client.print(" <button ");
            client.print("style=\"background-color:#ededed;border:1px solid #dcdcdc;display:inline-block;color:#777777;font-family:Arial Black;font-size:24px;\
                    font-weight:bold;font-style:normal;height:100px;line-height:100px;width:200px;\" ");
            client.print("onclick=\"location.href='/REDLOW'\">LOW</button><br>");
            client.println("GREEN LED <button ");
            client.print("style=\"background-color:#ededed;border:1px solid #dcdcdc;display:inline-block;color:#777777;font-family:Arial Black;font-size:24px;\
                    font-weight:bold;font-style:normal;height:100px;line-height:100px;width:200px;\" ");
            client.print("onclick=\"location.href='/GREENHIGH'\">HIGH</button>");
            client.print(" <button ");
            client.print("style=\"background-color:#ededed;border:1px solid #dcdcdc;display:inline-block;color:#777777;font-family:Arial Black;font-size:24px;\
                    font-weight:bold;font-style:normal;height:100px;line-height:100px;width:200px;\" ");
            client.print("onclick=\"location.href='/GREENLOW'\">LOW</button><br>");
            client.println("</div>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear the buffer:
            memset(buffer, 0, 150);
            i = 0;
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          buffer[i++] = c;      // add it to the end of the currentLine
        }
        // Check to see if the client request was "GET /F":
        if (endsWith(buffer, "GET /R")) {
          digitalWrite(PIN2, HIGH);                // Set left wheels to forward
          digitalWrite(PIN3, HIGH);                // Set right wheels to forward
          digitalWrite(PIN1, HIGH);               // left wheels HIGH
          digitalWrite(PIN4, HIGH);               // right wheels HIGH
        }
        // Check to see if the client request was "GET /B":
        if (endsWith(buffer, "GET /S")) {
          digitalWrite(PIN2, LOW);                // Set left wheels to reverse
          digitalWrite(PIN3, LOW);                // Set right wheels to reverse
          digitalWrite(PIN1, HIGH);               // left wheels HIGH
          digitalWrite(PIN4, HIGH);               // right wheels HIGH
        }
        // Check to see if the client request was "GET /S":
        if (endsWith(buffer, "GET /L")) {
          digitalWrite(PIN2, HIGH);                // Set left wheels to reverse
          digitalWrite(PIN3, HIGH);                // Set right wheels to reverse
          digitalWrite(PIN1, LOW);               // left wheels LOW
          digitalWrite(PIN4, LOW);               // right wheels LOW
        }
        // Check to see if the client request was "GET /R":
        if (endsWith(buffer, "GET /B")) {
          digitalWrite(PIN2, HIGH);                // Set left wheels to forward
          digitalWrite(PIN3, HIGH);                // Set right wheels to forward
          digitalWrite(PIN1, HIGH);               // left wheels HIGH
          digitalWrite(PIN4, LOW);               // right wheels LOW
        }
        // Check to see if the client request was "GET /B":
        if (endsWith(buffer, "GET /F")) {
          digitalWrite(PIN2, HIGH);                // Set left wheels to forward
          digitalWrite(PIN3, HIGH);                // Set right wheels to forward
          digitalWrite(PIN1, LOW);               // left wheels LOW
          digitalWrite(PIN4, HIGH);               // right wheels HIGH
        }
        
        // Check to see if the client request was "GET /REDHIGH" or "GET /REDLOW":
        if (endsWith(buffer, "GET /REDHIGH")) {
          digitalWrite(RED_LED, HIGH);               // GET /REDHIGH turns the LED on
        }
        if (endsWith(buffer, "GET /REDLOW")) {
          digitalWrite(RED_LED, LOW);                // GET /REDLOW turns the LED off
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (endsWith(buffer, "GET /GREENHIGH")) {
          digitalWrite(GREEN_LED, HIGH);               // GET /GREENHIGH turns the LED on
        }
        if (endsWith(buffer, "GET /GREENLOW")) {
          digitalWrite(GREEN_LED, LOW);                // GET /GREENLOW turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

//
//a way to check if one array ends with another array
//
boolean endsWith(char* inString, char* compString) {
  int compLength = strlen(compString);
  int strLength = strlen(inString);
  
  //compare the last "compLength" values of the inString
  int i;
  for (i = 0; i < compLength; i++) {
    char a = inString[(strLength - 1) - i];
    char b = compString[(compLength - 1) - i];
    if (a != b) {
      return false;
    }
  }
  return true;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

