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
#include "install.h"
#include "data.h"
#include "ringbuffer.h"
#include "ioBridge.h"


//#include "mdns.h"		// not used anymore
//#include "mdnsd.h"	// not used anymore
#include "avahi.h"


#define MAXTIMINGS 85
#define DHTPIN 1
#define NOTEMPSENSOR 3						// Number of Temperature Sensors
#define NOECSENSORS 3						// Number of EC Sensors
#define SETTINGSFILE "/home/pi/.gbmon/ramdisk/gbmon2.conf"
#define IOBRIDGEINT 4						// Interrupt from the atmega328p-pu
#define IOBRIDGEDATA 5						// data from the atmega
#define IOBRIDGECLK 6						// clk from the atmega

/*
 * Semaphores
 */
sem_t semaLockUpdate;						// Semaphore for Display Update
sem_t semaLockInfo;							// Semaphore for Infos
sem_t semaLockFan;							// Semaphore for Fan
sem_t semaLockFanTemp;						// Semaphore for Fan due Temp
sem_t semaLockCam;							// Semaphore for Cam
sem_t semaLockPrint;						// Semaphore for Print

/*
 * Global Variables
 */
float   appVersion;
int     appDebugMode;						// 1 Network; 2 SPI
char*   appNetworkInterface;
int     appPort;
bool	clientIsConnected;
bool	updateDisplay;
char*	piSerial[17];							// Serial Number of Raspberry Pi
											// needed for Apple Push Notification Service and EasyAPNS
											// will be connected to the device token id from the iPhone via an email adress or else

// Fan
int	fanToggle;
int fanToggleTemp;
char*	fanSystemcode;
int		fanUnitcode;



/*
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
*/


/*
 *	Setter
 */
void resetHumidity();
void resetTemperature();
void setUpdateDisplay(bool value);
void setLightValue(int value);
void setFanToggleTemp(int value);

/*
 *	Getter
 */
int getFanToggleTemp();
bool getUpdateDisplay();
//char* getTemperature();
//char* getHumidity();
//int getLightValue();


/*
 *	Functions
 */
//void createBonjourService();
int sendRC(char* systemCode, int unitCode, int command);
void closeApp();
void INThandler(int);
int main(int argc, char * argv[]);

#endif
