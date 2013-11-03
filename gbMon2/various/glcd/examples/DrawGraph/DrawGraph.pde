#include <glcd.h>
#include "fonts/fixednums15x31.h"

long sec = 0;
int min = 0;
long starttime;

	
void setup() {

	GLCD.Init(NON_INVERTED);
	GLCD.ClearScreen(); 
}


void loop() {

	gText ClockArea;
	ClockArea.DefineArea(20, 0, 6, 1, fixednums15x31);
	ClockArea.CursorToXY(2,0);
	
	sec = (millis() - starttime) / 1000;
	if (sec > 59) {
		min++;
		sec = 0;
		starttime = millis();
	}
	if (min < 10) {
		ClockArea.print(0);}
	ClockArea.print(min);
	ClockArea.print(":");
	if (sec < 10) {
		ClockArea.print(0);}
	ClockArea.print(sec);
	GLCD.BarGraph(10, 40, 108, 8, 59, sec);
}