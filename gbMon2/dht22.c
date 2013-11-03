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
		
		
		sem_wait(&semaLockInfo);       // down semaphore
		if (current.maxTemp[0] < t) {
			current.maxTemp[0] = t;
		}
		if ((current.minTemp[0] > t) || (current.temperature[0] == 0.0)){
			current.minTemp[0] = t;
		}
		
		if (current.maxHum < h) {
			current.maxHum = h;
		}
		
		if ((current.minHum > h) || (current.minHum == 0.0)){
			current.minHum = h;
		}
		
		if (current.temperature[0] != t){
			setUpdateDisplay(true);
			printf("Humidity = %.2f %% Temp = %.2f *C \n", h, t );
		}
		if (current.humidity != h){
			setUpdateDisplay(true);
			printf("Hum = %.2f %% Temperature = %.2f *C \n", h, t );
		}
		
		current.temperature[0] = t;
		current.humidity = h;
		sem_post(&semaLockInfo);       // up semaphore
		
		
		return 1;
	} else {
		//printf("Data not good, skip --> Read again \n");
		sleep(1);
		read_dht22_dat();
		return 0;
	}
}