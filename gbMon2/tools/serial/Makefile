all: clean serial

serial: serial.c
	gcc serial.c -o serial -lwiringPi

clean:
	rm -f serial
	rm -f *.o