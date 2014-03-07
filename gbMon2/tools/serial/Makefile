all: clean serial serialRead

serial: serial.c
	gcc serial.c -o serial -lwiringPi -lpthread

serialRead: serialRead.c
	gcc serialRead.c -o serialRead -lwiringPi


clean:
	rm -f serialRead
	rm -f serial
	rm -f *.o