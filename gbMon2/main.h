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
#include <semaphore.h>

#include "udpServer.h"
#include "debug.h"
#include "helpers.h"
#include "settings.h"
#include "lcd.h"
#include "fonts.h"
#include "lcddisplay.h"
#include "dht22.h"
#include "ds18b20.h"
#include "rrdtool.h"

#define MAXTIMINGS 85
#define DHTPIN 1
#define NOTEMPSENSOR 3						// Number of Temperature Sensors
#define SETTINGSFILE "/home/pi/gbmon2/gbMon2/default.conf"
/*
 * Semaphores
 */
sem_t semaLockUpdate;						// Semaphore for Display Update
sem_t semaLockInfo;							// Semaphore for Infos
sem_t semaLockFan;							// Semaphore for Fan
sem_t semaLockCam;							// Semaphore for Cam

/*
 * Global Variables
 */
float   appVersion;
int     appDebugMode;						// 1 Network; 2 SPI
char*   appNetworkInterface;
int     appPort;
bool	clientIsConnected;
bool	updateDisplay;


// Fan
int	fanToggle;
char*	fanSystemcode;
int		fanUnitcode;

struct	info current;

struct info {								// Info
    float temperature[NOTEMPSENSOR];		// Temperature
	float humidity;							// Humidity
	float maxTemp[NOTEMPSENSOR];			// max Temperature
	float maxHum;							// max Humidity
	float minTemp[NOTEMPSENSOR];			// min Temperature
	float minHum;							// min Humidity
	int lightValue;							// LightValue
};


/*
 *	Setter
 */
void setUpdateDisplay(bool value);
void setLightValue(int value);

/*
 *	Getter
 */
bool getUpdateDisplay();
char* getTemperature();
char* getHumidity();
int getLightValue();

/*
 *	Functions
 */
int sendRC(char* systemCode, int unitCode, int command);
void closeApp();
void INThandler(int);
int main(int argc, char * argv[]);

#endif
