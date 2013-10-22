//
//  arduino-spi.h
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbmon2_arduino_spi_h
#define gbmon2_arduino_spi_h

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <semaphore.h>  /* Semaphore */

//#include "main.h"


sem_t semaLockSpiCommand;
sem_t semaLockSpiSendCommand;
sem_t semaLockSpiProcess;

char *spiCommand;
int spiProcess;

void setSPIProcess();

/**
	<#Description#>
	@param str <#str description#>
 */
void setSPICommand(char *str);

/**
	<#Description#>
	@param c <#c description#>
 */
void spiSendCommand();

/**
	<#Description#>
	@param void <#void description#>
	@returns <#return value description#>
 */
int spiMain (void);


#endif
