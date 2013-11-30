//
//  ds18b20.h
//  gbMon2
//
//  Created by michael on 27.11.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbMon2_ds18b20_h
#define gbMon2_ds18b20_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "ds18b20.h"
#include "dht22.h"
#include "main.h"


void readTemperatureDS(int sensorID);
void* sensorsMain(void *args);

#endif
