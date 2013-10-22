//
//  main.h
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
#ifndef gbmon2_main_h
#define gbmon2_main_h
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include "server.h"
#include "debug.h"
#include "helpers.h"
#include "arduino-spi.h"
#include "settings.h"

/*
 * debugMode
 *   1 = Network
 *   2 = SPI
 */
float    appVersion;
int      appDebugMode;
char*    appNetworkInterface;
int      appPort;

/**
	signal Hanlder
	@param int SIGINT
 */
void     INThandler(int);

#endif
