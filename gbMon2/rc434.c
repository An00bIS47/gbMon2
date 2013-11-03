//
//  rc434.c
//  gbMon2
//
//  Created by michael on 03.11.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include "rc434.h"

char* getFanAsString(){
	sem_wait(&semaLockFan);       // down semaphore
	int result;
	result=fanToggle;
	sem_post(&semaLockFan);       // up semaphore
	
	if (result==1){
		return "1";
	} else {
		return "0";
	}
}

int getFan(){
	sem_wait(&semaLockFan);       // down semaphore
	int result;
	result=fanToggle;
	sem_post(&semaLockFan);       // up semaphore
	return result;
}

int setFan(){
	sem_wait(&semaLockFan);       // down semaphore
	int result;

	if (fanToggle==1) {
		sendRC(fanSystemcode, fanUnitcode, 0);		// Evtl Befehl 3x senden --> Settingsfile
		fanToggle=0;
	} else {
		sendRC(fanSystemcode, fanUnitcode, 1);		// Evtl Befehl 3x senden --> Settingsfile
		fanToggle=1;
	}
	
	result=fanToggle;
	sem_post(&semaLockFan);       // up semaphore
	
	// Save toggle value !!!
	debugPrint(true, true, "Saving Settings to SETTINGSFILE ...", false, "RC434");
	Settings_Add("fan", "toggle", getFanAsString());
	Settings_Save(SETTINGSFILE);
	debugPrint(false, false, "OK", true, "RC434");
	
	// Update Display
	setUpdateDisplay(true);
	
	return result;
}

int sendRC(char* systemCode, int unitCode, int command) {
	
    /*
     output PIN is hardcoded for testing purposes
     see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     for pin mapping of the raspberry pi GPIO connector
     */
    
    //char* systemCode = argv[1];
    //int unitCode = atoi(argv[2]);
    //int command  = atoi(argv[3]);
    
    //if (wiringPiSetup () == -1) return 1;
	//printf("sending systemCode[%s] unitCode[%i] command[%i]\n", fanSystemcode, fanUnitcode, command);
    
	struct RCSwitch* rcs = newRCSwitch();
    RCSwitch_enableTransmit(rcs, PINDHT);
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
	//printf("Done!\n");
	return 0;
	
}


