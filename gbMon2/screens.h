//
//  screens.h
//  gbMon2
//
//  Created by michael on 09.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//
//											 #				 		#
//											######			  	######
//										   #######			  	#######
//											######			  	######
//											 #					  	#
//
//	|--------|	|--------|	|--------|	|--------|	|---------|	|--------|	|--------|	|--------|
//	|   EC   |	|   EC   |	|   EC   |	|   HUM  |	|   MAIN  |	|  TEMP  |	|  TEMP  |	|  TEMP  |	LIVE				0
//	|--------|	|--------|	|--------|	|--------|	|---------|	|--------|	|--------|	|--------|
//																												###
//	|--------|	|--------|	|--------|	|--------|	|---------|	|--------|	|--------|	|--------|				###
//	|   EC   |	|   EC   |	|   EC   |	|   HUM  |	| SETTING |	|  TEMP  |	|  TEMP  |	|  TEMP  |	HOURs		###		1
//	|--------|	|--------|	|--------|	|--------|	|---------|	|--------|	|--------|	|--------|			   #####
//																												###
//	|--------|	|--------|	|--------|	|--------|	|---------|	|--------|	|--------|	|--------|				 #
//	|   EC   |	|   EC   |	|   EC   |	|   HUM  |	|		  |	|  TEMP  |	|  TEMP  |	|  TEMP  |	DAYs				2
//	|--------|	|--------|	|--------|	|--------|	|---------|	|--------|	|--------|	|--------|
//
//		0			1			2			3			4			5			6			7
//
//
#ifndef gbMon2_screens_h
#define gbMon2_screens_h

#include <stdio.h>
#include "screenGraph.h"

//int screens[3][8];
int screensPosX;
int screensPosY;

void goUp();
void goDown();
void goLeft();
void goRight();

void getScreen();

#endif
