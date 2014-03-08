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
	GLCDD_Line(15,35,55,35);
	
}

void displayTemperatureScreen(){
	displayBorder();
	
}