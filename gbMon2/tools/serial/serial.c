//
//  serial.c
//  gbMon2
//
//  Created by michael on 07.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringSerial.h>
#define NOECSENSORS 3						// Number of EC Sensors

/*
 * Substring
 *********************************************************************************
 */
char* substring(const char* str, size_t begin, size_t len){
    if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
        return 0;
    
    return strndup(str + begin, len);
}

int main (){
	int fd ;
	
	char inData[255];	// Allocate some space for the string
	char inChar = -1;	// Where to store the character read
	int spos = 0;		//Index into array; where to store the character
	
	printf("Welcome to SerialRead\n");
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0){
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	
	char buttons[8+1];
	char ldr[8+1];
	int counter=0;
	int i;
	int curPos=8;
	
	for (;;) {
		while (serialDataAvail(fd)) {
			inChar = (serialGetchar(fd));
			inData[spos]= inChar;
			spos++;

			//inData[spos]= '\0';
			if (inChar == 10) {			// LF 10 ; CR 13
				printf("%s\n",inData);
				if (strncmp(inData,"11100111",8) == 0) {
					printf("*** FRAME ***\n");
				}
				
				printf("Buttons: %s\n", substring(inData,curPos,8));
				curPos=curPos+8;
				printf("LDR: %s\n", substring(inData,16,8));
				curPos=curPos+8;
				
				for (i=0; i<NOECSENSORS; i++) {
					printf("EC %d: %s\n",i, substring(inData,curPos,8));
					curPos=curPos+8;
				}
				
				if (strcmp(substring(inData,curPos,8),"11100111") == 0) {
					printf("*** FRAME ***\n");
				}
				
				spos = 0;
				//inData[spos]='\0';
				
				//fflush (stdout) ;
			} // if (inChare == 13

		} // while
	} //for
}
