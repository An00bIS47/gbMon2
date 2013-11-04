//
//  rrdtool.h
//  gbMon2
//
//  Created by michael on 04.11.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbMon2_rrdtool_h
#define gbMon2_rrdtool_h

#include <stdio.h>
#include <rrd.h>
#include <unistd.h>

#include "main.h"

void createDBs();
void createDBTemperature();
void createDBHumidity();
void createDBlightFan();
void createDBecLevel();

void updateDBs();
void updateDBTemperature();
void updateDBHumidity();
int mainRRD();

#endif
