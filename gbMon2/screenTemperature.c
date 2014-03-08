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

	
	// Vertikale Linie
	GLCDD_Line(10,20,10,60);
	
	// 40
	GLCDD_Line(6,20,10,20);
	
	// 35
	GLCDD_Line(8,25,10,25);
	
	// 30
	GLCDD_Line(6,30,10,30);
	
	// 25
	GLCDD_Line(8,35,10,35);
	
	// 20
	GLCDD_Line(6,40,10,40);

	// 15
	GLCDD_Line(8,45,10,45);
	
	// 10
	GLCDD_Line(6,50,10,50);
	
	// 5
	GLCDD_Line(8,55,10,55);
	
	// Horizontale Linie
	GLCDD_Line(10,60,118,60);
	
}

void displayTemperatureScreen(){
	displayBorder();
	
}