//
//  screenGraph.h
//  gbMon2
//
//  Created by michael on 09.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#ifndef gbMon2_screenGraph_h
#define gbMon2_screenGraph_h

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "lcd.h"
#include "lcddisplay.h"
#include "main.h"

//GLCDD_Font* fnt_spaceLex_5;

int rand_lim(int limit);
void displayBorder();
void displayTemperatureScreenLive(int sensorID);
void displayECScreenLive(int sensorID);
void displayHumidityScreenLive();
void drawPoint(int grad);
void drawBalken(int grad);

#endif
