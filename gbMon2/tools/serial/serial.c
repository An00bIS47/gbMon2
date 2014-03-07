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
	char inChar = -1; // Where to store the character read
	int spos = 0; //Index into array; where to store the character
	char buttons[8];
	char ldr[8];
	int curPos=8;
	printf("Welcome to SerialRead\n");
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	
	// Loop, getting and printing characters
	
	for (;;) {
		
		while (serialDataAvail(fd)) {
			inChar = (serialGetchar(fd));
			//fprintf(stdout,"%c",inChar);
			inData[spos]= inChar;
			spos++;

			if (inChar == 10) {
				printf("%s",inData);
				strcpy(buttons,substring(inData,16,8));
				printf("Buttons: %s\n",buttons);
				
				//strcpy(ldr,substring(inData,16,8));
				//printf("LDR: %s\n",ldr);
				
				spos = 0;
				//fflush (stdout) ;
			} // if (inChare == 13
		}
	}
}





