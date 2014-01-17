//
//  helpers.h
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbmon2_helpers_h
#define gbmon2_helpers_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>
#include <ifaddrs.h>


#include "main.h"


typedef unsigned char byte;


uint32_t parseIPV4string(char* ipAddress);
int getWifiStrength();
unsigned long long getUptime();
char* substring(const char* str, size_t begin, size_t len);
const char *printBinary(int x);
char* getTimeShort();
char * getTime();
char * getVersion();
char* getLightValueAsString();
char * getIP(char *interface);
byte reverseByte(byte a);


#endif
