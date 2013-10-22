//
//  main.c
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
#include "main.h"

pthread_t tid[2];
pthread_t pThreadServer;        // Network Server Thread
pthread_t pThreadSPI;           // SPI Send Command to ATmega / Arduino

float    appVersion              =   0.1;

void  INThandler(int sig) {
    //char  c;
    
    // Join Threads
    signal(sig, SIG_IGN);
    pthread_join (pThreadServer, NULL);
    pthread_join (pThreadSPI, NULL);
	
	sem_destroy(&semaLockSpiCommand); /* destroy semaphore */
	sem_destroy(&semaLockSpiSendCommand); /* destroy semaphore */
    printf("Goodbye!\n");
    
    
    /*
    printf("You pressed Ctrl-C:\n"
           "Do you really want to quit? [y/n] ");
    c = getchar();
    if (c == 'y' || c == 'Y') {
        exit(0);
    } else
        signal(SIGINT, INThandler);
    getchar(); // Get new line character
     */
}



int main(int argc, const char * argv[]) {
    // Exit Abfangen
    //signal(SIGINT, INThandler);
    //signal (SIGQUIT, INThandler);
    // Print Header
    
    
    debugPrint(true, true, "============================================================", true, "MAIN");
    debugPrint(true, true, "                Raspberry Pi - gbMon", true, "MAIN");
    debugPrint(true, true, "============================================================", true, "MAIN");

    // load settings file
    char* settings_file = NULL;
    int settings = 0;
    if(argc == 2) {
        settings_file = argv[1];
        settings = Settings_Load(argv[1]);
    }
    if(!settings) {
        settings_file = "default.conf";
        if(!Settings_Load("default.conf")) {
            //printf("%s\r\n", _lng(ERROR_LOADING_SETTINGS));
            debugPrint(false, false, "Loading Settings!", false, "ERROR");
            return 0;
        }
    }
    
    // Load Settings
    appNetworkInterface = Settings_Get("general", "network");
    appPort = atoi(Settings_Get("general", "port"));
    appDebugMode = atoi(Settings_Get("general", "debugMode"));
    debugPrint(true, true, "Loading Settings ...", false, "MAIN");
    debugPrint(false, false, "OK", true, "MAIN");

    // Print Debug Infos
    debugPrintInfo();
    debugPrintPinLayout();
	
	// Init Semaphores
	sem_init(&semaLockSpiCommand, 1, 1);      /* initialize mutex to 1 - binary semaphore */
    /* second param = 0 - semaphore is local */
	sem_init(&semaLockSpiSendCommand, 1, 1);      /* initialize mutex to 1 - binary semaphore */
	sem_init(&semaLockSpiProcess, 1, 1);      /* initialize mutex to 1 - binary semaphore */
    
    // Starting Netork Thread
    debugPrint(true, true, "Starting Network-Server ...", false, "MAIN");
    pthread_create (&pThreadServer, NULL, serverMain, 1000);
    debugPrint(false, false, "OK", true, "MAIN");

    // Starting SPI Thread
    debugPrint(true, true, "Starting SPI-Thread ...", false, "MAIN");
    pthread_create (&pThreadSPI, NULL, spiMain, NULL);
    debugPrint(false, false, "OK", true, "MAIN");
	
	
	
	delay(3000);
	//setSPICommand("HELLO WORLD\n");
	
    for (;;) {
        //printf("\n");
		
		/*
		printf("Thread Main: Waiting to enter critical region...\n");
		sem_wait(&semaLockCommand);       // down semaphore
		// START CRITICAL REGION
		printf("Thread Main: Now in critical region...\n");
		
		spiCommand="Hello World";
		
		printf("Thread Main: Exiting critical region...\n");
		// END CRITICAL REGION
		sem_post(&semaLockCommand);       // up semaphore
		*/
		//setSPICommand("HELLO WORLD");

		//delay(3000);
		
    }

    // Join Threads
    pthread_join (pThreadServer, NULL);
    pthread_join (pThreadSPI, NULL);
	
	sem_destroy(&semaLockSpiCommand); /* destroy semaphore */
	sem_destroy(&semaLockSpiSendCommand); /* destroy semaphore */
	sem_destroy(&semaLockSpiProcess); /* destroy semaphore */
    return 0 ;
}


