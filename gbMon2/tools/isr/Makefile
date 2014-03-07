all: clean isr

isr: isr.c
	gcc isr.c -o isr -lwiringPi

clean:
	rm -f isr
	rm -f *.o