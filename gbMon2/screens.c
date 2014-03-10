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
		displayECScreenLive(2);
	}
	
	if ((screensPosX==1) && (screensPosY==0)) {
		displayECScreenLive(1);
	}
	
	if ((screensPosX==2) && (screensPosY==0)) {
		displayECScreenLive(0);
	}
	
	if ((screensPosX==3) && (screensPosY==0)) {
		displayHumidityScreenLive();
	}
	
	if ((screensPosX==4) && (screensPosY==0)) {
		displayMainScreen();
	}
	
	if ((screensPosX==5) && (screensPosY==0)) {
		displayTemperatureScreenLive(0);
	}
	
	if ((screensPosX==6) && (screensPosY==0)) {
		displayTemperatureScreenLive(1);
	}
	
	if ((screensPosX==7) && (screensPosY==0)) {
		displayTemperatureScreenLive(2);
	}
	
}

void goUp(){
	if (screensPosY > 0) {
		screensPosY--;
	}
	printf("goUP ---> ScreenPosX: %d - ScreenPosY: %d\n", screensPosX, screensPosY);
	setUpdateDisplay(true);
}

void goDown(){
	if (screensPosY < 2) {
		screensPosY++;
	}
	printf("goDown ---> ScreenPosX: %d - ScreenPosY: %d\n", screensPosX, screensPosY);
	setUpdateDisplay(true);
}

void goRight(){
	if (screensPosX < 7) {
		screensPosX++;
	}
	printf("goRight ---> ScreenPosX: %d - ScreenPosY: %d\n", screensPosX, screensPosY);
	setUpdateDisplay(true);
}

void goLeft(){
	if (screensPosX > 0) {
		screensPosX--;
	}
	printf("goLeft ---> ScreenPosX: %d - ScreenPosY: %d\n", screensPosX, screensPosY);
	setUpdateDisplay(true);
}