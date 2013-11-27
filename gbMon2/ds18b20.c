//
//  ds18b20.c
//  gbMon2
//
//  Created by michael on 27.11.13.
//  Copyright (c) 2013 michael. All rights reserved.
//


#include "ds18b20.h"

int debug=1;

/*
 * Read Temperature from Sensor DS18b20 and store it in current.temperatur[sensorID]
 *********************************************************************************
 */
void readTemperatureDS(int sensorID) {
    FILE * fp;
    char * line = NULL;
    char lines[2][100];
    size_t len = 0;
    ssize_t read;
    float t;
    
    
	printf("readTemperatureDS --> SensorID: %d \n", sensorID);
	
    if (sensorID==1) {
        fp = fopen("/sys/bus/w1/devices/w1_bus_master1/28-000004e3da40/w1_slave", "r");
    }
    if (sensorID==2){
        fp = fopen("/sys/bus/w1/devices/w1_bus_master1/28-000004e449dc/w1_slave", "r");
    }
    
    
    if (fp == NULL) {
        printf(">>>ERROR<<<: getTemperature fp==Null \n");
        printf("ERROR NUMBER: %d\n", errno);
        //exit(EXIT_FAILURE);
        fclose(fp);
        return readTemperatureDS(sensorID);
    }
    
    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);
        strcpy(lines[i],line);
        i++;
    }
    
    if (line)
        free(line);
    
    fclose(fp);
    
    char * ptrToSubString;
    ptrToSubString = strstr(lines[0],"YES");
    
    // now if that pointer to the substring is NOT null, then we've found our substring.
    // Let's check it out:
    if (ptrToSubString != NULL) {
        //printf("We found the string: %s\n",ptrToSubString);
        
        const char* str    = lines[1];
        size_t      begin  = 29;
        size_t      end    = 5;
        
        
        char*       substr = substring(str, begin, end);
        /*
         printf("str    = %s\n", str);
         printf("substr starting from pos %u, and %u characters in length...\n", begin, end);
         printf("substr = %s which has a strlen of %u\n", substr, strlen(substr));
         */
        
        int num = atoi(substr);
        //return num / 1000;
		
		t=num/1000;
		
		sem_wait(&semaLockInfo);       // down semaphore
		if (current.maxTemp[sensorID] < t) {
			current.maxTemp[sensorID] = t;
		}
		if ((current.minTemp[sensorID] > t) || (current.temperature[sensorID] == 0.0)){
			current.minTemp[sensorID] = t;
		}
		
		
		if (current.temperature[sensorID] != t){
			setUpdateDisplay(true);
			printf("Temperature %d = %.2f *C \n", sensorID, t );
		}
		
		current.temperature[sensorID] = t;
		sem_post(&semaLockInfo);       // up semaphore
		
		
        
        
        //free(substr);
    }
    else {
        if (debug==1){
            printf("%s >>> Warning: CRC Error - DS18b20 SensorID: %d \n", getTime(),sensorID);
        }
        readTemperatureDS(sensorID);
    }
}