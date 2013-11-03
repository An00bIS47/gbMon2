
all: clean screenTest sendRC

sendRC: sendRC.c
	gcc -c sendRC.c -o sendRC.o -lwiringPi -g
	g++ sendRC.o ../rcswitchwrapper.o ../rcswitch.o -o sendRC -lwiringPi -g

zlogTest: zlogTest.c
	gcc -g -c -o zlogTest.o zlogTest.c -I/usr/local/include
	gcc -o zlogTest zlogTest.o -L/usr/local/lib -lzlog -lpthread

screenTest: screenTest.c
	gcc -g screenTest.c ../lcd.o ../fonts/fnt_dejavu_9_b.o ../fonts/fnt_dejavu_9.o ../fonts/fnt_silkscreen_8.o ../fonts/fnt_dejavu.o ../fonts/fnt_spaceLex_8.o -o screenTest -lwiringPi

simulator: simulator.c
	gcc simulator.c -o simulator -lncurses

clean:
	rm -f sendRC
	rm -f screenTest
	rm -f simulator
	rm -f *.o