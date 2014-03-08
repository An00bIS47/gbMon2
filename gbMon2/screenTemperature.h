//
//  screenTemperature.h
//  gbMon2
//
//  Created by michael on 08.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#ifndef gbMon2_screenTemperature_h
#define gbMon2_screenTemperature_h

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "lcd.h"
#include "lcddisplay.h"

//GLCDD_Font* fnt_spaceLex_5;

int rand_lim(int limit);
void displayBorder();
void displayTemperatureScreen();

#endif
