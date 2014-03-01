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
	
	char strMin[4];
	char strMax[4];
	char strFloat[6];
	char strSensorID[2];
	
	sprintf(strMin,"min%d",sensorID);
	sprintf(strMax,"max%d",sensorID);
	sprintf(strSensorID, "%d", sensorID);
    
	debugPrint(true, true, "Reading DS18b20 ...", false, "DS18b20");
	
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
	
	strcpy(lines[0],"00 00 00 00 00 00 00 00 00 : crc=00 NO");
	strcpy(lines[1],"00 00 00 00 00 00 00 00 00 : t=00000");
	
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
        
		if (str != NULL) {
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
			if (data.temperature[sensorID].max < t) {
				data.temperature[sensorID].max = t;
				sprintf(strFloat,"%2.2f",data.temperature[sensorID].max);
				Settings_Add("temperature", strMax, strFloat);
				Settings_Save(SETTINGSFILE);
				
			}
			if ((data.temperature[sensorID].min > t) || (data.temperature[sensorID].current == 0.0)){
				data.temperature[sensorID].min = t;
				data.temperature[sensorID].min = t;
				sprintf(strFloat,"%2.2f",data.temperature[sensorID].current);
				Settings_Add("temperature", strMin, strFloat);
				Settings_Save(SETTINGSFILE);
			}
			
			
			if (data.temperature[sensorID].current != t){
				setUpdateDisplay(true);
			}
			data.temperature[sensorID].current = t;
			
			/*
			char *string[100];
			sprintf(string, "Temperature %d = %.2f *C", sensorID, current.temperature[sensorID] );
			//printf("Temperature %d = %.2f *C \n", sensorID, current.temperature[sensorID] );
			debugPrint(true, true, string, true, "DS18b20");
			*/
			sem_post(&semaLockInfo);       // up semaphore
			//free(substr);
		} else {
			debugPrint(false, false, "", true, "DS18b20");
			debugPrint(true, true, "Warning: CRC Error - DS18b20 SensorID: ", false, "DS18b20");
			debugPrint(false, false, strSensorID, true, "DS18b20");
			
			readTemperatureDS(sensorID);
		}
		
    } else {
		debugPrint(false, false, "", true, "DS18b20");
		debugPrint(true, true, "Warning: CRC Error - DS18b20 SensorID: ", false, "DS18b20");
		debugPrint(false, false, strSensorID, true, "DS18b20");
		
        readTemperatureDS(sensorID);
    }
	
	debugPrint(false, false, "OK", true, "");
}

void* sensorsMain(void *args){
	debugPrint(true, true, "Sensor Thread started", true, "DS18b20");
	//debugPrint(false, false, "OK", true, "RRDTOOL");
	for (;;) {
		debugPrint(true, true, "Reading DHT22 ...", false, "DS18b20");
		read_dht22_dat();
		debugPrint(false, false, "OK", true, "DS18b20");
		
		debugPrint(true, true, "Reading DS18b20 ...", false, "DS18b20");
		//current.temperature[1]=20.0;
		//readTemperatureDS(1);
		readTemperatureDS(2);
		debugPrint(false, false, "OK", true, "DS18b20");
		
		//sleep(1);
	}
	
	return 0;
}


