//We always have to include the library
#include "LedControl.h"
///based on example at http://www.devppl.com/forum/post66882.html


#include <SPI.h>
#include <Ethernet.h>
// Import the servo library
#include <Servo.h> 

int startbyte;       // start byte, begin reading input

// LED on Pin 13 for digital on/off demo
int ledPin = 2;


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 
  192,168,1, 177 };

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);
/*
THESE ARE THE PINS FOR PROTO PIC 12 PIN 4 DIGIT COMMON CATHOD
 //cathode WRITE LOW TO THIS SIDE!
 int DIGIT_1 = 12;
 int DIGIT_2 = 9;
 int DIGIT_3 = 8;
 int DIGIT_4 = 6;
 
 //anode WRITE +5V TO THIS SIDE!
 int SEGMENT_A = 11;              
 int SEGMENT_B = 7;
 int SEGMENT_C = 4;
 int SEGMENT_D = 2;
 int SEGMENT_E = 1;
 int SEGMENT_F = 10;
 int SEGMENT_G = 5;
 */

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
//LedControl lc=LedControl(12,11,10,1);
LedControl lc=LedControl(7,6,5,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=500;
int fig1=0;
int fig2=0;
int fig3=0;
int fig4=0;
long prevMillis=0;
int ledCounter=1000;

int minutesToArmaggedon=0;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  prevMillis = millis();
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);     
  digitalWrite(ledPin, LOW); 
}




void loop() { 

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) // Read the first byte
  {
    startbyte = client.read();
    minutesToArmaggedon = startbyte;
    Serial.println(minutesToArmaggedon);
    ledCounter=0;
  }
  else{
    //Serial.println("no clinet");
  }
  
  if(minutesToArmaggedon==0){
    lc.setDigit(0,0,1,false);
    lc.setDigit(0,1,2,false);
    lc.setDigit(0,2,0,false);
    lc.setDigit(0,3,0,false);
  }
  else{
    //set only the last digit
    if(minutesToArmaggedon<10){
    lc.setDigit(0,0, 1,false);
    lc.setDigit(0,1,1,false);
    lc.setDigit(0,2,5,false);
    lc.setDigit(0,3,10-minutesToArmaggedon,false);
    }
    else{
      //set the last 2 digits
      int lastDigit = minutesToArmaggedon%10;
      //asumming the value is less than 100 (which it will be) 
      int secondToLastDigit =( minutesToArmaggedon-lastDigit)/10;
    lc.setDigit(0,0, 1,false);
    lc.setDigit(0,1,1,false);
    lc.setDigit(0,2,secondToLastDigit,false);
    lc.setDigit(0,3,lastDigit,false);
    }
  }
  //flash the LED if we have got a new packet    

  if(ledCounter<100){
    digitalWrite(ledPin, HIGH);    
  }
  else{
    digitalWrite(ledPin, LOW) ;
  }
  ledCounter++;
}

char nthdigit(int x, int n)
{
    while (n--) {
        x /= 10;
    }
    return (x % 10) + '0';
}
/*
Python code
 
 #PYTHON SCRIPT FOR GETTING CURRENT STATUS OF DOOMSDAY CLOCK
 #suggest use of udpSendReceive for ethernet arduino library
 #this display https://www.sparkfun.com/products/9482? with this instructable http://www.instructables.com/id/4-Digit-7-Segment-LED-Display-Arduino/
 import socket
 
 import urllib2
 from bs4 import BeautifulSoup
 import time
 loadLive = 1
 staticLooper = 1
 HOST = "192.168.1.177"
 PORT = 80
 #keep doing this for ever. 
 s = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
 s.connect((HOST, PORT))
 
 while staticLooper > 0:
 	if loadLive == 1:
 		url = "http://www.thebulletin.org"
 		#read the page
 		page = urllib2.urlopen(url)
 		#make the soup
 		soup = BeautifulSoup(page.read())
 	else:
 		f = open('page.html', 'r')
 		soup = BeautifulSoup(f)
 	#the title of the page conveniently contains the countdown time	
 	str = soup.title.string
 	#split the sentence
 	exploded = str.split()
 	timeTillAnnihilation = ""
 	for i in range(len(exploded)):
 		#if the current word is minutes then the previous word will be the value eg 5 minutes to go before annihilation
 		if exploded[i] == "Minutes" and i>0:
 			timeTillAnnihilation = exploded[i-1]
 			print timeTillAnnihilation
 	#WRITE TO FILE
 	f = open('log.txt', 'w')
 	f.write((timeTillAnnihilation))
 	f.close()
 	timeTillAnnihilationINT = int(timeTillAnnihilation)
 	#WAIT FOR A SEC THEN DO IT ALL AGAIN.
 	s.send(chr(timeTillAnnihilationINT))
 	time.sleep(1)
 
 */



