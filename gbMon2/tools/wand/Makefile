all: clean wand pixel-change

wand: wand.c
	cc -o wand `pkg-config --cflags --libs MagickWand` wand.c

pixel-change: pixel-change.c
	cc -o pixel-change `pkg-config --cflags --libs MagickWand` pixel-change.c

clean:
	rm -f wand
	rm -f pixel-change
	rm -f *.o