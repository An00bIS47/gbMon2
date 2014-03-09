//
//  screens.c
//  gbMon2
//
//  Created by michael on 09.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#include "screens.h"


void getScreen(){
	
	if ((screensPosX==0) && (screensPosY==0)) {
		displayECScreen(2);
	}
	
	if ((screensPosX==1) && (screensPosY==0)) {
		displayECScreen(1);
	}
	
	if ((screensPosX==2) && (screensPosY==0)) {
		displayECScreen(0);
	}
	
	if ((screensPosX==3) && (screensPosY==0)) {
		displayHumidityScreen();
	}
	
	if ((screensPosX==4) && (screensPosY==0)) {
		displayMainScreen();
	}
	
	if ((screensPosX==5) && (screensPosY==0)) {
		displayTemperatureScreen(0);
	}
	
	if ((screensPosX==6) && (screensPosY==0)) {
		displayTemperatureScreen(1);
	}
	
	if ((screensPosX==7) && (screensPosY==0)) {
		displayTemperatureScreen(2);
	}
	
}

void goUp(){
	screensPosY--;
}

void goDown(){
	screensPosY++;
}

void goRight(){
	screensPosX++;
}

void goLeft(){
	screensPosX--;
}