//zoomkat 12-08-11
//simple client test
//for use with IDE 1.0
//open serial monitor and send an e to test
//for use with W5100 based ethernet shields
//note that the below bug fix may be required
// http://code.google.com/p/arduino/issues/detail?id=605

#include <SPI.h>
#include <Ethernet.h>
#include "LedControl.h"
#include <Servo.h>

String page;
byte mac[] = {
  //  90-a2-da-0d-45-d1
  // 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  0x90, 0xA2, 0xDA, 0x0D, 0x45, 0xD1
};
//uni router ip
//byte ip[] = { 192,168,1,177 };

//uni lan ip
//byte ip[] = { 10,67,33,237};

//mac shared internet ip
byte ip[] = { 192, 168, 2, 2};

//this is not currently used
byte myserver[] = { 82, 165, 205, 178 }; //{ 64,15,119,44 };// fieldventures web page server IP address
EthernetClient client;

int ledPin = 2;

//LED library object
LedControl lc = LedControl(7, 6, 5, 1);

int fig1 = 0;
int fig2 = 0;
int fig3 = 0;
int fig4 = 0;


int minutesToArmaggedon = 0;

//////////////////////

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("starting");
  // Ethernet.begin(mac, ip);

  //auto configure the IP address with DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  // print your local IP address:
  Serial.println(Ethernet.localIP());





  Serial.println("Better client test 12/01/11"); // so I can keep track of what is loaded
  Serial.println("Send an e in serial monitor to test"); // what to do to test

  //setup the display
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 7);
  /* and clear the display */
  lc.clearDisplay(0);

  //
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  delay(200);
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);

}

void loop() {
  //make the get call and subtract the ascii code offset
  int minutesToArmaggedon = (int)sendGET() - 48;
  //
  //  Serial.print("minutesToArmaggedon");
  //  Serial.println(minutesToArmaggedon);
  //  digitalWrite(ledPin, LOW);
  //  delay(100);
  if (minutesToArmaggedon == 0) {
    lc.setDigit(0, 0, 1, false);
    lc.setDigit(0, 1, 2, true);
    lc.setDigit(0, 2, 0, false);
    lc.setDigit(0, 3, 0, false);
  }
  else {
    //set only the last digit
    if (minutesToArmaggedon < 10) {
      lc.setDigit(0, 0, 1, false);
      lc.setDigit(0, 1, 1, true);
      lc.setDigit(0, 2, 5, false);
      lc.setDigit(0, 3, 10 - minutesToArmaggedon, false);
    }
    else {
      //set the last 2 digits
      int lastDigit = minutesToArmaggedon % 10;
      //asumming the value is less than 100 (which it will be)
      int secondToLastDigit = ( minutesToArmaggedon - lastDigit) / 10;
      lc.setDigit(0, 0, 1, false);
      lc.setDigit(0, 1, 1, true);
      lc.setDigit(0, 2, secondToLastDigit, false);
      lc.setDigit(0, 3, lastDigit, false);
    }
  }

  //
  //  digitalWrite(ledPin, HIGH);
  //  delay(40);
}

char nthdigit(int x, int n)
{
  while (n--) {
    x /= 10;
  }
  return (x % 10) + '0';
}

//////////////////////////

char sendGET() //client function to send/receive GET request data.
{
  char new_time = '0';
  if (client.connect("thebulletin.org", 80)) {  //starts client connection, checks for connection
    Serial.println("connected");
    
    client.println("GET /timeline HTTP/1.1"); //download text
    client.println("Host:thebulletin.org");
    client.println("User-Agent: arduino-ethernet");
    client.println(); //end of get request
  }
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  while (client.connected() && !client.available()) 
  {
    delay(1); //waits for data
    digitalWrite(ledPin, HIGH);
  }

  boolean listening = false;
  boolean flipOn = false;
  String myWord = "";
  boolean begin = false;
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer


    page += c;
        if (flipOn) {
        digitalWrite(ledPin, HIGH);
    
        }
        else {
          digitalWrite(ledPin, LOW);
        }
    
        flipOn = !flipOn;
    if (c == '\n' || page.length() > 80) {
      // Serial.print(" a line ");


      if (page.indexOf("minutes to midnight") > -1) {
        //     Serial.print("minutes : " );
        char mins = page.charAt(page.indexOf("minutes to midnight") - 2);
        //    Serial.println(mins );
        new_time = mins;
        page = "";
        break;
      }
      page = "";
    }



  }

  client.stop(); //stop client
  digitalWrite(ledPin, LOW);
  return new_time;
}

