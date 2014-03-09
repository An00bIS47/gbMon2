//
//  lcddisplay.h
//  gbMon2
//
//  Created by michael on 30.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbMon2_lcddisplay_h
#define gbMon2_lcddisplay_h

#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include "lcd.h"
#include "helpers.h"
#include "fonts.h"
#include <time.h>
#include "wand.h"
#include "screenGraph.h"

void displayStatusBar();
void displayMainScreen();
void* displayMain(void *args);
void displaySettings();
void displayWifi(int strength);
void displayClient();
void displayFan();

#endif
