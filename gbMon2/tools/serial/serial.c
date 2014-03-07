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
	
	for (;;) {
		while (serialDataAvail(fd)) {
			inChar = (serialGetchar(fd));
			inData[spos]= inChar;
			spos++;
			
			if (spos == 8) {
				if (strcmp(inData, "11100111")){
					// Rahmen Anfang
					printf("Rahmen Anfang \n");
					spos=0;
				} else {
					
					// Buttons
					if (counter==0) {
						for (i=0; i < 8; i++) {
							buttons[i]=inData[i];
						}
						buttons[8+1]='\0';
						counter++;
						spos=0;
						printf(" %s \n", buttons);
					}
					
					/*
					// LDR
					if (counter == 1) {
						for (i=0; i < 8; i++) {
							ldr[i]=inData[i];
						}
						ldr[8+1]='\0';
						counter++;
						spos=0;
						printf(" %s \n", ldr);
					}
					*/
					
					
				}
			}
			
			/*
			
			
			// fprintf(stdout,"%c",inChar);
			
			if (inChar != 13) {
				inData[spos]= inChar;
				spos++;
			}

			//inData[spos]= '\0';
			if (inChar == 10) {			// LF 10 ; CR 13
				printf("%s\n",inData);
				//if (strncmp(inData,"$GPG",4) == 0) {
				//	printf("*********** GPGGA found ************\n\n");
				//}
				spos = 0;
				//inData[spos]='\0';
				
				//fflush (stdout) ;
			} // if (inChare == 13
			*/
		} // while
	} //for
}
