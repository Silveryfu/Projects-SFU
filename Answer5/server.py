#import socket module
from socket import *
PORT=80
MAXMSGLEN=1024
serverSocket = socket(AF_INET, SOCK_STREAM)
#Prepare a sever socket
ipaddress=gethostbyname(gethostname())
serverSocket.bind((gethostbyname(gethostname()),PORT))
serverSocket.listen(5)
while True:
#Establish the connection
	print ipaddress+' Ready to serve...'
	connectionSocket, addr = serverSocket.accept()
	try:
		message = connectionSocket.recv(MAXMSGLEN)
		filename = message.split()[1]
		f = open("./serverRoot"+filename)
		outputdata = f.readlines();
		f.close()
		#Send one HTTP header line into socket
		connectionSocket.send("HTTP/1.1 200 OK \nConnection close\nContent-Type: text/html\n\n") 
		#Send the content of the requested file to the client
		for i in range(0, len(outputdata)):
			connectionSocket.send(outputdata[i])
	except IOError:
		#Send response message for file not found
		connectionSocket.send("HTTP/1.1 404 Not Found \nConnection close\n\n") 
		#Close client socket
	if not connectionSocket is None:
		connectionSocket.close()
