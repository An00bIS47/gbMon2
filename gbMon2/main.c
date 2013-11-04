//
//  main.c
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
// g++ send.o ../rcswitchwrapper.o ../RCSwitch.o -o send -lwiringPi -g

#include "main.h"

pthread_t	tid[4];
pthread_t	pThreadServer;				// Network Server Thread
pthread_t	pThreadDisplay;				// SPI Send Command to ATmega / Arduino
pthread_t	pThreadSensors;				// Sensors Thread
pthread_t	pThreadRRD;					// RRD Thread

float		appVersion              =   0.1;
bool		updateDisplay			=	false;

// Fan
int		fanToggle				=	0;
char*		fanSystemcode			=	"10001";
int			fanUnitcode				=	1;

void setUpdateDisplay(bool value){
	sem_wait(&semaLockUpdate);       // down semaphore
	updateDisplay = value;
	sem_post(&semaLockUpdate);       // up semaphore
	
}

bool getUpdateDisplay(){
	bool result;
	sem_wait(&semaLockUpdate);       // down semaphore
	result = updateDisplay;
	sem_post(&semaLockUpdate);       // up semaphore
	return result;
}

char* getHumidity(){
	char *buf = (char *) malloc(5 * sizeof(char));
	sem_wait(&semaLockInfo);       // down semaphore
	sprintf (buf, "%.1f", current.humidity) ;
	sem_post(&semaLockInfo);       // up semaphore
	return buf;
}

char* getTemperature(){
	char *buf = (char *) malloc(5 * sizeof(char));
	sem_wait(&semaLockInfo);       // down semaphore
	sprintf (buf, "%.1f", current.temperature[0]) ;
	sem_post(&semaLockInfo);       // up semaphore
	return buf;
}

void initCurrentInfo(){
	sem_wait(&semaLockInfo);       // down semaphore
	int i;
	
	current.humidity=0.0;
	current.minHum=0.0;
	current.maxHum=0.0;
	
	for (i=0; i < NOTEMPSENSOR; i++){
		current.temperature[i]=0.0;
		current.maxTemp[i]=0.0;
		current.minTemp[i]=0.0;
	}
	sem_post(&semaLockInfo);       // up semaphore
}


void  INThandler(int sig) {
    //char  c;
    
    // Join Threads
    signal(sig, SIG_IGN);
    pthread_join (pThreadServer, NULL);
    pthread_join (pThreadDisplay, NULL);
	
	
	sem_destroy(&semaLockUpdate); // destroy semaphore
	sem_destroy(&semaLockInfo); // destroy semaphore
	sem_destroy(&semaLockFan); // destroy semaphore
	/*
	sem_destroy(&semaLockSpiSendCommand); // destroy semaphore
	sem_destroy(&semaLockSpiProcess); // destroy semaphore
	*/
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

int main(int argc, char * argv[]) {
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
        settings_file = SETTINGSFILE;
        if(!Settings_Load(settings_file)) {
            //printf("%s\r\n", _lng(ERROR_LOADING_SETTINGS));
            debugPrint(false, false, "Loading Settings!", false, "ERROR");
            return 0;
        }
    }
    
    // Load Settings
    
	
	appNetworkInterface = Settings_Get("general", "network");
    appPort = atoi(Settings_Get("general", "port"));
    appDebugMode = atoi(Settings_Get("general", "debugMode"));
	fanToggle=atoi(Settings_Get("fan", "toggle"));
	fanSystemcode=Settings_Get("fan", "systemcode");
	fanUnitcode=atoi(Settings_Get("fan", "unitcode"));
	
	debugPrint(true, true, "Loading Settings ...", false, "MAIN");
    debugPrint(false, false, "OK", true, "MAIN");

    // Print Debug Infos
    debugPrintInfo();
    debugPrintPinLayout();
	
	// Init Semaphores
	sem_init(&semaLockUpdate, 1, 1);		// initialize mutex to 1 - binary semaphore
	sem_init(&semaLockInfo, 1, 1);			// initialize mutex to 1 - binary semaphore
	sem_init(&semaLockFan, 1, 1);			// initialize mutex to 1 - binary semaphore
	
    // second param = 0 - semaphore is local
	/*
	sem_init(&semaLockSpiSendCommand, 1, 1);      // initialize mutex to 1 - binary semaphore
	sem_init(&semaLockSpiProcess, 1, 1);      // initialize mutex to 1 - binary semaphore
    */
	
	
	//myprintf("Vaibhav %d %d", 10, 15);
	
	
	// Init current Infos -> Setzte alles auf 0
	initCurrentInfo();
	
	debugPrint(true, true, "Initialising wiringPi ...", false, "MAIN");
	if(wiringPiSetup() == -1)  exit(1);
	
	pinMode (CLKPIN, OUTPUT);
	pinMode (DATAPIN, OUTPUT);
	
	debugPrint(false, false, "OK", true, "MAIN");
	
	debugPrint(true, true, "Setting Up Database ...", false, "MAIN");
	createDBs();
	debugPrint(false, false, "OK", true, "MAIN");
	
    // Starting Server Thread
    debugPrint(true, true, "Starting Network-Server ...", true, "MAIN");
    pthread_create (&pThreadServer, NULL, serverMain, 1000);
    //debugPrint(false, false, "OK", true, "MAIN");

	// Starting Display Thread
    debugPrint(true, true, "Starting Display ...", true, "MAIN");
    pthread_create (&pThreadDisplay, NULL, displayMain, NULL);
    //debugPrint(false, false, "OK", true, "MAIN");
	
	// Starting RRD Thread
    debugPrint(true, true, "Starting Database Thread ...", false, "MAIN");
    pthread_create (&pThreadRRD, NULL, mainRRD(), NULL);
    //debugPrint(false, false, "OK", true, "MAIN");
	
	
    for (;;) {
		int res = read_dht22_dat();
		sleep(2);
    }

    // Join Threads
    pthread_join (pThreadServer, NULL);
    pthread_join (pThreadDisplay, NULL);
	
	
	sem_destroy(&semaLockUpdate); // destroy semaphore
	sem_destroy(&semaLockInfo); // destroy semaphore
	sem_destroy(&semaLockFan); // destroy semaphore
	/*
	sem_destroy(&semaLockSpiSendCommand); // destroy semaphore
	sem_destroy(&semaLockSpiProcess); // destroy semaphore
	*/
    return 0 ;
}


