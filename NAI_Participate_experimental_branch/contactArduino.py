#PYTHON SCRIPTsends val to arduino from environment var
#draws on example from http://www.daniweb.com/web-development/php/threads/311156/pass-data-from-php-to-python-and-run-python-script all credits for this to authros
import socket, sys
TITLE =""
MESSAGE=""
count=0
for arg in sys.argv:
	if arg == "-t":
		TITLE = sys.argv[count+1] # News header
	elif arg == "-m":
		MESSAGE = sys.argv[count+1] # news body
	count+=1

HOST = "10.67.33.237"
PORT = 80
#keep doing this for ever. 
print TITLE
print MESSAGE
s = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
s.connect((HOST, PORT))
timeTillAnnihilationINT = 10
#s.send(chr(timeTillAnnihilationINT))

for char in TITLE:
	s.send((char))
s.send('#')
