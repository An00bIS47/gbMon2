all: clean isr

isr: pixel-change.c
	gcc isr.c -o isr -lwiringPi

clean:
	rm -f isr
	rm -f *.o