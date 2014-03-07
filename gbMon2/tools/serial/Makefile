all: clean serial

serial: serial.c
	gcc serial.c -o serial -lwiringPi -lpthread

clean:
	rm -f serial
	rm -f *.o