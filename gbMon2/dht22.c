//
//  dht22.c
//  gbMon2
//
//  Created by michael on 31.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include "dht22.h"

static int dht22_dat[5] = {0,0,0,0,0};

uint8_t sizecvt(const int read) {
	/* digitalRead() and friends from wiringpi are defined as returning a value
	 < 256. However, they are returned as int() types. This is a safety function */
	
	if (read > 255 || read < 0) {
		printf("Invalid data from wiringPi library\n");
		exit(EXIT_FAILURE);
	}
	return (uint8_t)read;
}

int read_dht22_dat() {
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	
	char strFloat[6];
	
	dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;
	
	// pull pin down for 18 milliseconds
	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, LOW);
	delay(18);
	// then pull it up for 40 microseconds
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40);
	// prepare to read the pin
	pinMode(DHTPIN, INPUT);
	
	// detect change and read data
	for ( i=0; i< MAXTIMINGS; i++) {
		counter = 0;
		while (sizecvt(digitalRead(DHTPIN)) == laststate) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) {
				break;
			}
		}
		laststate = sizecvt(digitalRead(DHTPIN));
		
		if (counter == 255) break;
		
		// ignore first 3 transitions
		if ((i >= 4) && (i%2 == 0)) {
			// shove each bit into the storage bytes
			dht22_dat[j/8] <<= 1;
			if (counter > 16)
				dht22_dat[j/8] |= 1;
			j++;
		}
	}
	
	// check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	// print it out if data is good
	if ((j >= 40) && (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
        float t, h;
        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
        h /= 10;
        t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
        t /= 10.0;
        if ((dht22_dat[2] & 0x80) != 0)  t *= -1;
		
		/**
		 * Semaphore DOWN
		 *
		 */
		sem_wait(&semaLockInfo);       // down semaphore
		
		/**
		 *	Check if Temperature is greater than maxTemp
		 *		true:	Save into config File and current.maxTemp[0]
		 */
		if (data.temperature[0].max < t) {
			data.temperature[0].max = t;
            
			 sprintf(strFloat,"%2.2f",data.temperature[0].max);
			 Settings_Add("temperature", "max", strFloat);
			 Settings_Save(SETTINGSFILE);

		}
		
		/**
		 *	Check if Temperature is lower than minTemp
		 *		true:	Save into config File and current.minTemp[0]
		 */
		if ((data.temperature[0].min > t) || (data.temperature[0].current == 0.0)){
			data.temperature[0].min = t;
            
			 sprintf(strFloat,"%2.2f",data.temperature[0].min);
			 Settings_Add("temperature", "min", strFloat);
			 Settings_Save(SETTINGSFILE);
            
		}
		
		/**
		 *	Check if Humidity is greater than maxHum
		 *		true:	Save into config File and current.maxHum
		 */
		if (data.humidity.max < h) {
			data.humidity.max = h;
            
			 sprintf(strFloat,"%2.2f",data.humidity.max);
			 Settings_Add("humidity", "max", strFloat);
			 Settings_Save(SETTINGSFILE);
            
		}
		
		/**
		 *	Check if Humidity is lower than minHum
		 *		true:	Save into config File and current.minHum
		 */
		if ((data.humidity.min > h) || (data.humidity.current == 0.0)){
			data.humidity.min = h;
            
			 sprintf(strFloat,"%2.2f",data.humidity.min);
			 Settings_Add("humidity", "min", strFloat);
			 Settings_Save(SETTINGSFILE);
            
		}
		
		/**
		 * Update Display
		 *
		 */
		if (data.temperature[0].current != t){
			data.temperature[0].current = t;
			setUpdateDisplay(true);
			//printf("Humidity = %.2f %% Temp = %.2f *C \n", h, t );
		} else {
			data.temperature[0].current = t;
		}
		
		if (data.humidity.current != h){
			data.humidity.current = h;
			setUpdateDisplay(true);
			//printf("Hum = %.2f %% Temperature = %.2f *C \n", h, t );
		} else {
			data.humidity.current = h;
		}
		
		
		/**
		 * Debug
		 */
		char string[100];
		sprintf(string, "Humidity = %.2f %% Temperature = %.2f *C",  data.humidity.current, data.temperature[0].current);
		debugPrint(true, true, string, true, "DHT22");
		
		/**
		 * Semaphore UP
		 */
		sem_post(&semaLockInfo);       // up semaphore
		
		return 1;
	} else {
		//printf("Data not good, skip --> Read again \n");
		sleep(1);
		read_dht22_dat();
		return 0;
	}
}