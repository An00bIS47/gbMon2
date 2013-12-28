
all: clean udpServer udpClient

udpServer: udpServer.c
	gcc -Wall -o server udpServer.c

udpClient: udpClient.c
	gcc -Wall -o client udpClient.c

clean:
	rm -f udpClient
	rm -f udpServer
	rm -f *.o