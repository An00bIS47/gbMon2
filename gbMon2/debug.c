//
//  debug.c
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
#include "debug.h"

/**
	<#Description#>
 */

void debugPrintInfo(){
	
    if (appDebugMode > 0){
        char *buffer[255];
        sprintf(buffer,"Version: \t\t %.1f", appVersion);
        debugPrint(true, true, buffer, true, "INFO");
		sprintf(buffer,"Pi Serial: \t\t %s", piSerial);
        debugPrint(true, true, buffer, true, "INFO");
        sprintf(buffer,"Network Interface: \t %s", appNetworkInterface);
        debugPrint(true, true, buffer, true, "INFO");
        sprintf(buffer,"Port: \t\t %d", appPort);
        debugPrint(true, true, buffer, true, "INFO");
        sprintf(buffer,"IP-Address: \t %s", getIP(appNetworkInterface));
        debugPrint(true, true, buffer, true, "INFO");
        sprintf(buffer,"debugMode: \t\t %d", appDebugMode);
        debugPrint(true, true, buffer, true, "INFO");
		
		sprintf(buffer,"fanToggle: \t\t %d", fanToggle);
        debugPrint(true, true, buffer, true, "INFO");
		sprintf(buffer,"fanSystemcode: \t %s", fanSystemcode);
        debugPrint(true, true, buffer, true, "INFO");
		sprintf(buffer,"fanUnitcode: \t %d", fanUnitcode);
        debugPrint(true, true, buffer, true, "INFO");
		
    }
}


int myprintf( const char * format, ... ) {
	int rtn = 0;
	
	va_list args;
	
	va_start( args, format );
	
	rtn = vprintf( format, args );
	
	va_end( args );
	
	return rtn;
}


void debugPrint(bool info, bool prefix, char *msg, bool newLine, char *instance){
    if (appDebugMode==0 && strcmp(instance, "MAIN") == 0){
        if (prefix==true) {
            printf("%s >>> %s",getTime(), msg);
        } else {
            printf("%s",msg);
        }
        if (newLine == true) {
            printf("\n");
        }
    }
    if (appDebugMode > 0){
        if (prefix==true) {
            printf("%s >>> ",getTime());
        }
        if(strcmp(instance, "ERROR") == 0) {
            printf("\n");
        }
        if (info==true) {
            printf("DEBUG %s: %s",instance, msg);
        } else {
            printf("%s",msg);
        }
        if (newLine == true) {
            printf("\n");
        }
    }
}

void debugPrintPinLayout () {
    if (appDebugMode > 0){
        int piRev=1;
        printf ("%s >>> DEBUG INFO:\n",getTime());
        printf ("%s >>> DEBUG INFO:\n",getTime());
        printf ("%s >>> DEBUG INFO: RASPBERRY PI - PIN LAYOUT: \n", getTime()) ;
        printf("%s >>> DEBUG INFO: |-------------------------------------------------------------------------|\n", getTime());
        printf("%s >>> DEBUG INFO: | wiringPi | BCM GPIO |  Name  |   Header  |  Name  | BCM GPIO | wiringPi |\n", getTime());
        printf("%s >>> DEBUG INFO: |-------------------------------------------------------------------------|\n", getTime());
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "", "","3.3 V", "1", "2", "5 V", "", "");
        if (piRev==1) {
            printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "8", "0","SDA", "3", "4", "5 V", "", "");
        }
        if (piRev==2) {
            printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "8", "2","SDA", "3", "4", "5 V", "", "");
        }
        if (piRev==1) {
            printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "9", "1","SCL", "5", "6", "0 V", "", "");
        }
        if (piRev==2) {
            printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "9", "3","SCL", "5", "6", "0 V", "", "");
        }
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "7", "4","GPIO 7", "7", "8", "TxD", "14", "15");
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "", "","0 V", "9", "10", "RxD", "15", "16");
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "0", "17","GPIO 0", "11", "12", "GPIO 1", "18", "1");
        if (piRev==1) {
            printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "2", "21","GPIO 2", "13", "14", "0 V", "", "");
        }
        if (piRev==2) {
            printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "2", "27","GPIO 2", "13", "14", "0 V", "", "");
        }
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "3", "22","GPIO 3", "15", "16", "GPIO 4", "23", "4");
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "", "","3.3 V", "17", "18", "GPIO 5", "24", "5");
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "12", "10","MOSI", "19", "20", "0.0 V", "", "");
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "13", "9","MISO", "21", "22", "GPIO 6", "25", "6");
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "14", "11","SCLK", "23", "24", "CE 0", "8", "10");
        printf("%s >>> DEBUG INFO: | %8s | %8s | %6s | %3s | %3s | %6s | %8s | %8s | \n", getTime(), "", "","0 V", "15", "26", "CE 1", "7", "11");
        printf("%s >>> DEBUG INFO: |-------------------------------------------------------------------------|\n", getTime());
        printf ("%s >>> DEBUG INFO:\n",getTime());
    }
}
