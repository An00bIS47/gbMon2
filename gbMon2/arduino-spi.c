//
//  arduino-spi.c
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
#include "arduino-spi.h"

int spiProcess=0;

void setSPIProcess() {
	
	debugPrint(true, true, "Thread SPI setSPIProcess: Waiting to enter critical region ...", false, "SPI");
	sem_wait(&semaLockSpiProcess);       /* down semaphore */
	debugPrint(false, false, "OK", true, "");
	/* START CRITICAL REGION */

	
	spiProcess=(spiProcess + 1) % 2;
	//printf("Upadting spiCommand to: *%d*\n",spiProcess);
	debugPrint(true, true, "Thread SPI setSPIProcess: Updating spiProcess to: ", false, "SPI");
	
	if (spiProcess==1) {
		debugPrint(false, false, "*1*", true, "SPI");
	} else {
		debugPrint(false, false, "*0*", true, "SPI");
	}
//	debugPrint(false, false, spiProcess, false, "SPI");

	
	debugPrint(true, true, "Thread SPI setSPIProcess: Exiting critical region ...", false, "SPI");
	/* END CRITICAL REGION */
	sem_post(&semaLockSpiProcess);       /* up semaphore */
	debugPrint(false, false, "OK", true, "");
}


void clearSPICommand() {
	
	debugPrint(true, true, "Thread SPI clearSPICommand: Waiting to enter critical region ...", false, "SPI");
	sem_wait(&semaLockSpiCommand);       /* down semaphore */
	debugPrint(false, false, "OK", true, "");
	/* START CRITICAL REGION */

	spiCommand[0]='\0';
	//free(spiCommand);
	
	debugPrint(true, true, "Thread SPI clearSPICommand: Exiting critical region ...", false, "SPI");
	/* END CRITICAL REGION */
	sem_post(&semaLockSpiCommand);       /* up semaphore */
	debugPrint(false, false, "OK", true, "");
	setSPIProcess();
}

void setSPICommand(char *str) {
	
	debugPrint(true, true, "Thread SPI setSPICommand: Waiting to enter critical region ...", false, "SPI");
	sem_wait(&semaLockSpiCommand);       /* down semaphore */
	debugPrint(false, false, "OK", true, "");
	/* START CRITICAL REGION */

	
	spiCommand = malloc(strlen(str) * sizeof(char));
	
	spiCommand[0] = '\0';
	debugPrint(true, true, "Thread SPI setSPICommand: Updating spiCommand to: *", false, "SPI");
	debugPrint(false, false, str, false, "SPI");
	debugPrint(false, false, "*", true, "SPI");
	
	strcpy(spiCommand, str);
	//spiCommand[strlen(str)] = '\0';
	
	debugPrint(true, true, "Thread SPI setSPICommand: Exiting critical region ...", false, "SPI");
	/* END CRITICAL REGION */
	sem_post(&semaLockSpiCommand);       /* up semaphore */
	debugPrint(false, false, "OK", true, "");
	
	setSPIProcess();
}

void spiSendCommand(){
	
	debugPrint(true, true, "Thread SPI spiSendCommand: Waiting to enter critical region ...", false, "SPI");
	sem_wait(&semaLockSpiCommand);       /* down semaphore */
	debugPrint(false, false, "OK", true, "");
	
	//char *buffer[strlen(spiCommand)];
	char *buffer;
	
	buffer = malloc(strlen(spiCommand) * sizeof(char));
	strcpy(buffer, spiCommand);
	
	debugPrint(true, true, "Thread SPI spiSendCommand: Exiting critical region ...", false, "SPI");
	/* END CRITICAL REGION */
	sem_post(&semaLockSpiCommand);       /* up semaphore */
	debugPrint(false, false, "OK", true, "");
	
	debugPrint(true, true, "Thread SPI spiSendCommand: Waiting to enter critical region ...", false, "SPI");
	sem_wait(&semaLockSpiSendCommand);       /* down semaphore */
	/* START CRITICAL REGION */
	debugPrint(false, false, "OK", true, "");
	
	/*
	if (buffer[strlen(buffer)-1] != '\0'){
		debugPrint(true, true, "Thread SPI setSPICommand: Not ZERO terminated String *", false, "SPI");
		debugPrint(false, false, buffer, false, "SPI");
		debugPrint(false, false, "*", true, "SPI");
		buffer[strlen(buffer)-1] = '\0';
	}
	
	
	if (buffer[strlen(buffer)-1] != 0x00){
		debugPrint(true, true, "Thread SPI spiSendCommand: Not NL terminated String *", false, "SPI");
		debugPrint(false, false, buffer, false, "SPI");
		debugPrint(false, false, "*", true, "SPI");
		buffer[strlen(buffer)-1] = 0x00;
		printf("BUFFER NEWLINED: %s",buffer);
	}
	*/
	
	debugPrint(true, true,"=================================================",true,"SPI");
	debugPrint(true, true, "Thread SPI spiSendCommand: Sending command (buffer):", false, "SPI");
	debugPrint(false,false, buffer, true, "SPI");
	/*
	int i;
	for (i=0; i<sizeof(buffer); i++) {
		printf(" BUFFER%d: %1c : 0x%X - size: %lu\n", i, buffer[i] ,buffer[i], sizeof(buffer[i]));
	}
	printf(" BUFFER GESAMT: *%s* - size %lu \n", buffer, sizeof(buffer));
	*/
	
	printf("Buffer: \n");
	//printf("=================================================\n");
	printf("  Char    Dec    Hex       Binary \n");
	printf(" ------  -----  -----    ----------\n");
	
	int i;
	for (i=0; i<sizeof(buffer); i++) {
		printf("   %1c \t %3d \t 0x%02x \t 0b%s \n",(char) buffer[i],buffer[i], buffer[i],printBinary(buffer[i]));
	}
	
	wiringPiSPIDataRW(0, buffer, sizeof(buffer));
	//write(0, &buffer, 1);
	
	
	debugPrint(true, true, "Thread SPI spiSendCommand: After sending: (buffer):", false, "SPI");
	debugPrint(false,false, buffer, true, "SPI");
	debugPrint(true, true,"=================================================",true,"SPI");
	
	debugPrint(true, true, "Thread SPI spiSendCommand: Exiting critical region ...", false, "SPI");
	/* END CRITICAL REGION */
	sem_post(&semaLockSpiSendCommand);       /* up semaphore */
	debugPrint(false, false, "OK", true, "");
	
	free(buffer);
	clearSPICommand();
}

int spiMain (void) {
	
    debugPrint(true, true, "Initialising wiringPI SPI ...", false, "SPI");
    if (wiringPiSPISetup(0, 5000000) == -1) {
        debugPrint(true, false, "Initialising wiringPI SPI!", false, "ERROR");
        return -1;
    }
    debugPrint(false, false, "OK", true, "SPI");

    // HELLO WORLD\n
	// \n = 0x0A
	// \0 = 0x00
    //unsigned char c[] = {0x48, 0x45, 0x4C, 0x4C, 0x4F, 0x20, 0x57, 0x4F, 0x52, 0x4C, 0x44, 0x0A};
    //unsigned char c[] = {1, 2, 0, 0x4C, 0x4F, 0x20, 0x57, 0x4F, 0x52, 0x4C, 0x44, 0x0A};
	
	int i;
	
    for (;;) {
        //unsigned char buffer[sizeof(c)];
		char buff[1];

		//buff[0] = 0x48;
		
		buff[0] = 'H';
		/*
		buff[1] = 'A';
		buff[2] = 'L';
		buff[3] = 'O';
		buff[4] = 0x45;
		buff[5] = 0x00;
		
		
		
		for (i=0; i<sizeof(buff); i++) {
			printf(" BUFF%d: %1c : 0x%X - size: %lu\n", i, buff[i] ,buff[i], sizeof(buff[i]));
		}
		printf(" BUFF GESAMT: *%s* - size %lu \n", buff, sizeof(buff));
		
		setSPICommand(buff);
		
		//wiringPiSPIDataRW(0, buff, 2);
		
		//printf(" %c : %x \n", buff[1] ,buff[1]);
		/*

		// send data to SPI
		//wiringPiSPIDataRW(0, buff, 1);
		write(0, buff, 1);
		*/
		
		
		
		
		delay(2000);
		
		if (spiProcess==1) {
			spiSendCommand();
		}
    }
}
