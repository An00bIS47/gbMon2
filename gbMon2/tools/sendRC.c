//
//  sendRC.c
//  gbMon2
//
//  Created by michael on 03.11.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
// gcc -c sendRC.c -o sendRC.o -lwiringPi -g
// g++ sendRC.o ../rcswitchwrapper.o ../rcswitch.o -o sendRC -lwiringPi -g

// systemcode 10001

#include <stdio.h>
#include "../rcswitchwrapper.h"
#include <stdlib.h>


int main(int argc, char *argv[]) {
	
    /*
     output PIN is hardcoded for testing purposes
     see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     for pin mapping of the raspberry pi GPIO connector
     */
    int PIN = 2;					// PIN 2 (wiringPi)
    char* systemCode = argv[1];
    int unitCode = atoi(argv[2]);
    int command  = atoi(argv[3]);
    
    if (wiringPiSetup () == -1) return 1;
	printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
	
	
	//RCSwitch mySwitch = RCSwitch();
	//mySwitch.enableTransmit(PIN);
    
    
	struct RCSwitch* rcs = newRCSwitch();
    RCSwitch_enableTransmit(rcs, PIN);
    //printf("%i\n", MyClass_int_get(c));
    
	
	
	switch(command) {
        case 1:
            RCSwitch_switchOn(rcs, systemCode, unitCode);
			deleteRCSwitch(rcs);
            break;
        case 0:
            RCSwitch_switchOff(rcs, systemCode, unitCode);
			deleteRCSwitch(rcs);
            break;
        default:
            printf("command[%i] is unsupported\n", command);
			deleteRCSwitch(rcs);
            return -1;
    }
	printf("Done!\n");
	return 0;
	
}
