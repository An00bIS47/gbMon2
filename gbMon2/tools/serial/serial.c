//
//  serialRead.c
//  gbMon2
//
//  Created by michael on 07.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringSerial.h>

#define NUMBERECSENSORS 3


int binaryToDecimal(const char * str){
    int val = 0;
	
    while (*str != '\0')
        val = 2 * val + (*str++ - '0');
    return val;
}



/*
 * Substring
 *********************************************************************************
 */
char* substring(const char* str, size_t begin, size_t len){
    if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
        return 0;
    
    return strndup(str + begin, len);
}

int main ()
{
	int fd ;
	char inData[255]; // Allocate some space for the string
	char buffer[255];
	char inChar = -1; // Where to store the character read
	int spos = 0; //Index into array; where to store the character
	char buttons[8];
	char startFrame[8];
	char endFrame[8];
	char ldr[8];
	char ecSensors[NUMBERECSENSORS][8];
	int i;

	printf("Welcome to SerialRead\n");
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
	{
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
				//printf("%s",inData);
				
				strcpy(startFrame,substring(inData,curPos,8));
				//printf("StartFrame: %s\n",startFrame);
				curPos=curPos+8;
				
				strcpy(buttons,substring(inData,curPos,8));
				printf("Buttons:	%s\n",buttons);
				curPos=curPos+8;
				
				strcpy(ldr,substring(inData,curPos,8));
				printf("LDR:		%s - %d \n",ldr, binaryToDecimal(ldr));
				curPos=curPos+8;
				
				for (i=0; i<NUMBERECSENSORS; i++) {
					strcpy(ecSensors[i],substring(inData,curPos,8));
					printf("EC %d:	%s\n",i, ecSensors[i]);
					curPos=curPos+8;
				}
				
				strcpy(endFrame,substring(inData,curPos,8));
				//printf("endFrame:	%s\n",endFrame);
				curPos=0;
				
				if ((strcmp(endFrame, "11100111") == 0) && (strcmp(startFrame, "11100111") == 0)) {
					//---> Daten übergeben
				}
				
				spos = 0;
				//fflush (stdout) ;
			} // if (inChare == 13
		}
	}
}





