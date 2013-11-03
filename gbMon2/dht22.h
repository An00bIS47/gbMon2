//
//  dht22.h
//  gbMon2
//
//  Created by michael on 31.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbMon2_dht22_h
#define gbMon2_dht22_h


#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#include "main.h"
#include "helpers.h"

#define MAXTIMINGS 85
#define DHTPIN 1

uint8_t sizecvt(const int read);
int read_dht22_dat();

#endif
