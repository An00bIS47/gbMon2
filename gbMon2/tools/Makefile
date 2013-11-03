
all: clean screen-test

zlogTest: zlogTest.c
	gcc -g -c -o zlogTest.o zlogTest.c -I/usr/local/include
	gcc -o zlogTest zlogTest.o -L/usr/local/lib -lzlog -lpthread

screen-test: screen-test.c
	gcc -g screen-test.c lcd.o fnt_dejavu_9_b.o fnt_dejavu_9.o fnt_silkscreen_8.o fnt_dejavu.o fnt_spaceLex_8.o -o screen-test -lwiringPi

simulator: simulator.c
	gcc simulator.c -o simulator -lncurses

clean:
	rm -f screen-test
	rm -f simulator
	rm -f *.o