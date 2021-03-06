//
//  screenGraph.c
//  gbMon2
//
//  Created by michael on 09.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#include "screenGraph.h"

int curPos;

int rand_lim(int limit) {
	/* return a random number between 0 and limit inclusive.
	 */
	
    int divisor = RAND_MAX/(limit+1);
    int retval;
	
    do {
        retval = rand() / divisor;
    } while (retval > limit);
	
    return retval;
}


void displayBorder(){
	//GLCDD_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
	
	GLCDD_Rect r;
	
	r.x = 6;
	r.w = 128 - 12;
	r.h = -1;
	
	fnt_spaceLex_5=createFont(spaceLex_5);
	
	//GLCDD_Rectangle(0,  9, 128, 16, 0);
	//GLCDD_Rectangle(2, 11, 124, 12, 0);
	
	r.x = 1;
	r.y = 17;
	r.w = GLCDD_StringWidth(fnt_spaceLex_5, "40");
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "40");
	
	r.y = 27;
	r.w = GLCDD_StringWidth(fnt_spaceLex_5, "30");
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "30");
	
	r.y = 37;
	r.w = GLCDD_StringWidth(fnt_spaceLex_5, "20");
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "20");
	
	r.y = 47;
	r.w = GLCDD_StringWidth(fnt_spaceLex_5, "10");
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "10");
	
	// Vertikale Linie
	GLCDD_Line(10,20,10,60);
	
	// 40
	GLCDD_Line(8,20,10,20);
	
	// 35
	GLCDD_Line(9,25,10,25);
	
	// 30
	GLCDD_Line(8,30,10,30);
	
	// 25
	GLCDD_Line(9,35,10,35);
	
	// 20
	GLCDD_Line(8,40,10,40);
	
	// 15
	GLCDD_Line(9,45,10,45);
	
	// 10
	GLCDD_Line(8,50,10,50);
	
	// 5
	GLCDD_Line(9,55,10,55);
	
	// Horizontale Linie
	GLCDD_Line(10,60,118,60);
	
	curPos = 10;
}

void drawBalken(int grad){
	
	curPos = curPos + 2;
	int curBalken = 60-grad;
	// Vertikale Linie
	GLCDD_Line(curPos,curBalken,curPos,60);
}


void drawPoint(int grad){
	curPos = curPos + 1;
	int curBalken = 60-grad;
	GLCDD_SetPixel(curPos, curBalken);
}

void displayTemperatureScreenLive(int sensorID){
	displayBorder();
	int i;
	
	GLCDD_Rect r;
	
	r.w = 128 - 10;
	//r.h = -1;
	r.h = 5;
	
	// Humidity
	r.x = 20;
	r.y = 12;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "TEMPERATURE %d LIVE", sensorID);
	
	for (i=0; i < 108; i++){
		Data data;
		data=bufferPop(buffer);
		drawPoint(data.temperature[sensorID].current);
	}
}

void displayHumidityScreenLive(){
	displayBorder();
	int i;
	GLCDD_Rect r;
	
	r.w = 128 - 10;
	//r.h = -1;
	r.h = 5;
	
	// Humidity
	r.x = 20;
	r.y = 12;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "HUMIDITY LIVE");
	
	for (i=0; i < 108; i++){
		Data data;
		data=bufferPop(buffer);
		drawPoint(data.humidity.current);
	}
}

void displayECScreenLive(int sensorID){
	displayBorder();
	int i;
	
	GLCDD_Rect r;
	
	r.w = 128 - 10;
	//r.h = -1;
	r.h = 5;
	
	// Humidity
	r.x = 20;
	r.y = 12;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "EC-LEVEL %d LIVE", sensorID);
	
	for (i=0; i < 108; i++){
		Data data;
		data=bufferPop(buffer);
		drawPoint(data.ecLevel[sensorID].current);
	}
}
