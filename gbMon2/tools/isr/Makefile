all: clean isr

isr: isr.cf
	gcc isr.c -o isr -lwiringPi

clean:
	rm -f isr
	rm -f *.o