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
	printf("Welcome to SerialRead\n");
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0){
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	
	// Loop, getting and printing characters
	
	for (;;){
		
		
		while (serialDataAvail (fd))
		{
			printf (" -> %3d", serialGetchar (fd)) ;
			fflush (stdout) ;
		}
		
		/*
		putchar (serialGetchar (fd)) ;
		if (serialGetchar (fd)=='\n') {
			printf("NEWLINE");
		}
		fflush (stdout) ;
		 */
	}
}
