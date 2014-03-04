all: clean wand

wand: wand.c
	cc -o wand `pkg-config --cflags --libs MagickWand` wand.c

clean:
	rm -f wand
	rm -f *.o