#PYTHON SCRIPT FOR GETTING CURRENT STATUS OF DOOMSDAY CLOCK
#suggest use of udpSendReceive for ethernet arduino library
#this display https://www.sparkfun.com/products/9482? with this instructable http://www.instructables.com/id/4-Digit-7-Segment-LED-Display-Arduino/
import socket
import random
import urllib2
from bs4 import BeautifulSoup
import time
loadLive = 0
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
	#s.send(chr(random.randint(1, 60)))
	time.sleep(1)
	
	
#arduino code
#///based on example at http://www.devppl.com/forum/post66882.html

#include <SPI.h>
#include <Ethernet.h>
#// Import the servo library
#include <Servo.h> 


#// LED on Pin 13 for digital on/off demo
#int ledPin = 13;
#int pinState = LOW;

#// Enter a MAC address and IP address for your controller below.
#// The IP address will be dependent on your local network:
#byte mac[] = { 
 # 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#byte ip[] = { 
 # 192,168,1, 177 };

#// Initialize the Ethernet server library
#// with the IP address and port you want to use 
#// (port 80 is default for HTTP):
#EthernetServer server(80);

#void setup()
#{
#  // start the Ethernet connection and the server:
 # Ethernet.begin(mac, ip);
#  server.begin();
#  Serial.begin(9600);
#}
#
#void loop()
#{
  #// listen for incoming clients
  #EthernetClient client = server.available();
  #if (client) // Read the first byte
  #{
    #startbyte = client.read();
    #// If it's really the startbyte (255) ...
   # Serial.println(startbyte);
  #}
##}