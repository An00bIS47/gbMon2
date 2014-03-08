//
//  ioBridge.c
//  gbMon2
//
//  Created by michael on 06.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//
#include "ioBridge.h"

void* ioBridgeMain (void *args){
	int fd ;
	char inData[255]; // Allocate some space for the string
	char buffer[255];
	char inChar = -1; // Where to store the character read
	int spos = 0; //Index into array; where to store the character
	char buttons[8];
	char startFrame[8];
	char endFrame[8];
	char ldr[8];
	int ecSensors[NUMBERECSENSORS];
	int i;
	int sensorID;
	
	//printf("Welcome to SerialRead\n");
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0) {
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	
	// Loop, getting and printing characters
	for (;;) {
		int curPos=0;
		while (serialDataAvail(fd)) {
			inChar = (serialGetchar(fd));
			//fprintf(stdout,"%c",inChar);
			buffer[spos]= inChar;
			spos++;
			
			if (inChar == 10) {
				strcpy(inData,buffer);
				printf("%s",inData);
				
				strcpy(startFrame,substring(inData,curPos,8));
				printf("StartFrame:	%s\n",startFrame);
				curPos=curPos+8;
				
				strcpy(buttons,substring(inData,curPos,8));
				printf("Buttons:	%s\n",buttons);
				curPos=curPos+8;
				
				strcpy(ldr,substring(inData,curPos,8));
				printf("LDR:		%s - %d \n",ldr, binaryToDecimal(ldr));
				curPos=curPos+8;
				
				for (i=0; i<NUMBERECSENSORS; i++) {
					ecSensors[i]=binaryToDecimal(substring(inData,curPos,8));
					
					printf("EC %d:		%s - %d\n",i,substring(inData,curPos,8), ecSensors[i]);
					curPos=curPos+8;
				}
				
				strcpy(endFrame,substring(inData,strlen(inData)-10,8));
				printf("endFrame:	%s\n",endFrame);
				curPos=0;
				
				if (strcmp(startFrame, "11100111") == 0) {
					if (strcmp(startFrame, "11100111") == 0) {
						printf("HERRER\n");
					}
				}
				
				if ((strcmp(endFrame, "11100111") == 0) && (strcmp(startFrame, "11100111") == 0)) {
					//---> Daten übergeben
					
					setLightValue(binaryToDecimal(ldr));
					//data.lightValue = binaryToDecimal(ldr);
					
					
					printf("HERRER\n");
					for (sensorID=0; i<NUMBERECSENSORS; sensorID++) {
						char strMin[4];
						char strMax[4];
						char strSensorID[2];
						
						sprintf(strMin,"min%d",sensorID);
						sprintf(strMax,"max%d",sensorID);
						sprintf(strSensorID, "%d", sensorID);
						
						sem_wait(&semaLockInfo);
						
						//if (data.lightValue != binaryToDecimal(ldr)){
						//	data.lightValue = binaryToDecimal(ldr);
						//}
						
						if (data.ecLevel[sensorID].max < ecSensors[i]) {
							data.ecLevel[sensorID].max = ecSensors[i];
							data.ecLevel[i].max=ecSensors[i];
	
							Settings_Add("ecLevel", strMax, ecSensors[i]);
							Settings_Save(SETTINGSFILE);
							
						}
						if ((data.ecLevel[sensorID].min > ecSensors[i]) || (data.ecLevel[sensorID].current == 0)){
							data.ecLevel[sensorID].min = ecSensors[i];
							data.ecLevel[sensorID].min = ecSensors[i];
							Settings_Add("ecLevel", strMin, ecSensors[i]);
							Settings_Save(SETTINGSFILE);
						}
						
						
						if (data.ecLevel[sensorID].current != ecSensors[i]){
							data.ecLevel[sensorID].current = ecSensors[i];

							setUpdateDisplay(true);
						}
						sem_post(&semaLockInfo);
					}
				}
				sem_wait(&semaLockInfo);
				debugPrint(true, true, "LDR: ", false, "IOBRIDGE");
				debugPrint(false, false, data.lightValue, true, "");
				
				for (sensorID=0; i<NUMBERECSENSORS; sensorID++) {
					debugPrint(true, true, "EC  ", false, "IOBRIDGE");
					debugPrint(false, false, i, false, "");
					debugPrint(false, false, ": ", false, "IOBRIDGE");
					debugPrint(false, false, data.ecLevel[sensorID].current, true, "");
				}
				sem_post(&semaLockInfo);
				
				
				spos = 0;
				//fflush (stdout) ;
			} // if (inChare == 13
		}
	}
}