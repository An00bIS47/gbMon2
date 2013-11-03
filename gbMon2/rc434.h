//
//  rc434.h
//  gbMon2
//
//  Created by michael on 03.11.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbMon2_rc434_h
#define gbMon2_rc434_h

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>

#include "main.h"
#include "server.h"
#include "debug.h"
#include "helpers.h"
#include "settings.h"
#include "lcd.h"
#include "fonts.h"
#include "lcddisplay.h"
#include "rcswitchwrapper.h"


#define PINDHT 2

int sendRC(char* systemCode, int unitCode, int command);
int setFan();
int getFan();
char* getFanAsString();

#endif
