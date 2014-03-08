//
//  screenTemperature.c
//  gbMon2
//
//  Created by michael on 08.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#include "screenTemperature.h"


void displayBorder(){
	//GLCDD_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)

	GLCDD_Rect r;
	
	r.x = 6;
	r.w = 128 - 12;
	r.h = -1;
	
	fnt_spaceLex_5=createFont(spaceLex_5);
	
	//GLCDD_Rectangle(0,  9, 128, 16, 0);
	//GLCDD_Rectangle(2, 11, 124, 12, 0);

	r.x = 2;
	r.y = 18;
	r.w = GLCDD_StringWidth(fnt_spaceLex_5, "40");
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "40");
	
	
	
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
	
}

void displayTemperatureScreen(){
	displayBorder();
	
}