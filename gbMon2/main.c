//
//  main.c
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
// g++ send.o ../rcswitchwrapper.o ../RCSwitch.o -o send -lwiringPi -g

#include "main.h"

#define TEMPFANON		27.0
#define TEMPFANOFF		26.0

pthread_t	tid[4];
pthread_t	pThreadServer;				// Network Server Thread
pthread_t	pThreadDisplay;				// SPI Send Command to ATmega / Arduino
pthread_t	pThreadSensors;				// Sensors Thread
pthread_t	pThreadRRD;					// RRD Thread

float		appVersion              =   0.1;
bool		updateDisplay			=	false;

// Fan
int			fanToggle				=	0;
int			fanToggleTemp			=	0;
char*		fanSystemcode			=	"10001";
int			fanUnitcode				=	1;

struct mdnsd *svr;


void createBonjourService(){
	// create host entries
	char *hostname = "RaspberryPi.local";
	
	long long_address = inet_addr (getIP(appNetworkInterface)) ;
	struct in_addr addr;
	addr.s_addr = long_address;
	char *dot_ip = inet_ntoa(addr);
	

	printf("IP-Address used for Bonjour Service: %s\n",dot_ip);
	
	
	svr = mdnsd_start();
	if (svr == NULL) {
		printf("mdnsd_start() error\n");
		return 1;
	}
	
	mdnsd_set_hostname(svr, hostname, dot_ip);
	//mdnsd_set_hostname(svr, hostname, inet_addr(ipAddress));
	
	struct rr_entry *a2_e = NULL;
	a2_e = rr_create_a(create_nlabel(hostname), dot_ip);
	//a2_e = rr_create_a(create_nlabel(hostname), inet_addr(ipAddress));
	mdnsd_add_rr(svr, a2_e);
	
	struct rr_entry *aaaa_e = NULL;
	
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_flags = AI_NUMERICHOST;
	struct addrinfo* results;
	getaddrinfo(
				"fe80::5ab0:35ff:fe7f:cf27",
				NULL,
				&hints,
				&results);
	struct sockaddr_in6* addr = (struct sockaddr_in6*)results->ai_addr;
	struct in6_addr v6addr = addr->sin6_addr;
	freeaddrinfo(results);
	
	aaaa_e = rr_create_aaaa(create_nlabel(hostname), &v6addr);
	
	mdnsd_add_rr(svr, aaaa_e);
	
	const char *txt[] = {
		"gbMon2 Service",
		NULL
	};
	struct mdns_service *svc = mdnsd_register_svc(svr, "gbMon2",
												  "_gbmon._tcp.local", 1000, NULL, txt);
	mdns_service_destroy(svc);
}

void resetTemperature(){
	int i = 0;
	sem_wait(&semaLockInfo);		// down semaphore
	for ( i=0; i < NOTEMPSENSOR ; i++) {
		char* strFloat;
		char* strMin;
		char* strMax;
		sprintf(strMin, "min%d",i);
		sprintf(strMax, "max%d",i);
		
		current.minTemp[i]=current.temperature[i];
		current.maxTemp[i]=current.temperature[i];
		
		sprintf(strFloat,"%2.2f",current.minTemp[i]);
		Settings_Add("temperature", strMin, strFloat);
		Settings_Save(SETTINGSFILE);
		
		sprintf(strFloat,"%2.2f",current.maxTemp[i]);
		Settings_Add("temperature", strMax, strFloat);
		Settings_Save(SETTINGSFILE);
		
	}
	sem_post(&semaLockInfo);       // up semaphore
}

void resetHumidity(){
	
	char* strFloat;
	sem_wait(&semaLockInfo);		// down semaphore
	current.minHum=current.humidity;
	current.maxHum=current.humidity;
	
	sprintf(strFloat,"%2.2f",current.minHum);
	Settings_Add("humidity", "min", strFloat);
	Settings_Save(SETTINGSFILE);
	
	sprintf(strFloat,"%2.2f",current.maxHum);
	Settings_Add("humidity", "max", strFloat);
	Settings_Save(SETTINGSFILE);
	
	sem_post(&semaLockInfo);       // up semaphore
}


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

int getLightValue(){
	int result;
	sem_wait(&semaLockInfo);       // down semaphore
	result = current.lightValue;
	sem_post(&semaLockInfo);       // up semaphore
	return result;
}

void setLightValue(int value){
	sem_wait(&semaLockInfo);       // down semaphore
	current.lightValue = value;
	sem_post(&semaLockInfo);       // up semaphore
}

int getFanToggleTemp(){
	int result;
	sem_wait(&semaLockFanTemp);       // down semaphore
	result = fanToggleTemp;
	sem_post(&semaLockFanTemp);       // up semaphore
	return result;
}

void setFanToggleTemp(int value){
	sem_wait(&semaLockFanTemp);       // down semaphore
	fanToggleTemp = value;
	sem_post(&semaLockFanTemp);       // up semaphore
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


void closeApp(){
	
	// Stop Bonjour Service
	mdnsd_stop(svr);
	
	pthread_join (pThreadServer, NULL);
    pthread_join (pThreadDisplay, NULL);
	pthread_join (pThreadRRD, NULL);
	pthread_join (pThreadSensors, NULL);
	
	sem_destroy(&semaLockUpdate); // destroy semaphore
	sem_destroy(&semaLockInfo); // destroy semaphore
	sem_destroy(&semaLockFan); // destroy semaphore
	sem_destroy(&semaLockFanTemp); // destroy semaphore
	
	sem_destroy(&semaLockCam); // destroy semaphore
	sem_destroy(&semaLockPrint); // destroy semaphore
}

void  INThandler(int sig) {
    //char  c;
    
    // Join Threads
    closeApp();
	
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
	
	int i;
	

    
    debugPrint(true, true, "============================================================", true, "MAIN");
    debugPrint(true, true, "                Raspberry Pi - gbMon", true, "MAIN");
    debugPrint(true, true, "============================================================", true, "MAIN");

	
	// Init Semaphores
	sem_init(&semaLockUpdate, 1, 1);		// initialize mutex to 1 - binary semaphore
	sem_init(&semaLockInfo, 1, 1);			// initialize mutex to 1 - binary semaphore
	sem_init(&semaLockFan, 1, 1);			// initialize mutex to 1 - binary semaphore
	sem_init(&semaLockFanTemp, 1, 1);		// initialize mutex to 1 - binary semaphore
	sem_init(&semaLockCam, 1, 1);			// initialize mutex to 1 - binary semaphore
	sem_init(&semaLockPrint, 1, 1);			// initialize mutex to 1 - binary semaphore
	
	
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
            //debugPrint(false, false, "Loading settings!", false, "ERROR");
			printf("Error loading settings! \n");
			printf("Check if settingsfile exists at %s\n", SETTINGSFILE);
			printf("Exiting!");
            return 0;
        }
    }
    
    // Load Settings General
	appNetworkInterface = Settings_Get("general", "network");
    appPort = atoi(Settings_Get("general", "port"));
    appDebugMode = atoi(Settings_Get("general", "debugMode"));
	
	
	debugPrint(true, true, "Loading Settings ...", false, "MAIN");
	// Load Settings for FAN
	sem_wait(&semaLockFan);       // down semaphore
	fanToggle=atoi(Settings_Get("fan", "toggle"));
	sem_post(&semaLockFan);       // up semaphore
	
	fanSystemcode=Settings_Get("fan", "systemcode");
	fanUnitcode=atoi(Settings_Get("fan", "unitcode"));
	
	// Load Settings for min and max values
	sem_wait(&semaLockInfo);       // down semaphore
	// Humidity
	current.minHum=atof(Settings_Get("humidity", "min"));
	current.maxHum=atof(Settings_Get("humidity", "max"));
	// Temperature
	/*
	for ( i=0; i < NOTEMPSENSOR ; i++) {
		char* strMin;
		char* strMax;
		if (i==0) {
			sprintf(strMin, "min");
			sprintf(strMax, "max");
		} else {
			sprintf(strMin, "min%d",i);
			sprintf(strMax, "max%d",i);
		}
		current.minTemp[i]=atof(Settings_Get("temperature", strMin));
		current.maxTemp[i]=atof(Settings_Get("temperature", strMax));
	}
	 */
	sem_post(&semaLockInfo);       // up semaphore
    debugPrint(false, false, "OK", true, "MAIN");

    // Print Debug Infos
    debugPrintInfo();
    debugPrintPinLayout();
	
    // second param = 0 - semaphore is local
	/*
	sem_init(&semaLockSpiSendCommand, 1, 1);      // initialize mutex to 1 - binary semaphore
	sem_init(&semaLockSpiProcess, 1, 1);      // initialize mutex to 1 - binary semaphore
    */
	
	
	//myprintf("Vaibhav %d %d", 10, 15);
	
	
	// Init current Infos -> Setzte alles auf 0
	debugPrint(true, true, "Initialising current information ...", false, "MAIN");
	initCurrentInfo();
	debugPrint(false, false, "OK", true, "MAIN");
	
	debugPrint(true, true, "Setting up directories ...", false, "MAIN");
	char* pathToCheck = "/home/pi/.gbMon/ramdisk/db";
	if(checkDirExist(pathToCheck) == 0){
		mkdir(pathToCheck, 0750);
		chown(pathToCheck, "pi", "pi");
	}
	pathToCheck = "/home/pi/.gbMon/ramdisk/pics";
	if(checkDirExist(pathToCheck) == 0){
		mkdir(pathToCheck, 0750);
		chown(pathToCheck, "pi", "pi");
	}
	pathToCheck = "/home/pi/.gbMon/ramdisk/graphs";
	if(checkDirExist(pathToCheck) == 0){
		mkdir(pathToCheck, 0750);
		chown(pathToCheck, "pi", "pi");
	}
	debugPrint(false, false, "OK", true, "MAIN");
	
	debugPrint(true, true, "Initialising wiringPi ...", false, "MAIN");
	if(wiringPiSetup() == -1)  exit(1);

	pinMode (CLKPIN, OUTPUT);
	pinMode (DATAPIN, OUTPUT);
	debugPrint(false, false, "OK", true, "MAIN");
	
	debugPrint(true, true, "Setting up database ...", false, "MAIN");
	createDBs();
	debugPrint(false, false, "OK", true, "MAIN");

	// Starting Server Thread
    debugPrint(true, true, "Starting network-server ...", true, "MAIN");
    pthread_create (&pThreadServer, NULL, serverMain, 1000);
    //debugPrint(false, false, "OK", true, "MAIN");
	
	// Starting Display Thread
    debugPrint(true, true, "Starting display thread ...", true, "MAIN");
    pthread_create (&pThreadDisplay, NULL, displayMain, NULL);
    //debugPrint(false, false, "OK", true, "MAIN");
	
	// Starting Sensors Thread
    debugPrint(true, true, "Starting sensors thread ...", true, "MAIN");
	pthread_create (&pThreadSensors, NULL, sensorsMain, NULL);
	
	// Starting RRD Thread
    debugPrint(true, true, "Starting database thread ...", true, "MAIN");
    pthread_create (&pThreadRRD, NULL, rrdMain, NULL);
    //debugPrint(false, false, "OK", true, "MAIN");
	
	
	
	// Starting Bonjour Service
    debugPrint(true, true, "Starting bonjour service ...", true, "MAIN");
	createBonjourService();
	debugPrint(false, false, "OK", true, "MAIN");
	
	setLightValue(720);

    for (;;) {
	
		/*
         * Warnings - Temperature HIGH -> FAN On
         * ************************************
         */
        if (strtod(getTemperature(),NULL)>=TEMPFANON && getFanToggleTemp()==0) {
			debugPrint(true, true, "Temperature too high --> FAN ON", true, "MAIN");
			setFan();
			setFanToggleTemp(1);
		}
		/*
         * Warnings - Temperature OK -> FAN Off
         * ************************************
         */
        if (strtod(getTemperature(),NULL)<=TEMPFANOFF && getFanToggleTemp()==1) {
			debugPrint(true, true, "Temperature OK --> FAN OFF", true, "MAIN");
			setFan();
			setFanToggleTemp(0);
		}
		
	}

	
	closeApp();
	/*
    // Join Threads
    pthread_join (pThreadServer, NULL);
    pthread_join (pThreadDisplay, NULL);
	pthread_join (pThreadRRD, NULL);
	pthread_join (pThreadSensors, NULL);
	
	sem_destroy(&semaLockUpdate); // destroy semaphore
	sem_destroy(&semaLockInfo); // destroy semaphore
	sem_destroy(&semaLockFan); // destroy semaphore
	

	sem_destroy(&semaLockSpiSendCommand); // destroy semaphore
	sem_destroy(&semaLockSpiProcess); // destroy semaphore
	*/
    return 0 ;
}


